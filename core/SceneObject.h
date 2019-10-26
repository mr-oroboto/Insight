#ifndef INSIGHT_CORE_SCENEOBJECT_H
#define INSIGHT_CORE_SCENEOBJECT_H

#include <glm/glm.hpp>

#include "primitive/Primitive.h"

class DisplayManager;

class SceneObject
{
public:
    SceneObject(DisplayManager* display_manager, Primitive::Type type, glm::vec3 world_coords, glm::vec3 col);
    ~SceneObject();

    void setScale(GLfloat scale);
    void setAdditionalCoords(glm::vec3 world_coords);

    glm::vec3 getPosition();
    Primitive* getPrimitive();

    void draw(GLfloat secs_since_start, GLfloat secs_since_last_frame, bool use_colour = true);

protected:
    DisplayManager* display_manager_;

    Primitive* primitive_;
    glm::vec3 world_coords_;
    glm::vec3 additional_world_coords_;
    GLfloat scale_;
    glm::vec3 colour_;
};

#endif //INSIGHT_CORE_SCENEOBJECT_H
