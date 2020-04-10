#ifndef INSIGHT_HEAPBREADTHFIRSTTRAVERSAL_H
#define INSIGHT_HEAPBREADTHFIRSTTRAVERSAL_H

#include "Scenario.h"

class DisplayManager;

namespace insight {
namespace scenario {

class HeapBreadthFirstTraversal : public Scenario {
public:
    HeapBreadthFirstTraversal(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_HEAPBREADTHFIRSTTRAVERSAL_H
