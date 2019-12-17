#ifndef INSIGHT_SCENARIO_SCENARIOCOLLECTION_H
#define INSIGHT_SCENARIO_SCENARIOCOLLECTION_H

#include <vector>

#include "Scenario.h"
#include "core/WindowManager.h"

class ScenarioCollection
{
public:
    ScenarioCollection();

    void initialise(WindowManager* window_manager);
    void addScenario(Scenario* scenario);
    void nextScenario();

private:
    std::vector<Scenario*> scenarios_;
    unsigned int current_scenario_;

    WindowManager* window_manager_;
};

#endif //INSIGHT_SCENARIO_SCENARIOCOLLECTION_H
