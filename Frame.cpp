#include "Frame.h"
#include <iostream>

Frame::Frame(DisplayManager* dm)
{
    displayManager = dm;
}

Frame::~Frame()
{
    std::cout << "Frame::~Frame()" << std::endl;

    // Delete all the scene objects that belong to this frame.
    for (SceneObject* object : objects)
    {
        delete object;
    }
}

void Frame::addObject(Primitive::Type type, glm::vec3 worldPosition, glm::vec3 colour)
{
    SceneObject* object = new SceneObject(displayManager, type, worldPosition, colour);
    objects.push_back(object);
}

void Frame::addText(std::string text, GLuint x, GLuint y, GLfloat scale, glm::vec3 colour)
{
    Text textObj {
            x,
            y,
            text,
            scale,
            colour
    };

    texts.push_back(textObj);
}

void Frame::draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame)
{
    for (SceneObject* object : objects)
    {
        object->draw(secsSinceStart, secsSinceLastFrame);
    }

    for (Text text : texts)
    {
        displayManager->drawText(text.text, text.x, text.y, 0, false, text.scale, text.colour);
    }
}