#include "HeapDepthFirstTraversal.h"

#include <iostream>

#include "adt/HeapArray.h"
#include "decorator/HeapDecorator.h"
#include "core/FrameQueue.h"

void HeapDepthFirstTraversal::run()
{
    display_manager_->setUpdateSceneCallback(nullptr);

    run(Decorators::HeapDecorator::TraverseOrder::PRE_ORDER);
}

void HeapDepthFirstTraversal::run(Decorators::HeapDecorator::TraverseOrder traverse_order)
{
    std::unique_ptr<FrameQueue> frame_queue = std::make_unique<FrameQueue>(display_manager_, true);
    frame_queue->setFrameRate(1);

    Decorators::HeapDecorator* decorator = new Decorators::HeapDecorator(frame_queue.get());
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

    decorator->decorateDft(traverse_order);

    frame_queue->setReady();
    if (frame_queue->setActive())
    {
        // transfer ownership to DisplayManager
        display_manager_->setFrameQueue(std::move(frame_queue));
    }

    delete decorator;
}