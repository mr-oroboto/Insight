#ifndef INSIGHT_CORE_FRAMEQUEUE_H
#define INSIGHT_CORE_FRAMEQUEUE_H

#include <memory>
#include <queue>
#include <chrono>

#include "Frame.h"

#define FRAMEQUEUE_DEFAULT_FPS 30

namespace insight {


class DisplayManager;

class FrameQueue {
public:
    FrameQueue(DisplayManager* display_manager, bool repeats);
    ~FrameQueue();

    // Generate a new Frame that can have SceneObjects added to it.
    //
    // A shared pointer is returned so that the caller can keep a reference to the frame if desired (ie. to use during
    // a scene update callback).
    std::shared_ptr<Frame> newFrame(bool draw_object_position = false, bool draw_reference_axes = false, bool draw_floor = false);

    // Add a frame to the queue. Once added, it will be displayed when the FrameQueue is running on DisplayManager.
    //
    // Callers can keep their reference to the Frame in case they want to modify it during scene redraws etc.
    bool enqueueFrame(std::shared_ptr<Frame> frame);

    void setFrameRate(GLfloat fps);

    // Sets this FrameQueue as the active FrameQueue for the DisplayManager.
    //
    // If this is a repeating FrameQueue it must have been fully initialised with all Frames and made ready. If it's a
    // non-repeating FrameQueue then Frames can be added at any time and it doesn't need to be ready.
    bool setActive();

    // Has the FrameQueue been made active with setActive()?
    bool isActive();

    // Repeating FrameQueues must be made ready before they can be added to a DisplayManager and used. Ready means that
    // no new frames can be added.
    bool setReady();

    // Called by DisplayManager to draw the current frame and cycle to the next one.
    //
    // secs_since_rendering_started (in)   marks the time since the main rendering loop started
    // secs_since_framequeue_started (out) marks the time since this FrameQueue started
    // secs_since_last_renderloop (in)     marks the time since the last rendering loop ran
    // secs_since_last_frame (out)         marks the time since the last frame switch in this FrameQueue
    void drawCurrentFrame(GLfloat secs_since_rendering_started, GLfloat& secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat& secs_since_last_frame);

protected:
    bool repeating_;
    bool ready_;
    bool active_;

    GLfloat frame_rate_;
    GLfloat secs_per_frame_;

    std::chrono::high_resolution_clock::time_point last_frame_drawn_at_;
    std::chrono::high_resolution_clock::time_point first_frame_drawn_at_;

    DisplayManager* display_manager_;
    std::queue<std::shared_ptr<Frame>> queue_;
};


}   // namespace insight

#endif //INSIGHT_CORE_FRAMEQUEUE_H
