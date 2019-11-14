#ifndef INSIGHT_SCENARIO_STARFIELD_STAR_H
#define INSIGHT_SCENARIO_STARFIELD_STAR_H

#include "core/SceneObject.h"

class Star : public SceneObject
{
public:
    Star(DisplayManager* display_manager, Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour);
    ~Star();

    virtual void update(GLfloat secs_since_last_frame);
};


#endif //INSIGHT_SCENARIO_STARFIELD_STAR_H
