#ifndef INSIGHT_PRIMITIVE_PRIMITIVE_H
#define INSIGHT_PRIMITIVE_PRIMITIVE_H

#include <GL/glew.h>

class Primitive
{
public:
    Primitive() {}
    Primitive(GLuint positionAttribute, GLuint colourAttribute) {}
    virtual ~Primitive();

    enum Type
    {
        CUBE = 0,
        TRIANGLE,
        LINE,
        QUAD,
        TESSELATION
    };

    void setActive();
    Primitive::Type getType();
    virtual void draw() = 0;

protected:
    Primitive::Type type;
    GLuint  vao;
    GLuint  vbo;
    GLfloat* vertices;
};

#endif //INSIGHT_PRIMITIVE_PRIMITIVE_H
