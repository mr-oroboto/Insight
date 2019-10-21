#ifndef INSIGHT_SCENARIO_H
#define INSIGHT_SCENARIO_H

#include "../DisplayManager.h"

class Scenario
{
public:
    Scenario(DisplayManager* dm) : displayManager(dm) {}
    virtual ~Scenario();

    virtual void run() = 0;

protected:
    DisplayManager* displayManager;

};


#endif //INSIGHT_SCENARIO_H
