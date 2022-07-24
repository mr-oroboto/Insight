#ifndef INSIGHT_SCENARIO_LORENTZ_H
#define INSIGHT_SCENARIO_LORENTZ_H

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Scenario.h"

namespace insight {
namespace scenario {

class Lorentz : public Scenario {
public:
    Lorentz(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();

    void handleKeystroke(insight::WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop);

private:
    void updateSceneCallback(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame);

    glm::vec3 solveDerivatives(const glm::vec3& current_coordinates);

    float sigma_ = 10;
    float beta_ = 8.0f / 3.0f;
    float rho_ = 28;
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_LORENTZ_H
