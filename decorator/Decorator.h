#ifndef INSIGHT_DECORATOR_DECORATOR_H
#define INSIGHT_DECORATOR_DECORATOR_H

#include "core/FrameQueue.h"
#include "core/Frame.h"

namespace Decorators {

    class Decorator {
    public:
        Decorator(FrameQueue *queue) : frame_queue_(queue) { current_frame_ = nullptr; }
        virtual ~Decorator();

        virtual void decorate() = 0;

    protected:
        FrameQueue* frame_queue_;
        std::shared_ptr<Frame> current_frame_;
    };

}

#endif //INSIGHT_DECORATOR_DECORATOR_H
