#include "SceneObject.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float

#include "DisplayManager.h"
#include "primitive/Line.h"

SceneObject::SceneObject(DisplayManager* display_manager, Primitive::Type type, glm::vec3 world_coords, glm::vec3 col)
{
    display_manager_ = display_manager;

    primitive_ = display_manager_->getPrimitiveCollection()->selectPrimitive(type);

    world_coords_ = world_coords;
    additional_world_coords_ = world_coords;
    scale_ = 1.0f;

    colour_ = col;
}

SceneObject::~SceneObject()
{
//  std::cout << "SceneObject::~SceneObject()" << std::endl;
}

void SceneObject::setScale(GLfloat s)
{
    scale_ = s;
}

void SceneObject::setAdditionalCoords(glm::vec3 world_coords)
{
    additional_world_coords_ = world_coords;
}

void SceneObject::draw(GLfloat secs_since_start, GLfloat secs_since_last_frame, bool use_colour)
{
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model_transform = glm::mat4(1.0f);      // identity matrix
    glm::vec3 scale_vector = glm::vec3(scale_, scale_, scale_);

    // Multiply the identity matrix by various transformations to translate, scale and rotate the primitive

    if (primitive_->getType() == Primitive::LINE)
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

        dynamic_cast<Line*>(primitive_)->setCoords(world_coords_, additional_world_coords_);
    }
    else
    {
        model_transform = glm::translate(model_transform, world_coords_);
//  modelTransform = glm::rotate(modelTransform, secsSinceStart * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    model_transform = glm::scale(model_transform, scale_vector);

    glUniformMatrix4fv(display_manager_->getModelTransformUniform(), 1 /* number of matrices to upload */, GL_FALSE, glm::value_ptr(model_transform));

    if (use_colour)
    {
        glUniform1i(display_manager_->getModelDoOverrideColourUniform(), 1);
        glUniform3f(display_manager_->getModelOverrideColourUniform(), colour_.r, colour_.g, colour_.b);
    }
    else
    {
        glUniform1i(display_manager_->getModelDoOverrideColourUniform(), 0);
    }

    primitive_->draw();

    glDisable(GL_DEPTH_TEST);
}

glm::vec3 SceneObject::getPosition()
{
    return world_coords_;
}

Primitive* SceneObject::getPrimitive()
{
    return primitive_;
}