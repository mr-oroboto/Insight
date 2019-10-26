#include "FrameQueue.h"

#include <iostream>
#include <thread>

#include "DisplayManager.h"

FrameQueue::FrameQueue(DisplayManager* display_manager, bool repeats)
{
    ready_ = false;

    setFrameRate(FRAMEQUEUE_DEFAULT_FPS);

    display_manager_ = display_manager;
    repeating_ = repeats;
}

FrameQueue::~FrameQueue()
{
    std::cout << "FrameQueue::~FrameQueue()" << std::endl;

    while ( ! queue_.empty())
    {
        Frame* frame = queue_.front();
        queue_.pop();

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
    Frame* frame = new Frame(display_manager_);

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
    if (repeating_ && ready_)
    {
        std::cerr << "Repeating FrameQueue has been made ready, new Frames cannot be added" << std::endl;
        return false;
    }

    queue_.push(frame);

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
    if (repeating_ && ! ready_)
    {
        std::cerr << "Repeating FrameQueue has not been made ready" << std::endl;
        return false;
    }

    display_manager_->setFrameQueue(this);

    return true;
}

/**
 * Repeating FrameQueues must be made ready before they can be added to a DisplayManager and used. Ready means that
 * no new frames can be added.
 */
bool FrameQueue::setReady()
{
    if (queue_.empty())
    {
        std::cerr << "FrameQueue cannot be made ready with no Frames" << std::endl;
        return false;
    }

    ready_ = true;

    return true;
}

void FrameQueue::setFrameRate(GLfloat fps)
{
    frame_rate_ = fps;
    secs_per_frame_ = 1 / frame_rate_;

    first_frame_drawn_at_ = last_frame_drawn_at_ = std::chrono::high_resolution_clock::now();
}

void FrameQueue::drawCurrentFrame()
{
    if (queue_.empty())
    {
        return;
    }

    auto t_now = std::chrono::high_resolution_clock::now();

    GLfloat secs_since_start = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - first_frame_drawn_at_).count();
    GLfloat secs_since_last_frame = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - last_frame_drawn_at_).count();

    Frame* frame = queue_.front();

    if (frame)
    {
        frame->draw(secs_since_start, secs_since_last_frame);

        if (secs_since_last_frame >= secs_per_frame_)
        {
            // Remove the frame so we draw the next frame
            queue_.pop();
            last_frame_drawn_at_ = t_now;

            if (repeating_)
            {
                queue_.push(frame);
            }
            else
            {
                delete frame;
            }
        }
    }
}