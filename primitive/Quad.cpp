#include "Quad.h"

namespace insight {
namespace primitive {

        
void Quad::initialise()
{
    type_ = Primitive::QUAD;

    /**
     * OpenGL Right-Handed Co-ordinates (+y is up, +x is right, +z is out of screen)
     */
    vertices_ = new GLfloat[6 * 9] {
            // x     y      z    nx    ny    nz     r     g     b
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   // z = 0 so facing out of screen, bottom left
             1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   // @todo: fix normal
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   // @todo: fix normal
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   // @todo: fix normal
            -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   // @todo: fix normal
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f    // @todo: fix normal
    };

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 9, vertices_, GL_STATIC_DRAW);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
    glEnableVertexAttribArray(object_shader_->getPositionAttribute());
    glVertexAttribPointer(object_shader_->getPositionAttribute(),
                          3,                  /* num of values to read from array per vertex */
                          GL_FLOAT,           /* type of those values */
                          GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),  /* stride: each (x,y,z) pos now has RGBUV data in between */
                          0                   /* offset */);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
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

void Quad::draw()
{
    setActive();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


}   // namespace primitive
}   // namespace insight