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
    FrameQueue(DisplayManager* dm, bool repeats);
    ~FrameQueue();

    Frame* newFrame();
    bool enqueueFrame(Frame*);

    bool setActive();
    bool setReady();
    void setFrameRate(GLfloat fps);

    void drawCurrentFrame();

protected:
    bool repeating;
    bool ready;

    GLfloat frameRate;
    GLfloat secsPerFrame;

    std::chrono::high_resolution_clock::time_point lastFrameDrawnAt;
    std::chrono::high_resolution_clock::time_point firstFrameDrawnAt;

    DisplayManager* displayManager;
    std::queue<Frame*> queue;
};

#endif //INSIGHT_CORE_FRAMEQUEUE_H
