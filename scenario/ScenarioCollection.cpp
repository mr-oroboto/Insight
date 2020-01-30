#include "ScenarioCollection.h"

ScenarioCollection::ScenarioCollection()
{
    window_manager_ = nullptr;
    current_scenario_ = 0;
}

ScenarioCollection::~ScenarioCollection()
{
    for (Scenario* s : scenarios_)
    {
        delete s;
    }
}

void ScenarioCollection::initialise(WindowManager* window_manager)
{
    window_manager_ = window_manager;
}

void ScenarioCollection::addScenario(Scenario* scenario)
{
    scenarios_.push_back(scenario);
}

Scenario* ScenarioCollection::getScenario(unsigned long scenario_index)
{
    if (scenario_index < scenarios_.size())
    {
        return scenarios_[scenario_index];
    }

    return nullptr;
}

unsigned long ScenarioCollection::getScenarioCount()
{
    return scenarios_.size();
}

void ScenarioCollection::nextScenario(bool reset_camera)
{
    if (++current_scenario_ > scenarios_.size() - 1)
    {
        current_scenario_ = 0;
    }

    selectScenario(current_scenario_, reset_camera);
}

void ScenarioCollection::previousScenario(bool reset_camera)
{
    if (--current_scenario_ > scenarios_.size() - 1)
    {
        current_scenario_ = scenarios_.size() - 1;
    }

    selectScenario(current_scenario_, reset_camera);
}

void ScenarioCollection::selectScenario(size_t scenario, bool reset_camera)
{
    if (scenario >= scenarios_.size())
    {
        return;
    }

    if (reset_camera)
    {
        window_manager_->resetCamera();
    }

    window_manager_->getDisplayManager()->setUpdateSceneCallback(nullptr);
    scenarios_[scenario]->run();
}