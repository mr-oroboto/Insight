#ifndef INSIGHT_PRIMITIVE_H
#define INSIGHT_PRIMITIVE_H

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
        TRIANGLE
    };

    void setActive();
    virtual void draw() = 0;

protected:
    GLuint  vao;
    GLuint  vbo;
    GLfloat *vertices;
};

#endif //INSIGHT_PRIMITIVE_H
