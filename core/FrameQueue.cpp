#include "FrameQueue.h"

#include <iostream>
#include <thread>

#include "DisplayManager.h"

FrameQueue::FrameQueue(DisplayManager* dm, bool repeats)
{
    ready = false;

    setFrameRate(FRAMEQUEUE_DEFAULT_FPS);

    displayManager = dm;
    repeating = repeats;
}

FrameQueue::~FrameQueue()
{
    std::cout << "FrameQueue::~FrameQueue()" << std::endl;

    while ( ! queue.empty())
    {
        Frame* frame = queue.front();
        queue.pop();

        delete frame;
    }
}

/**
 * Generate a new Frame that can have SceneObjects added to it.
 *
 * This Frame should either be passed back to this FrameQueue using enqueueFrame(), or deleted by the caller.
 */
Frame* FrameQueue::newFrame()
{
    Frame* frame = new Frame(displayManager);

    return frame;
}

/**
 * Take ownership of a Frame created via newFrame().
 *
 * The Frame should not be referenced by other entities once it has been passed off via this method (it can be deleted
 * during drawCurrentFrame() on non-repeating FrameQueue objects).
 */
bool FrameQueue::enqueueFrame(Frame* frame)
{
    if (repeating && ready)
    {
        std::cerr << "Repeating FrameQueue has been made ready, new Frames cannot be added" << std::endl;
        return false;
    }

    queue.push(frame);

    return true;
}

/**
 * Sets this FrameQueue as the active FrameQueue for the DisplayManager.
 *
 * If this is a repeating FrameQueue it must have been fully initialised with all Frames and made ready. If it's a
 * non-repeating FrameQueue then Frames can be added at any time and it doesn't need to be ready.
 */
bool FrameQueue::setActive()
{
    if (repeating && ! ready)
    {
        std::cerr << "Repeating FrameQueue has not been made ready" << std::endl;
        return false;
    }

    displayManager->setFrameQueue(this);

    return true;
}

/**
 * Repeating FrameQueues must be made ready before they can be added to a DisplayManager and used. Ready means that
 * no new frames can be added.
 */
bool FrameQueue::setReady()
{
    if (queue.empty())
    {
        std::cerr << "FrameQueue cannot be made ready with no Frames" << std::endl;
        return false;
    }

    ready = true;

    return true;
}

void FrameQueue::setFrameRate(GLfloat fps)
{
    frameRate = fps;
    secsPerFrame = 1 / frameRate;

    firstFrameDrawnAt = lastFrameDrawnAt = std::chrono::high_resolution_clock::now();
}

void FrameQueue::drawCurrentFrame()
{
    if (queue.empty())
    {
        return;
    }

    auto t_now = std::chrono::high_resolution_clock::now();

    GLfloat secsSinceStart = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - firstFrameDrawnAt).count();
    GLfloat secsSinceLastFrame = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - lastFrameDrawnAt).count();

    Frame* frame = queue.front();

    if (frame)
    {
        frame->draw(secsSinceStart, secsSinceLastFrame);

        if (secsSinceLastFrame >= secsPerFrame)
        {
            // Remove the frame so we draw the next frame
            queue.pop();
            lastFrameDrawnAt = t_now;

            if (repeating)
            {
                queue.push(frame);
            }
            else
            {
                delete frame;
            }
        }
    }
}