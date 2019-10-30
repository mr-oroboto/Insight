#ifndef INSIGHT_PRIMITIVE_QUAD_H
#define INSIGHT_PRIMITIVE_QUAD_H

#include <GL/glew.h>

#include "Primitive.h"

class Quad : public Primitive
{
public:
    Quad(StandardShader *shader) : Primitive(shader) { initialise(); }
    ~Quad();

    void draw();

protected:
    void initialise();
};

#endif //INSIGHT_PRIMITIVE_QUAD_H
