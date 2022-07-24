#ifndef INSIGHT_SCENARIO_SCENARIO_H
#define INSIGHT_SCENARIO_SCENARIO_H

#include "core/InputHandler.h"

namespace insight {

class DisplayManager;

namespace scenario {


class Scenario : public insight::InputHandler {
public:
    Scenario(DisplayManager* display_manager) : display_manager_(display_manager) {}
    virtual ~Scenario() = default;

    virtual void run() = 0;

    // insight::InputHandler overrides
    void handleKeystroke(insight::WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop) {}
    void handleMouse(insight::WindowManager* window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop) {}

protected:
    DisplayManager* display_manager_;
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_SCENARIO_H
