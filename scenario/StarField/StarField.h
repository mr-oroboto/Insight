#ifndef INSIGHT_SCENARIO_STARFIELD_H
#define INSIGHT_SCENARIO_STARFIELD_H

#include <memory>

#include <GL/glew.h>

#include "scenario/Scenario.h"

namespace insight {

class DisplayManager;
class Frame;

namespace scenario {


class StarField : public Scenario {
public:
    StarField(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();

private:
    void updateSceneCallback(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame);

    std::shared_ptr<Frame> frame_;
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_STARFIELD_H
