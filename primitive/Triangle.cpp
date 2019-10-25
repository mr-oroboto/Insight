#include "Triangle.h"
#include <iostream>

Triangle::Triangle(GLuint positionAttribute, GLuint colourAttribute)
{
    type = Primitive::TRIANGLE;

    vertices = new GLfloat[3 * 6] {
             0.0f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6, vertices, GL_STATIC_DRAW);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute,
                          3                   /* num of values to read from array per vertex */,
                          GL_FLOAT            /* type of those values */,
                          GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                          6 * sizeof(float)   /* stride: each (x,y,z) pos now has RGBUV data in between */,
                          0                   /* offset */);

    glEnableVertexAttribArray(colourAttribute);
    glVertexAttribPointer(colourAttribute,
                          3                          /* num of values to read from array per vertex */,
                          GL_FLOAT                   /* type of those values */,
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          6 * sizeof(float)          /* stride: each colour block has (x,y,z) data in between */,
                          (void*)(3 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

}

Triangle::~Triangle()
{
    std::cout << "Triangle::~Triangle()" << std::endl;

    if (vertices != nullptr)
    {
        delete vertices;
    }
}

void Triangle::draw()
{
    setActive();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}