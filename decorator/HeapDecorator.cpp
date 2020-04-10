#include "HeapDecorator.h"

#include <queue>

#include <math.h>

#include "adt/HeapArray.h"
#include "core/DisplayManager.h"

void insight::decorator::HeapDecorator::setHeap(adt::HeapArray* heapArray)
{
    heap_ = heapArray;
}

void insight::decorator::HeapDecorator::setHightlightedNodeValue(int node_value)
{
    highlighted_node_value_ = node_value;
}

void insight::decorator::HeapDecorator::decorate()
{
    current_frame_ = frame_queue_->newFrame();

    glm::vec3 last_node_coords = glm::vec3(0, 5, 0);

    unsigned long node_count = heap_->getNodeCount();
    unsigned long heap_levels = floor(log(node_count) / log(2.0));

    char msg[64];
    sprintf(msg, "Heap insertion. [current value: %d, levels: %lu]", highlighted_node_value_, heap_levels);
    current_frame_->addText(msg, 10, 10);

    dft(0, heap_levels + 1, last_node_coords, false);

    frame_queue_->enqueueFrame(current_frame_);
}

void insight::decorator::HeapDecorator::decorateDft(TraverseOrder traverse_order)
{
    current_frame_ = frame_queue_->newFrame();

    glm::vec3 last_node_coords = glm::vec3(0, 5, 0);

    unsigned long node_count = heap_->getNodeCount();
    unsigned long heap_levels = floor(log(node_count) / log(2.0));

    char msg[64];
    switch (traverse_order)
    {
        case TraverseOrder::PRE_ORDER:
            sprintf(msg, "Heap, pre-order depth first traversal. [levels: %lu]", heap_levels);
            break;
        case TraverseOrder::IN_ORDER:
            sprintf(msg, "Heap, in-order depth first traversal. [levels: %lu]", heap_levels);
            break;
        case TraverseOrder::POST_ORDER:
            sprintf(msg, "Heap, post-order depth first traversal. [levels: %lu]", heap_levels);
            break;
    }
    current_frame_->addText(msg, 10, 10);

    // Do a DFT to render all the nodes into a frame, then do another DFT and clone that prototype frame once per node,
    // in each frame highlight the node that the DFT is currently traversing. Queue up all frames and then delete the
    // prototype frame (or keep it with nothing highlighted, doesn't matter).

    dft(0, heap_levels + 1, last_node_coords, false, traverse_order, nullptr);          // draw all the nodes first
    dft(0, heap_levels + 1, last_node_coords, false, traverse_order, current_frame_);   // now decorate the DFT itself

    frame_queue_->enqueueFrame(current_frame_);
}

void insight::decorator::HeapDecorator::dft(size_t node_index, unsigned long node_level, const glm::vec3& last_node_coords, bool is_left_of_parent, TraverseOrder traverse_order, std::shared_ptr<Frame> prototype_frame)
{
    int left_index = heap_->getLeftIndex(node_index);
    int right_index = heap_->getRightIndex(node_index);

    // Where should this node be drawn?
    glm::vec3 node_position = last_node_coords;
    if (node_index != 0)
    {
        if (is_left_of_parent)
        {
            node_position.x -= (node_level * 2.0);
            node_position.y -= 1.5;
        }
        else
        {
            node_position.x += (node_level * 2.0);
            node_position.y -= 1.5;
        }
    }

    switch (traverse_order)
    {
        case TraverseOrder::PRE_ORDER:
            drawNode(node_index, node_position, last_node_coords, prototype_frame);

            if (left_index >= 0)
            {
                dft(left_index, node_level - 1, node_position, true, traverse_order, prototype_frame);
            }

            if (right_index >= 0)
            {
                dft(right_index, node_level - 1, node_position, false, traverse_order, prototype_frame);
            }

            break;

        case TraverseOrder::IN_ORDER:
            if (left_index >= 0)
            {
                dft(left_index, node_level - 1, node_position, true, traverse_order, prototype_frame);
            }

            drawNode(node_index, node_position, last_node_coords, prototype_frame);

            if (right_index >= 0)
            {
                dft(right_index, node_level - 1, node_position, false, traverse_order, prototype_frame);
            }

            break;

        case TraverseOrder::POST_ORDER:
            if (left_index >= 0)
            {
                dft(left_index, node_level - 1, node_position, true, traverse_order, prototype_frame);
            }

            if (right_index >= 0)
            {
                dft(right_index, node_level - 1, node_position, false, traverse_order, prototype_frame);
            }

            drawNode(node_index, node_position, last_node_coords, prototype_frame);

            break;
    }
}

