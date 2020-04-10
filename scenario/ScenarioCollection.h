#ifndef INSIGHT_SCENARIO_SCENARIOCOLLECTION_H
#define INSIGHT_SCENARIO_SCENARIOCOLLECTION_H

#include <vector>

class WindowManager;

namespace insight {
namespace scenario {

class Scenario;

class ScenarioCollection {
public:
    ScenarioCollection();
    ~ScenarioCollection();

    void initialise(WindowManager *window_manager);

    void addScenario(Scenario *scenario);

    unsigned long getScenarioCount();

    Scenario *getScenario(unsigned long scenario_index);
    Scenario *getCurrentScenario();

    void nextScenario(bool reset_camera = true);
    void previousScenario(bool reset_camera = true);
    void selectScenario(size_t scenario, bool reset_camera = true);

private:
    std::vector<Scenario *> scenarios_;
    unsigned long current_scenario_;

    WindowManager *window_manager_;
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_SCENARIOCOLLECTION_H
