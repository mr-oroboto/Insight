#include "FrameQueue.h"
#include <iostream>

FrameQueue::FrameQueue(DisplayManager* dm, bool repeats)
{
    ready = false;

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

Frame* FrameQueue::newFrame()
{
    Frame* frame = new Frame(displayManager);

    return frame;
}

/**
 * Take ownership of a Frame created via newFrame()
 */
bool FrameQueue::enqueueFrame(Frame* frame)
{
    if (repeating && ready)
    {
        std::cerr << "Repeating FrameQueue has been made ready, new Frames cannot be added" << std::endl;
        return false;
    }

    queue.push(frame);
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

void FrameQueue::drawCurrentFrame(float time)
{
    // @todo: fix
    Frame* frame = queue.front();

    if (frame)
    {
        frame->draw(time);
    }
}