void insight::decorator::HeapDecorator::drawNode(size_t node_index, const glm::vec3& node_position, const glm::vec3& last_node_coords, std::shared_ptr<Frame> prototype_frame)
{
    int node_value = heap_->getNodeValue(node_index);
    glm::vec3 node_colour;

    if (node_value == highlighted_node_value_)
    {
        node_colour = glm::vec3(1, 0, 0);
    }
    else
    {
        node_colour = glm::vec3(0.8, 0.8, 0.8);
    }

    if (prototype_frame)
    {
        // We're decorating the DFT itself, do this by creating a new frame that highlights the current node
        std::shared_ptr<Frame> frame = prototype_frame->clone();
        frame->deleteObjectsAtPosition(node_position, insight::primitive::Primitive::Type::CUBE);
        frame->addObject(insight::primitive::Primitive::Type::CUBE, node_position, glm::vec3(1, 0, 0), "", 1.5);
        frame_queue_->enqueueFrame(frame);
    }
    else
    {
        current_frame_->addObject(insight::primitive::Primitive::Type::CUBE, node_position, node_colour, "slate", 1.5);

        if (node_index != 0)
        {
            current_frame_->addLine(last_node_coords, node_position, glm::vec3(1, 1, 1));
        }

        char msg[64];
        sprintf(msg, "%d", node_value);
        current_frame_->addText(msg, node_position.x, node_position.y, node_position.z, false, 0.015, glm::vec3(1.0, 1.0, 0.0));
    }
}

void insight::decorator::HeapDecorator::decorateBft()
{
    current_frame_ = frame_queue_->newFrame();

    unsigned long node_count = heap_->getNodeCount();
    unsigned long heap_levels = floor(log(node_count) / log(2.0));

    char msg[64];
    sprintf(msg, "Heap, breadth first traversal. [levels: %lu]", heap_levels);
    current_frame_->addText(msg, 10, 10);

    // Do a BFT to render all the nodes into a frame (which will be used as a prototype frame)
    bft(nullptr);

    // Now do a BFT and clone that prototype frame once per node, in each frame highlight the node that the BFT is
    // currently traversing. Queue up all frames and then delete the prototype frame (or keep it with nothing
    // highlighted, doesn't matter).
    bft(current_frame_);

    frame_queue_->enqueueFrame(current_frame_);
}

void insight::decorator::HeapDecorator::bft(std::shared_ptr<Frame> prototype_frame)
{
    struct NodeDetails {
        size_t node_index;

        glm::vec3 parent_coords;
        unsigned long level;
        bool is_left_of_parent;
    };

    std::queue<NodeDetails> node_queue;

    unsigned long node_count = heap_->getNodeCount();
    unsigned long heap_levels = floor(log(node_count) / log(2.0));

    NodeDetails details;

    details.node_index = 0;
    details.parent_coords = glm::vec3(0, 5, 0);
    details.level = heap_levels + 1;
    details.is_left_of_parent = false;

    node_queue.push(details);

    while ( ! node_queue.empty())
    {
        details = node_queue.front();
        node_queue.pop();

        // Where should this node be drawn?
        glm::vec3 node_position = details.parent_coords;
        if (details.node_index != 0)
        {
            if (details.is_left_of_parent)
            {
                node_position.x -= (details.level * 2.0);
                node_position.y -= 1.5;
            }
            else
            {
                node_position.x += (details.level * 2.0);
                node_position.y -= 1.5;
            }
        }

        if (prototype_frame)
        {
            std::shared_ptr<Frame> frame = prototype_frame->clone();
            frame->deleteObjectsAtPosition(node_position, insight::primitive::Primitive::Type::CUBE);
            frame->addObject(insight::primitive::Primitive::Type::CUBE, node_position, glm::vec3(1, 0, 0), "", 1.5);
            frame_queue_->enqueueFrame(frame);
        }
        else
        {
            current_frame_->addObject(insight::primitive::Primitive::Type::CUBE, node_position, glm::vec3(0.8, 0.8, 0.8), "slate", 1.5);

            if (details.node_index != 0)
            {
                current_frame_->addLine(details.parent_coords, node_position, glm::vec3(1, 1, 1));
            }

            int node_value = heap_->getNodeValue(details.node_index);

            char msg[64];
            sprintf(msg, "%d", node_value);
            current_frame_->addText(msg, node_position.x, node_position.y, node_position.z, false, 0.015, glm::vec3(1.0, 1.0, 0.0));
        }

        int left_index = heap_->getLeftIndex(details.node_index);
        if (left_index > 0)
        {
            NodeDetails left_details = details;
            left_details.node_index = left_index;
            left_details.is_left_of_parent = true;
            left_details.parent_coords = node_position;
            left_details.level = details.level - 1;

            node_queue.push(left_details);
        }

        int right_index = heap_->getRightIndex(details.node_index);
        if (right_index > 0)
        {
            NodeDetails right_details = details;
            right_details.node_index = right_index;
            right_details.is_left_of_parent = false;
            right_details.parent_coords = node_position;
            right_details.level = details.level - 1;

            node_queue.push(right_details);
        }
    }
}