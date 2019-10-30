#ifndef INSIGHT_PRIMITIVE_RECTANGLE_H
#define INSIGHT_PRIMITIVE_RECTANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

class Rectangle : public Primitive
{
public:
    Rectangle(StandardShader* shader) : Primitive(shader) { initialise(); }
    ~Rectangle();

    void draw();

protected:
    void initialise();
};

#endif //INSIGHT_PRIMITIVE_RECTANGLE_H
