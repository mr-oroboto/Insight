#ifndef INSIGHT_DECORATOR_HEAPDECORATOR_H
#define INSIGHT_DECORATOR_HEAPDECORATOR_H

#include <glm/glm.hpp>

#include "Decorator.h"
#include "core/FrameQueue.h"

namespace insight {

namespace adt {
class HeapArray;
}

namespace decorator {

class HeapDecorator : public Decorator {
public:
    enum TraverseOrder {
        PRE_ORDER = 0,
        IN_ORDER,
        POST_ORDER
    };

    HeapDecorator(FrameQueue *frame_queue) : Decorator(frame_queue) { }

    void setHeap(adt::HeapArray *heap);
    void setHightlightedNodeValue(int node_value);

    void decorate();
    void decorateDft(TraverseOrder traverse_order);
    void decorateBft();

protected:
    void dft(size_t node_index, unsigned long node_level, const glm::vec3 &last_node_coords, bool is_left_of_parent, TraverseOrder traverse_order = TraverseOrder::IN_ORDER, std::shared_ptr<Frame> prototype_frame = nullptr);
    void drawNode(size_t node_index, const glm::vec3 &node_position, const glm::vec3 &last_node_coords, std::shared_ptr<Frame> prototype_frame);
    void bft(std::shared_ptr<Frame> prototype_frame);

    adt::HeapArray *heap_;
    int highlighted_node_value_;
};

}   // namespace decorator
}   // namespace insight

#endif //INSIGHT_DECORATOR_HEAPDECORATOR_H
