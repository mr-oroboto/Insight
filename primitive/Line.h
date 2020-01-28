#ifndef INSIGHT_PRIMITIVE_LINE_H
#define INSIGHT_PRIMITIVE_LINE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Primitive.h"

class Line : public Primitive
{
public:
    Line(StandardShader* shader) : Primitive(shader) { initialise(); }
    ~Line();

    glm::mat4 getRotationTransform(const glm::mat4& model_transform);
    glm::mat4 getScaleTransform(const glm::mat4& model_transform, const glm::vec3& scale_vector);
    glm::mat4 getTranslationTransform(const glm::mat4& model_transform);

    void draw();

protected:
    void initialise();
};

#endif //INSIGHT_PRIMITIVE_LINE_H
