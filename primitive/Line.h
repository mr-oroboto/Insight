#ifndef INSIGHT_PRIMITIVE_LINE_H
#define INSIGHT_PRIMITIVE_LINE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Primitive.h"

class Line : public Primitive
{
public:
    Line(GLuint positionAttribute, GLuint colourAttribute);
    ~Line();

    void setCoords(glm::vec3 from, glm::vec3 to);

    void draw();

private:
    glm::vec3 fromWorldPosition;
    glm::vec3 toWorldPosition;
};

#endif //INSIGHT_PRIMITIVE_LINE_H
