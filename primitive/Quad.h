#ifndef INSIGHT_QUAD_H
#define INSIGHT_QUAD_H

#include "Primitive.h"

class Quad : public Primitive
{
public:
    Quad(GLuint positionAttribute, GLuint colourAttribute);
    ~Quad();

    void draw();
};

#endif //INSIGHT_QUAD_H
