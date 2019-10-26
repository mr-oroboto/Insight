#ifndef INSIGHT_SCENARIO_MINHEAP_H
#define INSIGHT_SCENARIO_MINHEAP_H

#include "Scenario.h"
#include "core/DisplayManager.h"

class MinHeap : public Scenario
{
public:
    MinHeap(DisplayManager* dm) : Scenario(dm) {}
    ~MinHeap();

    void run();
};


#endif //INSIGHT_SCENARIO_MINHEAP_H
