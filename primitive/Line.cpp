#include "Line.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define USE_TRANSFORMS true

Line::Line(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute)
{
    type_ = Primitive::LINE;

    /**
     * OpenGL Right-Handed Co-ordinates (+y is up, +x is right, +z is out of screen)
     */
    vertices_ = new GLfloat[2 * 9] {
            // x    y    z     nx    ny    nz    r     g     b
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // @todo: fix normal
            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f   // @todo: fix normal
    };

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 9, vertices_, USE_TRANSFORMS ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

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

void Line::setCoords(const glm::vec3& from_coords, const glm::vec3& to_coords)
{
    from_world_coords_ = from_coords;
    to_world_coords_ = to_coords;
}

glm::mat4 Line::getRotationTransform(const glm::mat4 &model_transform)
{
    if (USE_TRANSFORMS)
    {
        /**
         * Rotation is achieved by re-aligning our model's y-axis. Our "model line" vertices define a unit vector
         * pointing upwards (+y) from (0,0,0). By re-aligning (or pointing) our y-axis in the direction we want
         * our line to point (defined by the vector between the to and from co-ordinates of the line) we get a new
         * line that is pointing from (0,0,0) to the direction we need to go (it can then be translated and scaled).
         *
         * As a thought experiment, imagine we want a line from (0, 5, 0) to (5, 0, 0): the "arbitrary axis" that
         * is normal to both +y and +x is z (we need to rotate around z to go from our +y to our +x direction). Now
         * imagine that we want to go from (0, 5, 0) to (0, 0, 5): the arbitrary axis normal to both +y and +z is
         * the x axis (which is what we rotate 90 degrees around to make this new direction).
         */

        // create a unit vector pointing in the direction we need to go
        glm::vec3 direction_vector = glm::normalize(to_world_coords_ - from_world_coords_);

        // rotate it around an arbitrary axis that is normal to both the direction and our original y-axis
        glm::vec3 rotation_axis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction_vector));

        // special case if we want to flip around to a negative y value, we can rotate around the x axis
        if (direction_vector.x == 0 && direction_vector.z == 0)
        {
            rotation_axis = glm::vec3(1, 0, 0);
        }

        // the amount of rotation is the difference between our two unit vectors (the first is defined by the model's
        // fixed vertices, which point to +1 y
        GLfloat radians = acos(glm::dot(glm::vec3(0, 1, 0), direction_vector));

        return glm::rotate(model_transform, radians, rotation_axis);
    }

    return glm::mat4(1.0f);
}

glm::mat4 Line::getScaleTransform(const glm::mat4 &model_transform)
{
    if (USE_TRANSFORMS)
    {
        glm::vec3 direction_vector = from_world_coords_ - to_world_coords_;
        glm::vec3 scale_vector = glm::vec3(glm::length(direction_vector), glm::length(direction_vector), glm::length(direction_vector));
        return glm::scale(model_transform, scale_vector);
    }

    return glm::mat4(1.0f);
}

glm::mat4 Line::getTranslationTransform(const glm::mat4 &model_transform)
{
    if (USE_TRANSFORMS)
    {
        return glm::translate(model_transform, from_world_coords_);
    }

    return glm::mat4(1.0f);
}

void Line::draw()
{
    setActive();

    if ( ! USE_TRANSFORMS)
    {
        // Update the vertices
        delete vertices_;

        vertices_ = new GLfloat[2 * 9] {
                //       x                   y                    z                nx    ny    nz    r     g     b
                from_world_coords_.x, from_world_coords_.y, from_world_coords_.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                to_world_coords_.x,   to_world_coords_.y,   to_world_coords_.z,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 9, vertices_, GL_DYNAMIC_DRAW);
    }

    glDrawArrays(GL_LINES, 0, 2);
}