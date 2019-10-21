#ifndef INSIGHT_HEAPDECORATOR_H
#define INSIGHT_HEAPDECORATOR_H

#include "Decorator.h"

class HeapArray;

class HeapDecorator : public Decorator
{
public:
    HeapDecorator(FrameQueue* frameQueue) : Decorator(frameQueue) {}

    void setHeap(HeapArray* heap);
    void setHightlightedNodeValue(int nodeValue);

    bool decorate();

protected:
    void dft(size_t nodeIndex, glm::vec3 lastNodePosition, bool isLeftOfParent);

    HeapArray* heap;
    int highlightedNodeValue;
};


#endif //INSIGHT_HEAPDECORATOR_H
