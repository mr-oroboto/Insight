#ifndef INSIGHT_CORE_SCENEOBJECT_H
#define INSIGHT_CORE_SCENEOBJECT_H

#include <string>

#include <glm/glm.hpp>

#include "primitive/Primitive.h"

class DisplayManager;
class Texture;

class SceneObject
{
public:
    SceneObject(DisplayManager* display_manager, insight::primitive::Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour);
    virtual ~SceneObject() = default;

    void setScale(GLfloat scale);
    void setScale(GLfloat x, GLfloat y, GLfloat z);
    void setAdditionalCoords(const glm::vec3& world_coords);
    void setTexture(const std::string& texture_name);
    void setColour(const glm::vec3& colour);

    glm::vec3 getPosition();
    glm::vec3 getScale();
    insight::primitive::Primitive* getPrimitive();

    virtual void draw(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, bool use_colour = true);
    virtual void update(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, void* context);

    SceneObject* clone();       // prefer to copy ctor so subclasses of Frame keep control

protected:
    DisplayManager* display_manager_;

    insight::primitive::Primitive* primitive_;
    Texture* texture_;

    glm::vec3 world_coords_;
    glm::vec3 additional_world_coords_;
    GLfloat scale_x_;
    GLfloat scale_y_;
    GLfloat scale_z_;
    glm::vec3 colour_;
};

#endif //INSIGHT_CORE_SCENEOBJECT_H
