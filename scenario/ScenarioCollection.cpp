#include "ScenarioCollection.h"

#include "scenario/AutoPilot/AutoPilot.h"
#include "scenario/StarField/StarField.h"
#include "scenario/HeapDepthFirstTraversal.h"
#include "scenario/HeapBreadthFirstTraversal.h"
#include "scenario/MinHeap.h"

ScenarioCollection::ScenarioCollection()
{
    window_manager_ = nullptr;
    current_scenario_ = 0;
}

void ScenarioCollection::initialise(WindowManager* window_manager)
{
    window_manager_ = window_manager;

    AutoPilot* autopilot = new AutoPilot(window_manager_->getDisplayManager());
    addScenario(autopilot);

    MinHeap* minHeap = new MinHeap(window_manager_->getDisplayManager());
    addScenario(minHeap);

    HeapDepthFirstTraversal* dft = new HeapDepthFirstTraversal(window_manager_->getDisplayManager());
    addScenario(dft);

    HeapBreadthFirstTraversal* bft = new HeapBreadthFirstTraversal(window_manager_->getDisplayManager());
    addScenario(bft);

    StarField* starfield = new StarField(window_manager_->getDisplayManager());
    addScenario(starfield);
}

void ScenarioCollection::addScenario(Scenario* scenario)
{
    scenarios_.push_back(scenario);
}

void ScenarioCollection::nextScenario()
{
    if (++current_scenario_ > scenarios_.size() - 1)
    {
        current_scenario_ = 0;
    }

    window_manager_->resetCamera();
    scenarios_[current_scenario_]->run();
}