#include "Rectangle.h"

#include <iostream>

Rectangle::~Rectangle()
{
    std::cout << "Rectangle::~Rectangle()" << std::endl;
}

void Rectangle::initialise()
{
    type_ = Primitive::RECTANGLE;

    /**
     * OpenGL Right-Handed Co-ordinates (+y is up, +x is right, +z is out of screen)
     */
    vertices_ = new GLfloat[36 * 9] {
            // x     y      z     nx    ny    nz    r     g     b
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,       // into screen face (-z) of cube (green)
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             0.5f,  1.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             0.5f,  1.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f,  1.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,        // out of screen face (+z) of cube (red)
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.5f,  1.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.5f,  1.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            -0.5f,  1.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

            -0.5f,  1.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,       // left face (-x) of cube (yellow)
            -0.5f,  1.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -0.5f,  1.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

             0.5f,  1.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,        // right face (+x) of cube (blue)
             0.5f,  1.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f,  1.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,       // bottom face (-y) of cube (cyan)
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            -0.5f,  1.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,        // top face (+y) of cube (white)
             0.5f,  1.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.5f,  1.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.5f,  1.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  1.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  1.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * 9, vertices_, GL_STATIC_DRAW);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
    glEnableVertexAttribArray(object_shader_->getPositionAttribute());
    glVertexAttribPointer(object_shader_->getPositionAttribute(),
                          3,                  /* num of values to read from array per vertex */
                          GL_FLOAT,           /* type of those values */
                          GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),  /* stride: each (x,y,z) pos now has RGBUV data in between */
                          0                   /* offset */);

    glEnableVertexAttribArray(object_shader_->getNormalAttribute());
    glVertexAttribPointer(object_shader_->getNormalAttribute(),
                          3,                          /* num of values to read from array per vertex */
                          GL_FLOAT,                   /* type of those values */
                          GL_FALSE,                   /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),          /* stride: each (x,y,z) pos now has RGBUV data in between */
                          (void*)(3 * sizeof(float))  /* offset */);

    glEnableVertexAttribArray(object_shader_->getColourAttribute());
    glVertexAttribPointer(object_shader_->getColourAttribute(),
                          3,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),         /* stride: each colour block has (x,y,z) data in between */
                          (void*)(6 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

}

void Rectangle::draw()
{
    setActive();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}