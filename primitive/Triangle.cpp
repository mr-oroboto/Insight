#include "Triangle.h"

#include <iostream>

Triangle::Triangle(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute)
{
    type_ = Primitive::TRIANGLE;

    vertices_ = new GLfloat[3 * 9] {
            // x     y     z     nx    ny    nz    r     g     b
             0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top @todo: fix normal
             0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom right @todo: fix normal
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f  // bottom left @todo: fix normal
    };

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 9, vertices_, GL_STATIC_DRAW);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
    glEnableVertexAttribArray(position_attribute);
    glVertexAttribPointer(position_attribute,
                          3                   /* num of values to read from array per vertex */,
                          GL_FLOAT            /* type of those values */,
                          GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float)   /* stride: each (x,y,z) pos now has RGBUV data in between */,
                          0                   /* offset */);

    glEnableVertexAttribArray(normal_attribute);
    glVertexAttribPointer(normal_attribute,
                          3                          /* num of values to read from array per vertex */,
                          GL_FLOAT                   /* type of those values */,
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float)          /* stride: each (x,y,z) pos now has RGBUV data in between */,
                          (void*)(3 * sizeof(float)) /* offset */);

    glEnableVertexAttribArray(colour_attribute);
    glVertexAttribPointer(colour_attribute,
                          3                          /* num of values to read from array per vertex */,
                          GL_FLOAT                   /* type of those values */,
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float)          /* stride: each colour block has (x,y,z) data in between */,
                          (void*)(6 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

}

Triangle::~Triangle()
{
    std::cout << "Triangle::~Triangle()" << std::endl;
}

void Triangle::draw()
{
    setActive();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}