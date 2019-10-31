#include "HeapDecorator.h"

#include <math.h>

#include "adt/HeapArray.h"
#include "core/DisplayManager.h"

void HeapDecorator::setHeap(HeapArray* heapArray)
{
    heap_ = heapArray;
}

void HeapDecorator::setHightlightedNodeValue(int node_value)
{
    highlighted_node_value_ = node_value;
}

bool HeapDecorator::decorate()
{
    bool success = true;

    current_frame_ = frame_queue_->newFrame();

    glm::vec3 last_node_coords = glm::vec3(0, 5, 0);

    unsigned long node_count = heap_->getNodeCount();
    unsigned long heap_levels = floor(log(node_count) / log(2.0));

    char msg[64];
    sprintf(msg, "current value: %d, levels: %lu", highlighted_node_value_, heap_levels);
    current_frame_->addText(msg, 10, 10);

    dft(0, heap_levels + 1, last_node_coords, false);

    frame_queue_->enqueueFrame(current_frame_);

    return success;
}

bool HeapDecorator::decorateDft(TraverseOrder traverse_order)
{
    bool success = true;

    current_frame_ = frame_queue_->newFrame();

    glm::vec3 last_node_coords = glm::vec3(0, 5, 0);

    unsigned long node_count = heap_->getNodeCount();
    unsigned long heap_levels = floor(log(node_count) / log(2.0));

    char msg[64];
    switch (traverse_order)
    {
        case TraverseOrder::PRE_ORDER:
            sprintf(msg, "pre-order depth first traversal, levels: %lu", heap_levels);
            break;
        case TraverseOrder::IN_ORDER:
            sprintf(msg, "in-order depth first traversal, levels: %lu", heap_levels);
            break;
        case TraverseOrder::POST_ORDER:
            sprintf(msg, "post-order depth first traversal, levels: %lu", heap_levels);
            break;
    }
    current_frame_->addText(msg, 10, 10);

    // Do a DFT to render all the nodes into a frame, then do another DFT and clone that prototype frame once per node,
    // in each frame highlight the node that the DFT is currently traversing. Queue up all frames and then delete the
    // prototype frame (or keep it with nothing highlighted, doesn't matter).

    dft(0, heap_levels + 1, last_node_coords, false, traverse_order);                   // draw all the nodes first
    dft(0, heap_levels + 1, last_node_coords, false, traverse_order, current_frame_);   // now decorate the DFT itself

    frame_queue_->enqueueFrame(current_frame_);

    return success;
}

void HeapDecorator::dft(size_t node_index, unsigned long node_level, const glm::vec3& last_node_coords, bool is_left_of_parent, TraverseOrder traverse_order, Frame* prototype_frame)
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

void HeapDecorator::drawNode(size_t node_index, const glm::vec3& node_position, const glm::vec3& last_node_coords, Frame* prototype_frame)
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

    Texture* texture = frame_queue_->getDisplayManager()->getTextureCollection()->getTexture("slate");

    if (prototype_frame)
    {
        // We're decorating the DFT itself, do this by creating a new frame that highlights the current node
        Frame* frame = prototype_frame->clone();
        frame->deleteObjectsAtPosition(node_position, Primitive::Type::CUBE);
        frame->addObject(Primitive::Type::CUBE, node_position, glm::vec3(1, 0, 0), nullptr, 1.5);
        frame_queue_->enqueueFrame(frame);
    }
    else
    {
        current_frame_->addObject(Primitive::Type::CUBE, node_position, node_colour, texture, 1.5);

        if (node_index != 0)
        {
            current_frame_->addLine(last_node_coords, node_position, glm::vec3(1, 1, 1));
        }

        char msg[64];
        sprintf(msg, "%d", node_value);
        current_frame_->addText(msg, node_position.x, node_position.y, node_position.z, false, 0.015, glm::vec3(1.0, 1.0, 0.0));
    }
}
