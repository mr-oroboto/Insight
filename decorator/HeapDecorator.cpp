#include "HeapDecorator.h"
#include "../adt/HeapArray.h"

void HeapDecorator::setHeap(HeapArray* heapArray)
{
    heap = heapArray;
}

void HeapDecorator::setHightlightedNodeValue(int nodeValue)
{
    highlightedNodeValue = nodeValue;
}

bool HeapDecorator::decorate()
{
    bool success = true;

    currentFrame = frameQueue->newFrame();

    glm::vec3 lastNodePosition = glm::vec3(0, 0, 5);
    dft(0, lastNodePosition, false);

    char msg[64];
    sprintf(msg, "%d", highlightedNodeValue);
    currentFrame->addText(msg, 10, 10);

    frameQueue->enqueueFrame(currentFrame);

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
    glm::vec3 nodeColour;
    if (nodeValue == highlightedNodeValue)
    {
        nodeColour = glm::vec3(1, 0, 0);
    }
    else
    {
        nodeColour = glm::vec3(0.8, 0.8, 0.8);
    }

    currentFrame->addObject(Primitive::Type::CUBE, nodePosition, nodeColour);

    if (rightIndex >= 0)
    {
        dft(rightIndex, nodePosition, false);
    }
}

