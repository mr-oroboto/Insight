#include "HeapDecorator.h"
#include "../primitive/Primitive.h"
#include "../Frame.h"

bool HeapDecorator::decorate(Frame *frame)
{
    bool success = true;

    currentFrame = frame;

    glm::vec3 lastNodePosition = glm::vec3(0, 0, 5);
    dft(0, lastNodePosition, false);

    return success;
}

void HeapDecorator::dft(size_t nodeIndex, glm::vec3 lastNodePosition, bool isLeftOfParent)
{
    int leftIndex = heap->leftIndex(nodeIndex);
    int rightIndex = heap->rightIndex(nodeIndex);

    // Where should this node be drawn?
    glm::vec3 nodePosition = lastNodePosition;
    if (nodeIndex != 0)
    {
        if (isLeftOfParent)
        {
            nodePosition.x -= 2.0;
            nodePosition.z -= 1.5;
        }
        else
        {
            nodePosition.x += 2.0;
            nodePosition.z -= 1.5;
        }
    }

    if (leftIndex >= 0)
    {
        dft(leftIndex, nodePosition, true);
    }

    // Draw the node
    int nodeValue = heap->nodeValue(nodeIndex);
    currentFrame->addObject(Primitive::Type::CUBE, nodePosition);

    if (rightIndex >= 0)
    {
        dft(rightIndex, nodePosition, false);
    }
}

