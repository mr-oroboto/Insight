#ifndef INSIGHT_FRAME_H
#define INSIGHT_FRAME_H

#include "DisplayManager.h"
#include "SceneObject.h"
#include "primitive/Primitive.h"
#include <glm/glm.hpp>
#include <vector>

class Frame
{
public:
    Frame(DisplayManager* dm);
    ~Frame();

    void addObject(Primitive::Type type, glm::vec3 worldPosition, glm::vec3 colour);
    void draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame);

protected:
    DisplayManager* displayManager;
    std::vector<SceneObject*> objects;
};

#endif //INSIGHT_FRAME_H
