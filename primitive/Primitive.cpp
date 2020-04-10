#include "Primitive.h"

insight::primitive::Primitive::~Primitive()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);

    if (vertices_ != nullptr)
    {
        delete[] vertices_;
    }
}

void insight::primitive::Primitive::setActive()
{
    glBindVertexArray(vao_);
}

insight::primitive::Primitive::Type insight::primitive::Primitive::getType()
{
    return type_;
}

bool insight::primitive::Primitive::getSupportsTransforms()
{
    return supports_transforms_;
}

void insight::primitive::Primitive::setCoords(const glm::vec3& from_coords, const glm::vec3& to_coords)
{
    from_world_coords_ = from_coords;
    to_world_coords_ = to_coords;
}

glm::mat4 insight::primitive::Primitive::getRotationTransform(const glm::mat4& model_transform)
{
    return glm::mat4(1.0f);
}

glm::mat4 insight::primitive::Primitive::getScaleTransform(const glm::mat4& model_transform, const glm::vec3& scale_vector)
{
    return glm::mat4(1.0f);
}

glm::mat4 insight::primitive::Primitive::getTranslationTransform(const glm::mat4& model_transform)
{
    return glm::mat4(1.0f);
}
