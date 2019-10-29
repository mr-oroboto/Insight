#ifndef INSIGHT_DECORATOR_HEAPDECORATOR_H
#define INSIGHT_DECORATOR_HEAPDECORATOR_H

#include <glm/glm.hpp>

#include "Decorator.h"
#include "core/FrameQueue.h"

class HeapArray;

class HeapDecorator : public Decorator
{
public:
    HeapDecorator(FrameQueue* frame_queue) : Decorator(frame_queue) {}

    void setHeap(HeapArray* heap);
    void setHightlightedNodeValue(int node_value);

    bool decorate();

protected:
    void dft(size_t node_index, unsigned long node_level, const glm::vec3& last_node_coords, bool is_left_of_parent);

    HeapArray* heap_;
    int highlighted_node_value_;
};


#endif //INSIGHT_DECORATOR_HEAPDECORATOR_H
