#ifndef INSIGHT_CUBE_H
#define INSIGHT_CUBE_H

#include "Primitive.h"

class Cube : public Primitive
{
public:
    Cube(GLuint positionAttribute, GLuint colourAttribute);
    ~Cube();

    void draw();
};

#endif //INSIGHT_CUBE_H
