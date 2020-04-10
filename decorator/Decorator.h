#ifndef INSIGHT_DECORATOR_DECORATOR_H
#define INSIGHT_DECORATOR_DECORATOR_H

#include "core/FrameQueue.h"
#include "core/Frame.h"

namespace insight {
namespace decorator {

class Decorator {
public:
    Decorator(FrameQueue *queue) : frame_queue_(queue) { current_frame_ = nullptr; }
    virtual ~Decorator() = default;

    virtual void decorate() = 0;

protected:
    FrameQueue* frame_queue_;
    std::shared_ptr<Frame> current_frame_;
};

}   // namespace decorator
}   // namespace insight

#endif //INSIGHT_DECORATOR_DECORATOR_H
