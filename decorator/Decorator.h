#ifndef INSIGHT_DECORATOR_DECORATOR_H
#define INSIGHT_DECORATOR_DECORATOR_H

#include "core/FrameQueue.h"
#include "core/Frame.h"

class Decorator
{
public:
    Decorator(FrameQueue* queue) : frame_queue_(queue) { current_frame_ = nullptr; }
    virtual ~Decorator();

    virtual bool decorate() = 0;

protected:
    FrameQueue* frame_queue_;
    Frame* current_frame_;
};

#endif //INSIGHT_DECORATOR_DECORATOR_H
