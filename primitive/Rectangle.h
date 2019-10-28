#ifndef INSIGHT_PRIMITIVE_RECTANGLE_H
#define INSIGHT_PRIMITIVE_RECTANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

class Rectangle : public Primitive
{
public:
    Rectangle(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute);
    ~Rectangle();

    void draw();
};

#endif //INSIGHT_PRIMITIVE_RECTANGLE_H
