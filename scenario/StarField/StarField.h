#ifndef INSIGHT_SCENARIO_STARFIELD_H
#define INSIGHT_SCENARIO_STARFIELD_H

#include "scenario/Scenario.h"
#include "core/DisplayManager.h"

class StarField : public Scenario
{
public:
    StarField(DisplayManager* display_manager) : Scenario(display_manager) {}
    ~StarField();

    void run();

private:
    void updateSceneCallback(GLfloat secs_since_last_frame);

    Frame* frame_;
};


#endif //INSIGHT_SCENARIO_STARFIELD_H
