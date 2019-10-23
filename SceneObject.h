#ifndef INSIGHT_SCENEOBJECT_H
#define INSIGHT_SCENEOBJECT_H

#include <glm/glm.hpp>
#include "DisplayManager.h"
#include "primitive/Primitive.h"

class SceneObject
{
public:
    SceneObject(DisplayManager* dm, Primitive::Type type, glm::vec3 worldPos, glm::vec3 col);
    ~SceneObject();

    void draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame, bool useColour = true);
    glm::vec3 getPosition();

protected:
    DisplayManager* displayManager;
    Primitive* primitive;
    glm::vec3 worldPosition;
    glm::vec3 colour;
};


#endif //INSIGHT_SCENEOBJECT_H
