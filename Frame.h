#ifndef INSIGHT_FRAME_H
#define INSIGHT_FRAME_H

#include "DisplayManager.h"
#include "SceneObject.h"
#include "primitive/Primitive.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Frame
{
public:
    Frame(DisplayManager* dm, bool drawObjectPos = false);
    ~Frame();

    void addObject(Primitive::Type type, glm::vec3 worldPosition, glm::vec3 colour);
    void addText(std::string text, GLfloat x, GLfloat y, GLfloat z = 0.0f, bool ortho = true, GLfloat scale = 1.0f, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));
    void draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame);

protected:
    struct Text
    {
        std::string text;
        glm::vec3 position;
        bool ortho;
        GLfloat scale;
        glm::vec3 colour;
    };

    bool drawObjectPositions;

    DisplayManager* displayManager;
    std::vector<SceneObject*> objects;
    std::vector<Text> texts;
};

#endif //INSIGHT_FRAME_H
