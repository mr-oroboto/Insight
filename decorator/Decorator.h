#ifndef INSIGHT_DECORATOR_H
#define INSIGHT_DECORATOR_H

#include <glm/glm.hpp>

class Frame;

class Decorator
{
public:
    Decorator();
    virtual ~Decorator();

    virtual bool decorate(Frame* frame) = 0;

protected:
    Frame* currentFrame;
};


#endif //INSIGHT_DECORATOR_H
