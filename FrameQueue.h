#ifndef INSIGHT_FRAMEQUEUE_H
#define INSIGHT_FRAMEQUEUE_H

#include "Frame.h"
#include <queue>

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

    void drawCurrentFrame(float time);

protected:
    bool repeating;
    bool ready;

    DisplayManager* displayManager;
    std::queue<Frame*> queue;
};

#endif //INSIGHT_FRAMEQUEUE_H
