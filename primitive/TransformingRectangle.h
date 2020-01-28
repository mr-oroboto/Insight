#ifndef INSIGHT_PRIMITIVE_TRANSFORMINGRECTANGLE_H
#define INSIGHT_PRIMITIVE_TRANSFORMINGRECTANGLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Rectangle.h"

class TransformingRectangle : public Rectangle
{
public:
    TransformingRectangle(StandardShader* shader);
    ~TransformingRectangle();

    glm::mat4 getRotationTransform(const glm::mat4& model_transform);
    glm::mat4 getScaleTransform(const glm::mat4& model_transform);
    glm::mat4 getTranslationTransform(const glm::mat4& model_transform);

protected:
    void initialise();
};

#endif //INSIGHT_PRIMITIVE_TRANSFORMINGRECTANGLE_H
