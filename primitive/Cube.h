#ifndef INSIGHT_PRIMITIVE_CUBE_H
#define INSIGHT_PRIMITIVE_CUBE_H

#include <GL/glew.h>

#include "Primitive.h"

namespace insight {
namespace primitive {

class Cube : public Primitive {
public:
    Cube(shader::StandardShader* shader) : Primitive(shader) { initialise(); }
    ~Cube() = default;

    void draw();

protected:
    void initialise();
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_CUBE_H
