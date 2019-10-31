#ifndef INSIGHT_PRIMITIVE_TRIANGLE_H
#define INSIGHT_PRIMITIVE_TRIANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

class Triangle : public Primitive
{
public:
    Triangle(StandardShader* shader) : Primitive(shader) { initialise(); }
    ~Triangle();

    void draw();

protected:
    void initialise();
};

#endif //INSIGHT_PRIMITIVE_TRIANGLE_H
