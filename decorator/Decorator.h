#ifndef INSIGHT_DECORATOR_H
#define INSIGHT_DECORATOR_H

#include <glm/glm.hpp>
#include "../FrameQueue.h"
#include "../Frame.h"

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


#endif //INSIGHT_DECORATOR_H
