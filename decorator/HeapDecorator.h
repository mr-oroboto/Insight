#ifndef INSIGHT_DECORATOR_HEAPDECORATOR_H
#define INSIGHT_DECORATOR_HEAPDECORATOR_H

#include <glm/glm.hpp>

#include "Decorator.h"
#include "core/FrameQueue.h"

class HeapArray;

class HeapDecorator : public Decorator
{
public:
    HeapDecorator(FrameQueue* frameQueue) : Decorator(frameQueue) {}

    void setHeap(HeapArray* heap);
    void setHightlightedNodeValue(int nodeValue);

    bool decorate();

protected:
    void dft(size_t nodeIndex, unsigned long nodeLevel, glm::vec3 lastNodePosition, bool isLeftOfParent);

    HeapArray* heap;
    int highlightedNodeValue;
};


#endif //INSIGHT_DECORATOR_HEAPDECORATOR_H
