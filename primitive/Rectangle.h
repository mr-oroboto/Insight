#ifndef INSIGHT_PRIMITIVE_RECTANGLE_H
#define INSIGHT_PRIMITIVE_RECTANGLE_H

#include <GL/glew.h>

#include "Primitive.h"

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

#endif //INSIGHT_PRIMITIVE_RECTANGLE_H
