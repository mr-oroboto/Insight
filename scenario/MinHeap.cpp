#include "MinHeap.h"

#include <iostream>

#include "adt/HeapArray.h"
#include "decorator/HeapDecorator.h"
#include "core/FrameQueue.h"

MinHeap::~MinHeap()
{
    std::cout << "MinHeap::~MinHeap()" << std::endl;
}

void MinHeap::run()
{
    FrameQueue* frameQueue = new FrameQueue(displayManager, false);
    frameQueue->setFrameRate(1);

    HeapDecorator* decorator = new HeapDecorator(frameQueue);
    MinHeapArray minHeap(decorator);

    int heapValues[] = {
            4, 50, 7, 90, 55, 87, 2, 16
    };

    for (size_t i = 0; i < sizeof(heapValues) / sizeof(int); i++)
    {
        int heapValue = heapValues[i];
        minHeap.insert(heapValue);
    }

    minHeap.validate();

    frameQueue->setReady();
    frameQueue->setActive();    // transfer ownership to DisplayManager

    delete decorator;
}