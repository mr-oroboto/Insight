#ifndef INSIGHT_TRIANGLE_H
#define INSIGHT_TRIANGLE_H

#include "Primitive.h"

class Triangle : public Primitive
{
public:
    Triangle(GLuint positionAttribute, GLuint colourAttribute);
    ~Triangle();

    void draw();
};

#endif //INSIGHT_TRIANGLE_H
