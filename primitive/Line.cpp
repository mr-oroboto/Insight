#include "Line.h"

#include <iostream>

Line::Line(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute)
{
    type_ = Primitive::LINE;

    /**
     * The "face" descriptions below assume a co-ordinate system where +z is going up, +x is coming out of the screen
     * going left and +y is coming out of the screen going right
     */
    vertices_ = new GLfloat[2 * 9] {
            // x    y    z     nx    ny    nz    r     g     b
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // @todo: fix normal
            1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f   // @todo: fix normal
    };

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 9, vertices_, GL_DYNAMIC_DRAW);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
    glEnableVertexAttribArray(position_attribute);
    glVertexAttribPointer(position_attribute,
                          3,                  /* num of values to read from array per vertex */
                          GL_FLOAT,           /* type of those values */
                          GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),  /* stride: each (x,y,z) pos now has RGBUV data in between */
                          0                   /* offset */);

    glEnableVertexAttribArray(normal_attribute);
    glVertexAttribPointer(normal_attribute,
                          3,                          /* num of values to read from array per vertex */
                          GL_FLOAT,                   /* type of those values */
                          GL_FALSE,                   /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),          /* stride: each (x,y,z) pos now has RGBUV data in between */
                          (void*)(3 * sizeof(float))  /* offset */);

    glEnableVertexAttribArray(colour_attribute);
    glVertexAttribPointer(colour_attribute,
                          3,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),         /* stride: each colour block has (x,y,z) data in between */
                          (void*)(6 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

}

Line::~Line()
{
    std::cout << "Line::~Line()" << std::endl;
}

void Line::setCoords(glm::vec3 from_coords, glm::vec3 to_coords)
{
    from_world_coords_ = from_coords;
    to_world_coords_ = to_coords;
}

void Line::draw()
{
    setActive();

    // Update the vertices
    delete vertices_;

    vertices_ = new GLfloat[2 * 9] {
            //       x                   y                    z                nx    ny    nz    r     g     b
            from_world_coords_.x, from_world_coords_.y, from_world_coords_.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            to_world_coords_.x,   to_world_coords_.y,   to_world_coords_.z,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 9, vertices_, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, 2);
}