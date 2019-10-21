#ifndef INSIGHT_HEAPDECORATOR_H
#define INSIGHT_HEAPDECORATOR_H

#include "Decorator.h"
#include "../adt/HeapArray.h"

class HeapDecorator : public Decorator
{
public:
    HeapDecorator(HeapArray* h) : heap(h) {}

    bool decorate(Frame* frame);

protected:
    void dft(size_t nodeIndex, glm::vec3 lastNodePosition, bool isLeftOfParent);

    HeapArray* heap;
};


#endif //INSIGHT_HEAPDECORATOR_H
