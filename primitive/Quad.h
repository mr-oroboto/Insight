#ifndef INSIGHT_PRIMITIVE_QUAD_H
#define INSIGHT_PRIMITIVE_QUAD_H

#include <GL/glew.h>

#include "Primitive.h"

class Quad : public Primitive
{
public:
    Quad(GLuint positionAttribute, GLuint colourAttribute);
    ~Quad();

    void draw();
};

#endif //INSIGHT_PRIMITIVE_QUAD_H
