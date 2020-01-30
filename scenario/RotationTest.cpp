#include "RotationTest.h"

#include <iostream>

#include "core/FrameQueue.h"

RotationTest::~RotationTest()
{
}

void RotationTest::run()
{
    FrameQueue* frame_queue = new FrameQueue(display_manager_, true);
    frame_queue->setFrameRate(1);

    Frame* current_frame = frame_queue->newFrame();

    current_frame->addObject(Primitive::Type::RECTANGLE, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    frame_queue->enqueueFrame(current_frame);

    frame_queue->setReady();
    frame_queue->setActive();    // transfer ownership to DisplayManager
}