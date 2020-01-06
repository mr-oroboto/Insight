#ifndef INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H
#define INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H

#include "Scenario.h"
#include "decorator/HeapDecorator.h"

class HeapDepthFirstTraversal : public Scenario
{
public:
    HeapDepthFirstTraversal(DisplayManager* display_manager) : Scenario(display_manager) {}
    ~HeapDepthFirstTraversal();

    void run();
    void run(Decorators::HeapDecorator::TraverseOrder traverse_order);
};


#endif //INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H
