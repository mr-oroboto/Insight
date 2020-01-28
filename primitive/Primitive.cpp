#include "Primitive.h"

#include <iostream>

Primitive::~Primitive()
{
    std::cout << "Primitive::~Primitive()" << std::endl;

    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);

    if (vertices_ != nullptr)
    {
        delete[] vertices_;
    }
}

void Primitive::setActive()
{
    glBindVertexArray(vao_);
}

Primitive::Type Primitive::getType()
{
    return type_;
}

void Primitive::initialise()
{

}

bool Primitive::getSupportsTransforms()
{
    return supports_transforms_;
}

void Primitive::setCoords(const glm::vec3& from_coords, const glm::vec3& to_coords)
{
    from_world_coords_ = from_coords;
    to_world_coords_ = to_coords;
}

glm::mat4 Primitive::getRotationTransform(const glm::mat4& model_transform)
{
    return glm::mat4(1.0f);
}

glm::mat4 Primitive::getScaleTransform(const glm::mat4& model_transform, const glm::vec3& scale_vector)
{
    return glm::mat4(1.0f);
}

glm::mat4 Primitive::getTranslationTransform(const glm::mat4& model_transform)
{
    return glm::mat4(1.0f);
}
