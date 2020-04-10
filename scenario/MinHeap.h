#ifndef INSIGHT_SCENARIO_MINHEAP_H
#define INSIGHT_SCENARIO_MINHEAP_H

#include "Scenario.h"
#include "core/DisplayManager.h"

class MinHeap : public Scenario
{
public:
    MinHeap(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();
};


#endif //INSIGHT_SCENARIO_MINHEAP_H
