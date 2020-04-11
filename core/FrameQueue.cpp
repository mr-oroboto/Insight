#include "FrameQueue.h"

#include <iostream>
#include <thread>

#include "DisplayManager.h"

namespace insight {


FrameQueue::FrameQueue(DisplayManager* display_manager, bool repeats)
    : ready_(false),
      active_(false),
      display_manager_(display_manager),
      repeating_(repeats)
{
    setFrameRate(FRAMEQUEUE_DEFAULT_FPS);
}

FrameQueue::~FrameQueue()
{
    std::queue<std::shared_ptr<Frame>> empty;
    queue_.swap(empty);
}

std::shared_ptr<Frame> FrameQueue::newFrame(bool draw_object_position, bool draw_reference_axes, bool draw_floor)
{
    return std::make_shared<Frame>(display_manager_, draw_object_position, draw_reference_axes, draw_floor);
}

bool FrameQueue::enqueueFrame(std::shared_ptr<Frame> frame)
{
    if (repeating_ && ready_)
    {
        std::cerr << "Repeating FrameQueue has been made ready, new Frames cannot be added" << std::endl;
        return false;
    }

    queue_.push(frame);

    return true;
}

bool FrameQueue::setActive()
{
    if (repeating_ && ! ready_)
    {
        std::cerr << "Repeating FrameQueue has not been made ready" << std::endl;
        return false;
    }

    active_ = true;

    return true;
}

bool FrameQueue::isActive()
{
    return active_;
}

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

void FrameQueue::drawCurrentFrame(GLfloat secs_since_rendering_started, GLfloat& secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat& secs_since_last_frame)
{
    if (queue_.empty())
    {
        return;
    }

    auto t_now = std::chrono::high_resolution_clock::now();

    secs_since_framequeue_started = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - first_frame_drawn_at_).count();
    secs_since_last_frame = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - last_frame_drawn_at_).count();

    std::shared_ptr<Frame> frame = queue_.front();
    assert(frame);

    frame->draw(secs_since_rendering_started, secs_since_framequeue_started, secs_since_last_renderloop, secs_since_last_frame);

    if (secs_since_last_frame >= secs_per_frame_)
    {
        // Remove the frame so we draw the next frame
        queue_.pop();
        last_frame_drawn_at_ = t_now;

        if (repeating_)
        {
            queue_.push(frame);
        }
    }
}


}   // namespace insight