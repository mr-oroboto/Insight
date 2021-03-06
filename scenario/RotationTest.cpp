#include "RotationTest.h"

#include "core/DisplayManager.h"

namespace insight {
namespace scenario {

void RotationTest::run()
{
    std::unique_ptr<FrameQueue> frame_queue = std::make_unique<FrameQueue>(display_manager_, true);
    frame_queue->setFrameRate(1);

    std::shared_ptr<Frame> current_frame = frame_queue->newFrame();

    current_frame->addObject(primitive::Primitive::Type::RECTANGLE, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    frame_queue->enqueueFrame(current_frame);

    frame_queue->setReady();
    if (frame_queue->setActive())
    {
        // transfer ownership to DisplayManager
        display_manager_->setFrameQueue(std::move(frame_queue));
    }
}

}   // namespace scenario
}   // namespace insight