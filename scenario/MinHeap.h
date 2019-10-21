#ifndef INSIGHT_MINHEAP_H
#define INSIGHT_MINHEAP_H

#include "Scenario.h"

class MinHeap : public Scenario
{
public:
    MinHeap(DisplayManager* dm) : Scenario(dm) {}
    ~MinHeap();

    void run();
};


#endif //INSIGHT_MINHEAP_H
