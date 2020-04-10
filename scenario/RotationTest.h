#ifndef INSIGHT_SCENARIO_ROTATIONTEST_H
#define INSIGHT_SCENARIO_ROTATIONTEST_H

#include "Scenario.h"

class DisplayManager;

namespace insight {
namespace scenario {

class RotationTest : public Scenario {
public:
    RotationTest(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_ROTATIONTEST_H
