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
    FrameQueue* frame_queue = new FrameQueue(display_manager_, true);
    frame_queue->setFrameRate(1);

    HeapDecorator* decorator = new HeapDecorator(frame_queue);
    MinHeapArray heap(decorator);

    int heap_values[] = {
            4, 50, 7, 90, 55, 87, 2, 16
    };

    for (size_t i = 0; i < sizeof(heap_values) / sizeof(int); i++)
    {
        heap.insert(heap_values[i]);
    }

    heap.validate();

    frame_queue->setReady();
    frame_queue->setActive();    // transfer ownership to DisplayManager

    delete decorator;
}