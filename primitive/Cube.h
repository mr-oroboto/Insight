#ifndef INSIGHT_PRIMITIVE_CUBE_H
#define INSIGHT_PRIMITIVE_CUBE_H

#include <GL/glew.h>

#include "Primitive.h"

class Cube : public Primitive
{
public:
    Cube(GLuint position_attribute, GLuint colour_attribute);
    ~Cube();

    void draw();
};

#endif //INSIGHT_PRIMITIVE_CUBE_H
