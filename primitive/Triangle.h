#ifndef INSIGHT_PRIMITIVE_TRIANGLE_H
#define INSIGHT_PRIMITIVE_TRIANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

class Triangle : public Primitive
{
public:
    Triangle(GLuint positionAttribute, GLuint colourAttribute);
    ~Triangle();

    void draw();
};

#endif //INSIGHT_PRIMITIVE_TRIANGLE_H
