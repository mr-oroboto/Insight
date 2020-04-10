#ifndef INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H
#define INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H

#include "Scenario.h"
#include "decorator/HeapDecorator.h"

namespace insight {
namespace scenario {

class HeapDepthFirstTraversal : public Scenario {
public:
    HeapDepthFirstTraversal(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();
    void run(Decorators::HeapDecorator::TraverseOrder traverse_order);
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_HEAPDEPTHFIRSTTRAVERSAL_H
