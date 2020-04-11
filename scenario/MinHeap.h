#ifndef INSIGHT_SCENARIO_MINHEAP_H
#define INSIGHT_SCENARIO_MINHEAP_H

#include "Scenario.h"

namespace insight {

class DisplayManager;

namespace scenario {


class MinHeap : public Scenario {
public:
    MinHeap(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_MINHEAP_H
