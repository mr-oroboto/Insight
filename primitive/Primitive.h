#ifndef INSIGHT_PRIMITIVE_PRIMITIVE_H
#define INSIGHT_PRIMITIVE_PRIMITIVE_H

#include <GL/glew.h>

class Primitive
{
public:
    Primitive() {}
    Primitive(GLuint position_attribute, GLuint colour_attribute) {}
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
    Primitive::Type type_;
    GLuint vao_;
    GLuint vbo_;
    GLfloat* vertices_;
};

#endif //INSIGHT_PRIMITIVE_PRIMITIVE_H
