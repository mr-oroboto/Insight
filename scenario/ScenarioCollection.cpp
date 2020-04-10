#include "ScenarioCollection.h"

#include "scenario/Scenario.h"
#include "core/WindowManager.h"

insight::scenario::ScenarioCollection::ScenarioCollection()
{
    window_manager_ = nullptr;
    current_scenario_ = 0;
}

insight::scenario::ScenarioCollection::~ScenarioCollection()
{
    for (Scenario* s : scenarios_)
    {
        delete s;
    }
}

void insight::scenario::ScenarioCollection::initialise(WindowManager* window_manager)
{
    window_manager_ = window_manager;
}

void insight::scenario::ScenarioCollection::addScenario(Scenario* scenario)
{
    scenarios_.push_back(scenario);
}

insight::scenario::Scenario* insight::scenario::ScenarioCollection::getScenario(unsigned long scenario_index)
{
    if (scenario_index < scenarios_.size())
    {
        return scenarios_[scenario_index];
    }

    return nullptr;
}

unsigned long insight::scenario::ScenarioCollection::getScenarioCount()
{
    return scenarios_.size();
}

void insight::scenario::ScenarioCollection::nextScenario(bool reset_camera)
{
    if (++current_scenario_ > scenarios_.size() - 1)
    {
        current_scenario_ = 0;
    }

    selectScenario(current_scenario_, reset_camera);
}

void insight::scenario::ScenarioCollection::previousScenario(bool reset_camera)
{
    if (--current_scenario_ > scenarios_.size() - 1)
    {
        current_scenario_ = scenarios_.size() - 1;
    }

    selectScenario(current_scenario_, reset_camera);
}

void insight::scenario::ScenarioCollection::selectScenario(size_t scenario, bool reset_camera)
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

insight::scenario::Scenario* insight::scenario::ScenarioCollection::getCurrentScenario()
{
    if (scenarios_.size() == 0)
    {
        return nullptr;
    }

    return scenarios_[current_scenario_];
}