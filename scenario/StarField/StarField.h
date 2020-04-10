#ifndef INSIGHT_SCENARIO_STARFIELD_H
#define INSIGHT_SCENARIO_STARFIELD_H

#include "scenario/Scenario.h"
#include "core/DisplayManager.h"

class StarField : public Scenario
{
public:
    StarField(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();

private:
    void updateSceneCallback(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame);

    Frame* frame_;
};


#endif //INSIGHT_SCENARIO_STARFIELD_H
