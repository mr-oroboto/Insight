#include "TransformingRectangle.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

TransformingRectangle::TransformingRectangle(StandardShader *shader)
        : Rectangle(shader)
{
    initialise();
}

TransformingRectangle::~TransformingRectangle()
{
    std::cout << "TransformingRectangle::~TransformingRectangle()" << std::endl;
}

void TransformingRectangle::initialise()
{
    type_ = Primitive::TRANSFORMING_RECTANGLE;
    supports_transforms_ = true;
}

glm::mat4 TransformingRectangle::getTranslationTransform(const glm::mat4 &model_transform)
{
//    std::cout << "from (" << from_world_coords_.x << "," << from_world_coords_.y << "," << from_world_coords_.z << ")" << std::endl;
    return glm::translate(model_transform, from_world_coords_);
}

glm::mat4 TransformingRectangle::getRotationTransform(const glm::mat4 &model_transform)
{
    /**
     * Rotation is achieved by re-aligning our model's y-axis. Our "model line" vertices define a unit vector
     * pointing upwards (+y) from (0,0,0). By re-aligning (or pointing) our y-axis in the direction we want
     * our line to point (defined by the vector between the to and from co-ordinates of the line) we get a new
     * line that is pointing from (0,0,0) to the direction we need to go (it can then be translated and scaled).
     *
     * As a thought experiment, imagine we want a line from (0, 5, 0) to (5, 0, 0): the "arbitrary axis" that
     * is normal to both +y and +x is z (we need to rotate around z to go from our +y to our +x direction). Now
     * imagine that we want to go from (0, 5, 0) to (0, 0, 5): the arbitrary axis normal to both +y and +z is
     * the x axis (which is what we rotate 90 degrees around to make this new direction).
     */

    // create a unit vector pointing in the direction we need to go
    glm::vec3 direction_vector = glm::normalize(to_world_coords_ - from_world_coords_);

    // rotate it around an arbitrary axis that is normal to both the direction and our original y-axis
    glm::vec3 rotation_axis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction_vector));

    // special case if we want to flip around to a negative y value, we can rotate around the x axis
    if (direction_vector.x == 0 && direction_vector.z == 0)
    {
        rotation_axis = glm::vec3(1, 0, 0);
    }

    // the amount of rotation is the difference between our two unit vectors (the first is defined by the model's
    // fixed vertices, which point to +1 y
    GLfloat radians = acos(glm::dot(glm::vec3(0, 1, 0), direction_vector));

    return glm::rotate(model_transform, radians, rotation_axis);
}

glm::mat4 TransformingRectangle::getScaleTransform(const glm::mat4 &model_transform, const glm::vec3& scale_vector)
{
    glm::vec3 direction_vector = from_world_coords_ - to_world_coords_;
    glm::vec3 adjusted_scale_vector = glm::vec3(scale_vector.x, glm::length(direction_vector), scale_vector.z);

    return glm::scale(model_transform, adjusted_scale_vector);
}

