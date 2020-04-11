#include "SceneObject.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float

#include "DisplayManager.h"
#include "primitive/Line.h"

namespace insight {


SceneObject::SceneObject(DisplayManager* display_manager, primitive::Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour)
    : display_manager_(display_manager),
      primitive_(display_manager->getPrimitiveCollection()->selectPrimitive(type)),
      texture_(nullptr),
      world_coords_(world_coords),
      additional_world_coords_(world_coords),
      scale_x_(1.0f),
      scale_y_(1.0f),
      scale_z_(1.0f),
      colour_(colour) {}

void SceneObject::setScale(GLfloat s)
{
    scale_x_ = scale_y_ = scale_z_ = s;
}

void SceneObject::setScale(GLfloat x, GLfloat y, GLfloat z)
{
    scale_x_ = x;
    scale_y_ = y;
    scale_z_ = z;
}

void SceneObject::setTexture(const std::string& texture_name)
{
    texture_ = display_manager_->getTexture(texture_name);
}

void SceneObject::setColour(const glm::vec3& colour)
{
    colour_ = colour;
}

void SceneObject::setAdditionalCoords(const glm::vec3& world_coords)
{
    additional_world_coords_ = world_coords;
}

void SceneObject::draw(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, bool use_colour)
{
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model_transform = glm::mat4(1.0f);      // identity matrix
    glm::vec3 scale_vector = glm::vec3(scale_x_, scale_y_, scale_z_);

    // Multiply the identity matrix by various transformations to translate, scale and rotate the primitive

    model_transform = glm::translate(model_transform, world_coords_);
    model_transform = glm::scale(model_transform, scale_vector);

    if (primitive_->getSupportsTransforms())
    {
        primitive_->setCoords(world_coords_, additional_world_coords_);

        // order of operations is important
        model_transform = primitive_->getTranslationTransform(glm::mat4(1.0f));
        model_transform = primitive_->getRotationTransform(model_transform);
        model_transform = primitive_->getScaleTransform(model_transform, scale_vector);
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

    if (texture_)
    {
        display_manager_->getObjectShader()->setTexturesOn(true);
        display_manager_->getObjectShader()->setTextureSamplerTextureUnit(0);
        texture_->use(0);
    }
    else
    {
        display_manager_->getObjectShader()->setTexturesOn(false);
    }

    primitive_->draw();

    glDisable(GL_DEPTH_TEST);
}

void SceneObject::update(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, void* context)
{

}

glm::vec3 SceneObject::getPosition()
{
    return world_coords_;
}

glm::vec3 SceneObject::getScale()
{
    return glm::vec3(scale_x_, scale_y_, scale_z_);
}

primitive::Primitive* SceneObject::getPrimitive()
{
    return primitive_;
}

SceneObject* SceneObject::clone()
{
    SceneObject* clone = new SceneObject(display_manager_, primitive_->getType(), world_coords_, colour_);

    clone->texture_ = texture_;
    clone->setAdditionalCoords(additional_world_coords_);
    clone->setScale(scale_x_, scale_y_, scale_z_);

    return clone;
}


}   // namespace insight