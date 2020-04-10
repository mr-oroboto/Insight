#ifndef INSIGHT_PRIMITIVE_TRIANGLE_H
#define INSIGHT_PRIMITIVE_TRIANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

namespace insight {
namespace primitive {

class Triangle : public Primitive {
public:
    Triangle(shader::StandardShader* shader) : Primitive(shader) { initialise(); }
    ~Triangle() = default;

    void draw();

protected:
    void initialise();
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_TRIANGLE_H
