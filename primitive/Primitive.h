#ifndef INSIGHT_PRIMITIVE_PRIMITIVE_H
#define INSIGHT_PRIMITIVE_PRIMITIVE_H

#include <GL/glew.h>

#include "shader/StandardShader.h"

namespace insight {
namespace primitive {

class Primitive {
public:
    virtual ~Primitive();

    enum Type
    {
        CUBE = 0,
        TRIANGLE,
        LINE,
        QUAD,
        TESSELATION,
        RECTANGLE,
        TRANSFORMING_RECTANGLE
    };

    void setActive();

    Primitive::Type getType();

    // The following methods are only used when the primitive supports transformations
    bool getSupportsTransforms();
    void setCoords(const glm::vec3& from_coords, const glm::vec3& to_coords);
    virtual glm::mat4 getRotationTransform(const glm::mat4& model_transform);
    virtual glm::mat4 getScaleTransform(const glm::mat4& model_transform, const glm::vec3& scale_vector);
    virtual glm::mat4 getTranslationTransform(const glm::mat4& model_transform);

    virtual void draw() = 0;

protected:
    Primitive(shader::StandardShader* shader) : object_shader_(shader) { supports_transforms_ = false; }

    virtual void initialise() = 0;

    Primitive::Type type_;

    shader::StandardShader* object_shader_;

    GLuint vao_;
    GLuint vbo_;
    GLfloat* vertices_;

    bool supports_transforms_;
    glm::vec3 from_world_coords_;
    glm::vec3 to_world_coords_;
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_PRIMITIVE_H
