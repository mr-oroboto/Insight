#ifndef INSIGHT_CORE_SCENEOBJECT_H
#define INSIGHT_CORE_SCENEOBJECT_H

#include <glm/glm.hpp>

#include "primitive/Primitive.h"
#include "Texture.h"

class DisplayManager;

class SceneObject
{
public:
    SceneObject(DisplayManager* display_manager, Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour);
    virtual ~SceneObject();

    void setScale(GLfloat scale);
    void setAdditionalCoords(const glm::vec3& world_coords);
    void setTexture(Texture* texture);
    void setColour(const glm::vec3& colour);

    glm::vec3 getPosition();
    Primitive* getPrimitive();

    void draw(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, bool use_colour = true);
    virtual void update(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame);

    SceneObject* clone();       // prefer to copy ctor so subclasses of Frame keep control

protected:
    DisplayManager* display_manager_;

    Primitive* primitive_;
    Texture* texture_;

    glm::vec3 world_coords_;
    glm::vec3 additional_world_coords_;
    GLfloat scale_;
    glm::vec3 colour_;
};

#endif //INSIGHT_CORE_SCENEOBJECT_H
