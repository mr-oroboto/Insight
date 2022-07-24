#ifndef INSIGHT_SCENARIO_SCENARIOCOLLECTION_H
#define INSIGHT_SCENARIO_SCENARIOCOLLECTION_H

#include <vector>

#include "Insight.h"

namespace insight {

class WindowManager;

namespace scenario {

class Scenario;

class ScenarioCollection : public insight::InputHandler {
public:
    ScenarioCollection();
    ~ScenarioCollection();

    void initialise(WindowManager *window_manager);

    void addScenario(Scenario *scenario);

    unsigned long getScenarioCount();

    Scenario*   getScenario(unsigned long scenario_index);
    Scenario*   getCurrentScenario();

    void nextScenario(bool reset_camera = true);
    void previousScenario(bool reset_camera = true);
    void selectScenario(size_t scenario, bool reset_camera = true);

    // insight::InputHandler overrides
    void handleKeystroke(insight::WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop);
    void handleMouse(insight::WindowManager* window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop);

private:
    std::vector<Scenario*> scenarios_;
    unsigned long current_scenario_;

    WindowManager* window_manager_;

    bool pushed_input_handler_;
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_SCENARIOCOLLECTION_H
