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
    Frame(DisplayManager* dm);
    ~Frame();

    void addObject(Primitive::Type type, glm::vec3 worldPosition, glm::vec3 colour);
    void addText(std::string text, GLuint x, GLuint y, GLfloat scale = 1.0f, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));
    void draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame);

protected:
    struct Text
    {
        GLuint x;
        GLuint y;
        std::string text;
        GLfloat scale;
        glm::vec3 colour;
    };

    DisplayManager* displayManager;
    std::vector<SceneObject*> objects;
    std::vector<Text> texts;
};

#endif //INSIGHT_FRAME_H
