#ifndef INSIGHT_PRIMITIVE_PRIMITIVE_H
#define INSIGHT_PRIMITIVE_PRIMITIVE_H

#include <GL/glew.h>

#include "shader/StandardShader.h"

class Primitive
{
public:
    virtual ~Primitive();

    enum Type
    {
        CUBE = 0,
        TRIANGLE,
        LINE,
        QUAD,
        TESSELATION,
        RECTANGLE
    };

    void setActive();

    Primitive::Type getType();

    virtual void draw() = 0;

protected:
    Primitive(StandardShader* shader) : object_shader_(shader) { }

    virtual void initialise() = 0;

    Primitive::Type type_;

    StandardShader* object_shader_;

    GLuint vao_;
    GLuint vbo_;
    GLfloat* vertices_;
};

#endif //INSIGHT_PRIMITIVE_PRIMITIVE_H
