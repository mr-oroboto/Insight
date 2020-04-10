#ifndef INSIGHT_SCENARIO_ROTATIONTEST_H
#define INSIGHT_SCENARIO_ROTATIONTEST_H

#include "Scenario.h"
#include "core/DisplayManager.h"

class RotationTest : public Scenario
{
public:
    RotationTest(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();
};

#endif //INSIGHT_SCENARIO_ROTATIONTEST_H
