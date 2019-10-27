#include "HeapDecorator.h"

#include <math.h>

#include "adt/HeapArray.h"

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

void HeapDecorator::dft(size_t node_index, unsigned long node_level, glm::vec3 last_node_coords, bool is_left_of_parent)
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

    if (left_index >= 0)
    {
        dft(left_index, node_level - 1, node_position, true);
    }

    // Draw the node
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

    current_frame_->addObject(Primitive::Type::CUBE, node_position, node_colour, 1.5);

    if (node_index != 0)
    {
        current_frame_->addLine(last_node_coords, node_position, glm::vec3(1, 1, 1));
    }

    char msg[64];
    glm::vec3 text_colour = glm::vec3(1.0, 1.0, 0.0);
    sprintf(msg, "%d", node_value);
    current_frame_->addText(msg, node_position.x, node_position.y, node_position.z, false, 0.015, text_colour);

    if (right_index >= 0)
    {
        dft(right_index, node_level - 1, node_position, false);
    }
}

