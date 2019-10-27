#ifndef INSIGHT_PRIMITIVE_TRIANGLE_H
#define INSIGHT_PRIMITIVE_TRIANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

class Triangle : public Primitive
{
public:
    Triangle(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute);
    ~Triangle();

    void draw();
};

#endif //INSIGHT_PRIMITIVE_TRIANGLE_H
