#ifndef INSIGHT_DECORATOR_DECORATOR_H
#define INSIGHT_DECORATOR_DECORATOR_H

#include "core/FrameQueue.h"
#include "core/Frame.h"

class Decorator
{
public:
    Decorator(FrameQueue* queue) : frameQueue(queue) { currentFrame = nullptr; }
    virtual ~Decorator();

    virtual bool decorate() = 0;

protected:
    FrameQueue* frameQueue;
    Frame* currentFrame;
};


#endif //INSIGHT_DECORATOR_DECORATOR_H
