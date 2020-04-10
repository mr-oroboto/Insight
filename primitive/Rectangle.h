#ifndef INSIGHT_PRIMITIVE_RECTANGLE_H
#define INSIGHT_PRIMITIVE_RECTANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

namespace insight {
namespace primitive {

class Rectangle : public Primitive {
public:
    Rectangle(insight::shader::StandardShader* shader);
    ~Rectangle() = default;

    void draw();

protected:
    void initialise();

    unsigned int attributes_per_vertex_;
    unsigned int total_vertices_;
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_RECTANGLE_H
