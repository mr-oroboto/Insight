#ifndef INSIGHT_CORE_SCENEOBJECT_H
#define INSIGHT_CORE_SCENEOBJECT_H

#include <glm/glm.hpp>

#include "primitive/Primitive.h"

class DisplayManager;

class SceneObject
{
public:
    SceneObject(DisplayManager* dm, Primitive::Type type, glm::vec3 worldPos, glm::vec3 col);
    ~SceneObject();

    void setScale(GLfloat scale);
    void setAdditionalCoords(glm::vec3 coords);

    glm::vec3 getPosition();
    Primitive* getPrimitive();

    void draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame, bool useColour = true);

protected:
    DisplayManager* displayManager;

    Primitive* primitive;
    glm::vec3 worldPosition;
    glm::vec3 additionalWorldPosition;
    GLfloat scale;
    glm::vec3 colour;
};

#endif //INSIGHT_CORE_SCENEOBJECT_H
