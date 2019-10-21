#include "SceneObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float
#include <iostream>

SceneObject::SceneObject(DisplayManager* dm, Primitive::Type type, glm::vec3 worldPos, glm::vec3 col)
{
    displayManager = dm;

    primitive = displayManager->getPrimitiveCollection()->selectPrimitive(type);
    worldPosition = worldPos;
    colour = col;
}

SceneObject::~SceneObject()
{
    std::cout << "SceneObject::~SceneObject()" << std::endl;
}

void SceneObject::draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame, bool useColour)
{
    glm::mat4 modelTransform = glm::mat4(1.0f);      // identity matrix

    // Multiply the identity matrix by a rotation transform matrix (see open.gl/transformations) of 180 deg around z-axis
    modelTransform = glm::translate(modelTransform, worldPosition);
//    modelTransform = glm::rotate(modelTransform, secsSinceStart * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(displayManager->getModelTransformUniform(), 1 /* number of matrices to upload */, GL_FALSE, glm::value_ptr(modelTransform));

    if (useColour)
    {
        glUniform1i(displayManager->getModelDoOverrideColourUniform(), 1);
        glUniform3f(displayManager->getModelOverrideColourUniform(), colour.r, colour.g, colour.b);
    }
    else
    {
        glUniform1i(displayManager->getModelDoOverrideColourUniform(), 0);
    }

    primitive->draw();
}