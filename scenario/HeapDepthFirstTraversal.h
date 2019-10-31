#ifndef INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H
#define INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H

#include "Scenario.h"

class HeapDepthFirstTraversal : public Scenario
{
public:
    HeapDepthFirstTraversal(DisplayManager* display_manager) : Scenario(display_manager) {}
    ~HeapDepthFirstTraversal();

    void run();
};


#endif //INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H
