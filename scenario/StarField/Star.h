#ifndef INSIGHT_SCENARIO_STARFIELD_STAR_H
#define INSIGHT_SCENARIO_STARFIELD_STAR_H

#include "core/SceneObject.h"

class DisplayManager;

namespace insight {
namespace scenario {

class Star : public SceneObject {
public:
    Star(DisplayManager* display_manager, primitive::Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour);
    ~Star() = default;

    enum Colour
    {
        WHITE = 0,
        RED = 1,
        BLUE = 2
    };

    void setNearAndFarPlane(GLfloat near, GLfloat far);
    void setForwardUnitsPerSec(GLfloat forward_units_per_sec);
    void setStarColour(Star::Colour colour);

    virtual void update(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, void* context);

private:
    Colour star_colour_;

    GLfloat forward_units_per_sec_;
    GLfloat fov_near_z_;                // front of screen (where stars disappear)
    GLfloat fov_far_z_;                 // rear of screen (where stars appear)
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_STARFIELD_STAR_H
