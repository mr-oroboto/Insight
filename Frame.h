#ifndef INSIGHT_FRAME_H
#define INSIGHT_FRAME_H

#include "DisplayManager.h"
#include "SceneObject.h"
#include "primitives/Primitive.h"
#include <glm/glm.hpp>
#include <vector>

class Frame
{
public:
    Frame(DisplayManager* dm);
    ~Frame();

    void addObject(Primitive::Type type, glm::vec3 worldPosition);
    void draw(float time);

protected:
    DisplayManager* displayManager;
    std::vector<SceneObject*> objects;
};

#endif //INSIGHT_FRAME_H
