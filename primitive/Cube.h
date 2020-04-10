#ifndef INSIGHT_PRIMITIVE_CUBE_H
#define INSIGHT_PRIMITIVE_CUBE_H

#include <GL/glew.h>

#include "Primitive.h"

class Cube : public Primitive {
public:
    Cube(insight::shader::StandardShader* shader) : Primitive(shader) { initialise(); }
    ~Cube() = default;

    void draw();

protected:
    void initialise();
};

#endif //INSIGHT_PRIMITIVE_CUBE_H
