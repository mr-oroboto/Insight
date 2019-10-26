#ifndef INSIGHT_PRIMITIVE_CUBE_H
#define INSIGHT_PRIMITIVE_CUBE_H

#include <GL/glew.h>

#include "Primitive.h"

class Cube : public Primitive
{
public:
    Cube(GLuint positionAttribute, GLuint colourAttribute);
    ~Cube();

    void draw();
};

#endif //INSIGHT_PRIMITIVE_CUBE_H
