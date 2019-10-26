#include "SceneObject.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float

#include "DisplayManager.h"
#include "primitive/Line.h"

SceneObject::SceneObject(DisplayManager* dm, Primitive::Type type, glm::vec3 worldPos, glm::vec3 col)
{
    displayManager = dm;

    primitive = displayManager->getPrimitiveCollection()->selectPrimitive(type);

    worldPosition = worldPos;
    additionalWorldPosition = worldPos;
    scale = 1.0f;

    colour = col;
}

SceneObject::~SceneObject()
{
    std::cout << "SceneObject::~SceneObject()" << std::endl;
}

void SceneObject::setScale(GLfloat s)
{
    scale = s;
}

void SceneObject::setAdditionalCoords(glm::vec3 coords)
{
    additionalWorldPosition = coords;
}

void SceneObject::draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame, bool useColour)
{
    glEnable(GL_DEPTH_TEST);

    glm::mat4 modelTransform = glm::mat4(1.0f);      // identity matrix
    glm::vec3 scaleVector = glm::vec3(scale, scale, scale);

    // Multiply the identity matrix by various transformations to translate, scale and rotate the primitive

    if (primitive->getType() == Primitive::LINE)
    {
        // Get new unit vector, then translate to new "from" position, then scale

//        glm::vec3 vector = (additionalPosition - worldPosition);
//        glm::vec3 vectorNormalised = glm::normalize(vector);
//
//        // rotate vector parallel to
//        GLfloat rad = acos(vectorNormalised.x);
//        modelTransform = glm::rotate(modelTransform, rad, glm::vec3(0.0f, 0.0f, 1.0f));
//
//        glm::vec4 b = glm::vec4(vectorNormalised.x, vectorNormalised.y, vectorNormalised.z, 0);
//        glm::vec4 m = modelTransform * b;
//
//        glm::vec3 c = glm::vec3(m);
//        glm::vec3 norm = glm::cross(glm::vec3(0, 0, -1), c);
//
//        // rotate vector parallel to xz plane
//        rad = acos(vectorNormalised.z);
//        modelTransform = glm::rotate(modelTransform, rad, norm);
//
//        modelTransform = glm::translate(modelTransform, worldPosition);

        dynamic_cast<Line*>(primitive)->setCoords(worldPosition, additionalWorldPosition);
    }
    else
    {
        modelTransform = glm::translate(modelTransform, worldPosition);
//  modelTransform = glm::rotate(modelTransform, secsSinceStart * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    modelTransform = glm::scale(modelTransform, scaleVector);

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

    glDisable(GL_DEPTH_TEST);
}

glm::vec3 SceneObject::getPosition()
{
    return worldPosition;
}

Primitive* SceneObject::getPrimitive()
{
    return primitive;
}