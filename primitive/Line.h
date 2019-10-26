#ifndef INSIGHT_PRIMITIVE_LINE_H
#define INSIGHT_PRIMITIVE_LINE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Primitive.h"

class Line : public Primitive
{
public:
    Line(GLuint position_attribute, GLuint colour_attribute);
    ~Line();

    void setCoords(glm::vec3 from_coords, glm::vec3 to_coords);

    void draw();

private:
    glm::vec3 from_world_coords_;
    glm::vec3 to_world_coords_;
};

#endif //INSIGHT_PRIMITIVE_LINE_H
