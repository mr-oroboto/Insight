#include "MinHeap.h"

#include <iostream>

#include "adt/HeapArray.h"
#include "core/DisplayManager.h"
#include "decorator/HeapDecorator.h"

void insight::scenario::MinHeap::run()
{
    display_manager_->setUpdateSceneCallback(nullptr);

    std::unique_ptr<FrameQueue> frame_queue = std::make_unique<FrameQueue>(display_manager_, true);
    frame_queue->setFrameRate(1);

    Decorators::HeapDecorator* decorator = new Decorators::HeapDecorator(frame_queue.get());
    MinHeapArray heap(decorator);

    int heap_values[] = {
            4, 50, 7, 90, 55, 87, 2, 16, 23
    };

    for (size_t i = 0; i < sizeof(heap_values) / sizeof(int); i++)
    {
        heap.insert(heap_values[i]);
    }

    heap.validate();

    frame_queue->setReady();
    if (frame_queue->setActive())
    {
        // transfer ownership to DisplayManager
        display_manager_->setFrameQueue(std::move(frame_queue));
    }

    delete decorator;
}