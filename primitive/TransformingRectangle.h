#ifndef INSIGHT_PRIMITIVE_TRANSFORMINGRECTANGLE_H
#define INSIGHT_PRIMITIVE_TRANSFORMINGRECTANGLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Rectangle.h"

namespace insight {
namespace primitive {

class TransformingRectangle : public Rectangle {
public:
    TransformingRectangle(shader::StandardShader* shader);
    ~TransformingRectangle() = default;

    glm::mat4 getRotationTransform(const glm::mat4& model_transform);
    glm::mat4 getScaleTransform(const glm::mat4& model_transform, const glm::vec3& scale_vector);
    glm::mat4 getTranslationTransform(const glm::mat4& model_transform);

protected:
    void initialise();
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_TRANSFORMINGRECTANGLE_H
