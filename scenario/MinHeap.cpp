#include "MinHeap.h"
#include "../adt/HeapArray.h"
#include "../decorator/HeapDecorator.h"
#include "../FrameQueue.h"

MinHeap::~MinHeap()
{

}

void MinHeap::run()
{
    MinHeapArray minHeap;
    HeapDecorator decorator(&minHeap);

    Frame* frame = nullptr;
    FrameQueue* frameQueue = new FrameQueue(displayManager, true);
    frameQueue->setFrameRate(1);

    frame = frameQueue->newFrame();
    minHeap.insert(4);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    frame = frameQueue->newFrame();
    minHeap.insert(50);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    frame = frameQueue->newFrame();
    minHeap.insert(7);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    frame = frameQueue->newFrame();
    minHeap.insert(90);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    frame = frameQueue->newFrame();
    minHeap.insert(55);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    frame = frameQueue->newFrame();
    minHeap.insert(87);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    frame = frameQueue->newFrame();
    minHeap.insert(2);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    frame = frameQueue->newFrame();
    minHeap.insert(16);
    decorator.decorate(frame);
    frameQueue->enqueueFrame(frame);

    minHeap.validate();

    frameQueue->setReady();
    frameQueue->setActive();
}