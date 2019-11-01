#include "HeapBreadthFirstTraversal.h"

#include <iostream>

#include "adt/HeapArray.h"
#include "decorator/HeapDecorator.h"
#include "core/FrameQueue.h"

HeapBreadthFirstTraversal::~HeapBreadthFirstTraversal()
{
    std::cout << "HeapBreadthFirstTraversal::~HeapBreadthFirstTraversal()" << std::endl;
}

void HeapBreadthFirstTraversal::run()
{
    FrameQueue* frame_queue = new FrameQueue(display_manager_, true);
    frame_queue->setFrameRate(1);

    HeapDecorator* decorator = new HeapDecorator(frame_queue);
    MinHeapArray heap(nullptr);
    decorator->setHeap(dynamic_cast<HeapArray*>(&heap));

    int heap_values[] = {
            4, 50, 7, 90, 55, 87, 2, 16, 23
    };

    for (size_t i = 0; i < sizeof(heap_values) / sizeof(int); i++)
    {
        heap.insert(heap_values[i]);
    }

    heap.validate();

    decorator->decorateBft();

    frame_queue->setReady();
    frame_queue->setActive();    // transfer ownership to DisplayManager

    delete decorator;
}