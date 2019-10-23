#include "Frame.h"
#include <iostream>

Frame::Frame(DisplayManager* dm, bool drawObjectPos)
{
    displayManager = dm;
    drawObjectPositions = drawObjectPos;
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

void Frame::addText(std::string text, GLfloat x, GLfloat y, GLfloat z, bool ortho, GLfloat scale, glm::vec3 colour)
{
    glm::vec3 position = glm::vec3(x, y, z);
    Text textObj {
            text,
            position,
            ortho,
            scale,
            colour
    };

    texts.push_back(textObj);
}

void Frame::draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame)
{
    // We must first render all objects before we render any text
    for (SceneObject* object : objects)
    {
        object->draw(secsSinceStart, secsSinceLastFrame);
    }

    // Now text can be rendered
    if (drawObjectPositions)
    {
        for (SceneObject* object : objects)
        {
            char msg[64];
            glm::vec3 textColour = glm::vec3(1.0, 1.0, 0.0);
            glm::vec3 objectPosition = object->getPosition();
            snprintf(msg, sizeof(msg), "(%.1f,%.1f,%.1f)", objectPosition.x, objectPosition.y, objectPosition.z);
            displayManager->drawText(msg, objectPosition, false, 0.015, textColour);
        }
    }

    for (Text text : texts)
    {
        displayManager->drawText(text.text, text.position, text.ortho, text.scale, text.colour);
    }
}