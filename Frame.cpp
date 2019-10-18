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

void Frame::addObject(Primitive::Type type, glm::vec3 worldPosition)
{
    SceneObject* object = new SceneObject(displayManager, type, worldPosition);
    objects.push_back(object);
}

void Frame::draw(float time)
{
    for (SceneObject* object : objects)
    {
        object->draw(time);
    }
}