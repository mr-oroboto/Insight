#ifndef INSIGHT_PRIMITIVE_QUAD_H
#define INSIGHT_PRIMITIVE_QUAD_H

#include <GL/glew.h>

#include "Primitive.h"

namespace insight {
namespace primitive {

class Quad : public Primitive {
public:
    Quad(insight::shader::StandardShader *shader) : Primitive(shader) { initialise(); }
    ~Quad() = default;

    void draw();

protected:
    void initialise();
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_QUAD_H
