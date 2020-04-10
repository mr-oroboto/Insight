#ifndef INSIGHT_PRIMITIVE_QUAD_H
#define INSIGHT_PRIMITIVE_QUAD_H

#include <GL/glew.h>

#include "Primitive.h"

class Quad : public Primitive {
public:
    Quad(insight::shader::StandardShader *shader) : Primitive(shader) { initialise(); }
    ~Quad() = default;

    void draw();

protected:
    void initialise();
};

#endif //INSIGHT_PRIMITIVE_QUAD_H
