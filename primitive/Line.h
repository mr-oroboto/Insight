#ifndef INSIGHT_PRIMITIVE_LINE_H
#define INSIGHT_PRIMITIVE_LINE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Primitive.h"

namespace insight {
namespace primitive {

class Line : public Primitive {
public:
    Line(insight::shader::StandardShader* shader) : Primitive(shader) { initialise(); }
    ~Line() = default;

    glm::mat4 getRotationTransform(const glm::mat4& model_transform);
    glm::mat4 getScaleTransform(const glm::mat4& model_transform, const glm::vec3& scale_vector);
    glm::mat4 getTranslationTransform(const glm::mat4& model_transform);

    void draw();

protected:
    void initialise();
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_LINE_H
