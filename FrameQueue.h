#ifndef INSIGHT_FRAMEQUEUE_H
#define INSIGHT_FRAMEQUEUE_H

#include "Frame.h"
#include <queue>
#include <chrono>

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

#endif //INSIGHT_FRAMEQUEUE_H
