#ifndef INSIGHT_SCENEOBJECT_H
#define INSIGHT_SCENEOBJECT_H

#include <glm/glm.hpp>
#include "DisplayManager.h"
#include "primitive/Primitive.h"

class SceneObject
{
public:
    SceneObject(DisplayManager* dm, Primitive::Type type, glm::vec3 worldPos);
    ~SceneObject();

    void draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame);

protected:
    DisplayManager* displayManager;
    Primitive* primitive;
    glm::vec3 worldPosition;
};


#endif //INSIGHT_SCENEOBJECT_H
