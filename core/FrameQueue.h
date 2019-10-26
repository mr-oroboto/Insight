#ifndef INSIGHT_CORE_FRAMEQUEUE_H
#define INSIGHT_CORE_FRAMEQUEUE_H

#include <queue>
#include <chrono>

#include "Frame.h"

#define FRAMEQUEUE_DEFAULT_FPS 30

class DisplayManager;

class FrameQueue
{
public:
    FrameQueue(DisplayManager* display_manager, bool repeats);
    ~FrameQueue();

    Frame* newFrame();
    bool enqueueFrame(Frame*);

    void setFrameRate(GLfloat fps);
    bool setActive();
    bool setReady();

    void drawCurrentFrame();

protected:
    bool repeating_;
    bool ready_;

    GLfloat frame_rate_;
    GLfloat secs_per_frame_;

    std::chrono::high_resolution_clock::time_point last_frame_drawn_at_;
    std::chrono::high_resolution_clock::time_point first_frame_drawn_at_;

    DisplayManager* display_manager_;
    std::queue<Frame*> queue_;
};

#endif //INSIGHT_CORE_FRAMEQUEUE_H
