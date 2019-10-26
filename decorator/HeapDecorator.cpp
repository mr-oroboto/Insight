#include "HeapDecorator.h"

#include <math.h>

#include "adt/HeapArray.h"

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

    unsigned long nodeCount = heap->nodeCount();
    unsigned long heapLevels = floor(log(nodeCount) / log(2.0));

    char msg[64];
    sprintf(msg, "current value: %d, levels: %lu", highlightedNodeValue, heapLevels);
    currentFrame->addText(msg, 10, 10);

    dft(0, heapLevels + 1, lastNodePosition, false);

    frameQueue->enqueueFrame(currentFrame);

    return success;
}

void HeapDecorator::dft(size_t nodeIndex, unsigned long inverseNodeLevel, glm::vec3 lastNodePosition, bool isLeftOfParent)
{
    int leftIndex = heap->leftIndex(nodeIndex);
    int rightIndex = heap->rightIndex(nodeIndex);

    // Where should this node be drawn?
    glm::vec3 nodePosition = lastNodePosition;
    if (nodeIndex != 0)
    {
        if (isLeftOfParent)
        {
            nodePosition.x += (inverseNodeLevel * 2.0);
            nodePosition.z -= 1.5;
        }
        else
        {
            nodePosition.x -= (inverseNodeLevel * 2.0);
            nodePosition.z -= 1.5;
        }
    }

    if (leftIndex >= 0)
    {
        dft(leftIndex, inverseNodeLevel - 1, nodePosition, true);
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

    if (nodeIndex != 0)
    {
        currentFrame->addLine(lastNodePosition, nodePosition, glm::vec3(1, 1, 1));
    }

    char msg[64];
    glm::vec3 textColour = glm::vec3(1.0, 1.0, 0.0);
    sprintf(msg, "%d", nodeValue);
    currentFrame->addText(msg, nodePosition.x, nodePosition.y, nodePosition.z, false, 0.015, textColour);

    if (rightIndex >= 0)
    {
        dft(rightIndex, inverseNodeLevel - 1, nodePosition, false);
    }
}

