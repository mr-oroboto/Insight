#include "ScenarioCollection.h"

#include "scenario/Scenario.h"
#include "core/WindowManager.h"

namespace insight {
namespace scenario {
    

ScenarioCollection::ScenarioCollection()
{
    window_manager_ = nullptr;
    current_scenario_ = 0;
    pushed_input_handler_ = false;
}

ScenarioCollection::~ScenarioCollection()
{
    for (Scenario* s : scenarios_)
    {
        delete s;
    }

    // TODO: Work out destruction ordering, this is getting called *after* WindowManager is destroyed
    window_manager_->popInputHandler();
}

void ScenarioCollection::initialise(WindowManager* window_manager)
{
    window_manager_ = window_manager;

    // Register ourselves to handle the next scenario keystroke.
    window_manager_->pushInputHandler(this);
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
        window_manager_->getDisplayManager()->resetCamera();
    }

    window_manager_->getDisplayManager()->setUpdateSceneCallback(nullptr);

    if (pushed_input_handler_)
    {
        window_manager_->popInputHandler();
    }

    window_manager_->pushInputHandler(scenarios_[scenario]);
    pushed_input_handler_ = true;

    scenarios_[scenario]->run();
}

Scenario* ScenarioCollection::getCurrentScenario()
{
    if (scenarios_.size() == 0)
    {
        return nullptr;
    }

    return scenarios_[current_scenario_];
}

void ScenarioCollection::handleKeystroke(insight::WindowManager *window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop)
{
    if (keystroke_event.type == SDL_KEYDOWN)
    {
        if (keystroke_event.key.keysym.sym == SDLK_n)
        {
            nextScenario();
        }
    }
}

void ScenarioCollection::handleMouse(insight::WindowManager *window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop)
{
}


}   // namespace scenario
}   // namespace insight