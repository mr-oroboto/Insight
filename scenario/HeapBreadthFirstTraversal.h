#ifndef INSIGHT_HEAPBREADTHFIRSTTRAVERSAL_H
#define INSIGHT_HEAPBREADTHFIRSTTRAVERSAL_H

#include "Scenario.h"

class HeapBreadthFirstTraversal : public Scenario
{
public:
    HeapBreadthFirstTraversal(DisplayManager* display_manager) : Scenario(display_manager) {}
    ~HeapBreadthFirstTraversal();

    void run();
};

#endif //INSIGHT_HEAPBREADTHFIRSTTRAVERSAL_H
