#ifndef INSIGHT_SCENARIO_SCENARIO_H
#define INSIGHT_SCENARIO_SCENARIO_H

#include "core/DisplayManager.h"

class Scenario
{
public:
    Scenario(DisplayManager* display_manager) : display_manager_(display_manager) {}
    virtual ~Scenario();

    virtual void run() = 0;

protected:
    DisplayManager* display_manager_;
};


#endif //INSIGHT_SCENARIO_SCENARIO_H
