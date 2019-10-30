#include "SceneObject.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float

#include "DisplayManager.h"
#include "primitive/Line.h"

SceneObject::SceneObject(DisplayManager* display_manager, Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour)
{
    display_manager_ = display_manager;

    primitive_ = display_manager_->getPrimitiveCollection()->selectPrimitive(type);

    world_coords_ = world_coords;
    additional_world_coords_ = world_coords;
    scale_ = 1.0f;

    colour_ = colour;
}

SceneObject::~SceneObject()
{
//  std::cout << "SceneObject::~SceneObject()" << std::endl;
}

void SceneObject::setScale(GLfloat s)
{
    scale_ = s;
}

void SceneObject::setAdditionalCoords(const glm::vec3& world_coords)
{
    additional_world_coords_ = world_coords;
}

void SceneObject::draw(GLfloat secs_since_start, GLfloat secs_since_last_frame, bool use_colour)
{
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model_transform = glm::mat4(1.0f);      // identity matrix
    glm::vec3 scale_vector = glm::vec3(scale_, scale_, scale_);

    // Multiply the identity matrix by various transformations to translate, scale and rotate the primitive

    model_transform = glm::translate(model_transform, world_coords_);
    model_transform = glm::scale(model_transform, scale_vector);

    if (primitive_->getType() == Primitive::LINE)
    {
        Line* line = dynamic_cast<Line*>(primitive_);

        line->setCoords(world_coords_, additional_world_coords_);

        // order of operations is important
        model_transform = line->getTranslationTransform(glm::mat4(1.0f));
        model_transform = line->getRotationTransform(model_transform);
        model_transform = line->getScaleTransform(model_transform);
    }

    display_manager_->getObjectShader()->setModelTransform(model_transform);

    if (use_colour)
    {
        display_manager_->getObjectShader()->setOverrideModelColour(true, colour_);
    }
    else
    {
        display_manager_->getObjectShader()->setOverrideModelColour(false);
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