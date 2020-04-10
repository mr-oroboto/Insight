#ifndef INSIGHT_SCENARIO_AUTOPILOT_H
#define INSIGHT_SCENARIO_AUTOPILOT_H

#include "scenario/Scenario.h"
#include "core/DisplayManager.h"
#include "scenario/StarField/Star.h"

class AutoPilot : public Scenario
{
public:
    AutoPilot(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();

private:
    void setupStarField(GLuint num_stars, const glm::vec3& origin, GLfloat x_spread, GLfloat y_spread, GLfloat z_spread);

    void drawFloor(GLfloat floor_z_start, GLfloat floor_z_end);
    void updateSceneCallback(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame);

    Frame* frame_;

    GLfloat z_;
    GLfloat floor_drawn_from_z_;        // where is the floor drawn "from" (from here out toward the viewer)
    GLfloat initial_floor_length_z_;

    GLfloat camera_pitch_degrees_;
    GLfloat camera_yaw_degrees_;
    GLfloat camera_roll_degrees_;
    GLfloat camera_roll_increment_;
    GLfloat camera_roll_max_degrees_;

    std::vector<Star*> stars_;
};

#endif //INSIGHT_SCENARIO_AUTOPILOT_H
