#include "Tesselation.h"

#include <iostream>
#include <random>

#define ATTRIBUTES_PER_VERTEX 11

void insight::primitive::Tesselation::initialise()
{
    type_ = Primitive::TESSELATION;

    tesselation_type_ = Type::FLAT;
    randomise_peaks_ = false;

    is_border_left_ = true;
    is_border_right_ = true;
    is_border_top_ = true;
    is_border_bottom_ = true;

    x_increment_ = 0.5;
    z_increment_ = 0.5;
    y_min_ = 0.05;
    y_max_ = 0.1;
    y_free_seed_ = y_free_ = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(0.01, 1.5);

    for (size_t i = 0; i < 1024; i++)
    {
        random_height_seeds_.push_back(distribution(gen));
    }

    width_ = TESSELATION_WIDTH;
    length_ = width_;                     // DRAGON: don't change this, we assume the tesselation is a square

    vertices_per_subtile_ = 12;           // 4 triangles with open base

    total_vertices_ = static_cast<GLuint>((width_ / x_increment_) * (length_ / z_increment_) * vertices_per_subtile_);
    vertices_ = new GLfloat[total_vertices_ * ATTRIBUTES_PER_VERTEX];
    std::cout << "Creating " << total_vertices_ << " vertices for tesselation, allocated " << (sizeof(GLfloat) * total_vertices_ * 8) << " byte vertice buffer at " << vertices_ << std::endl;

    resetSeamVertices();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
    glEnableVertexAttribArray(object_shader_->getPositionAttribute());
    glVertexAttribPointer(object_shader_->getPositionAttribute(),
                          3,                  /* num of values to read from array per vertex */
                          GL_FLOAT,           /* type of those values */
                          GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                          ATTRIBUTES_PER_VERTEX * sizeof(float), /* stride: each (x,y,z) pos now has RGBUV data in between */
                          0                   /* offset */);

    glEnableVertexAttribArray(object_shader_->getNormalAttribute());
    glVertexAttribPointer(object_shader_->getNormalAttribute(),
                          3,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          ATTRIBUTES_PER_VERTEX * sizeof(float),        /* stride: each (x,y,z) pos now has RGBUV data in between */
                          (void*)(3 * sizeof(float)) /* offset */);

    glEnableVertexAttribArray(object_shader_->getColourAttribute());
    glVertexAttribPointer(object_shader_->getColourAttribute(),
                          3,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          ATTRIBUTES_PER_VERTEX * sizeof(float),        /* stride: each colour block has (x,y,z) data in between */
                          (void*)(6 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

    glEnableVertexAttribArray(object_shader_->getTextureCoordsAttribute());
    glVertexAttribPointer(object_shader_->getTextureCoordsAttribute(),
                          2,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          ATTRIBUTES_PER_VERTEX * sizeof(float),        /* stride: each colour block has (x,y,z) data in between */
                          (void*)(9 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

    initVertices();
}

void insight::primitive::Tesselation::resetSeamVertices()
{
    current_row_bottom_right_y_.clear();
    previous_row_bottom_right_y_.clear();

    current_right_column_bottom_right_y_.clear();
    previous_right_column_bottom_right_y_.clear();

    sub_tiles_per_row_ = 0;
    for (GLfloat x = 0.0f; x < width_; x += x_increment_)
    {
        previous_row_bottom_right_y_.push_back(0);
        previous_right_column_bottom_right_y_.push_back(0);       // assumes square tesselation

        current_row_bottom_right_y_.push_back(0);
        current_right_column_bottom_right_y_.push_back(0);        // assumes sqaure tesselation

        sub_tiles_per_row_++;
    }
}

void insight::primitive::Tesselation::initVertices()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(y_min_, y_max_);

    GLuint current_vertices = 0;
    GLuint current_column = 0, current_row = 0, current_tile = 0;
    GLfloat x, z;

    for (z = 0.0f; z < length_ && current_vertices < total_vertices_; z += z_increment_)
    {
        current_column = 0;

        // Of the tile's four corners, we only get to choose the y value of one of them (the bottom right corner), the
        // rest come from the fact the tesselation itself is a border tile on that corner (ie. y = 0), or the tesselation
        // is bordered by another tesselation, in which case we must smoothly flow on from that tesselation's values.
        //
        // All yFreeBottomRight values of the tile's left side start from the same seed value (which could also be from
        // a neighbouring tesselation).
        y_free_ = y_free_seed_;

        for (x = 0.0f; x < width_ && current_vertices < total_vertices_; x += x_increment_)
        {
            GLfloat y_fixed_top_left, y_fixed_top_right, y_fixed_bottom_left, y_free_bottom_right, y_peak;

            if (tesselation_type_ == Type::RANDOM)
            {
                y_free_ = random_height_seeds_[current_tile++ % random_height_seeds_.size()];
            }

            // Set our only free height variable, the other corners come from previous tiles (in this, or a past tesselation if not isolated)
            switch (tesselation_type_)
            {
                case Type::HILLS:
                    y_free_ += 0.1;
                    y_free_bottom_right = sqrt(sin(y_free_) * sin(y_free_)) + sqrt(cos(y_free_) * tan(y_free_));
                    break;

                case Type::RAMPED:
                    y_free_ += 0.1;
                    y_free_bottom_right = y_free_;
                    break;

                case Type::SIN_LENGTH:
                    y_free_ += 0.1;
                    y_free_bottom_right = sqrt(sin(y_free_) * sin(y_free_));
                    break;

                case Type::FLAT:
                default:
                    y_free_bottom_right = y_free_;
                    break;
            }

            current_row_bottom_right_y_[current_column] = y_free_bottom_right;

            if (current_column == 0)
            {
                /**
                 * First column, pin left side to 0 (or right column of previous tesselation)
                 */

                if (is_border_left_)
                {
                    y_fixed_top_left = 0;
                    y_fixed_bottom_left = 0;
                }
                else
                {
                    if (current_row >= 1)
                    {
                        y_fixed_top_left = previous_right_column_bottom_right_y_[current_row - 1];
                    }
                    else
                    {
                        y_fixed_top_left = previous_right_column_bottom_right_y_[current_row];    // @todo, should be currentRow - 1
                    }

                    y_fixed_bottom_left = previous_right_column_bottom_right_y_[current_row];
                }

                y_fixed_top_right = previous_row_bottom_right_y_[current_column];
            }
            else if (current_column == sub_tiles_per_row_ - 1)
            {
                /**
                 * Last column, pin right side to 0 if isolated
                 */

                y_fixed_top_left = previous_row_bottom_right_y_[current_column - 1];
                y_fixed_bottom_left = current_row_bottom_right_y_[current_column - 1];

                if (is_border_right_)
                {
                    y_fixed_top_right = 0;
                    y_free_bottom_right = 0;
                }
                else
                {
                    y_fixed_top_right = previous_row_bottom_right_y_[current_column]; // zFreeBottomRight remains free (selected earlier)
                }

                current_right_column_bottom_right_y_[current_row] = y_free_bottom_right;
            }
            else
            {
                y_fixed_top_left = previous_row_bottom_right_y_[current_column - 1];
                y_fixed_bottom_left = current_row_bottom_right_y_[current_column - 1];
                y_fixed_top_right = previous_row_bottom_right_y_[current_column];
            }

            if (current_row == 0)
            {
                /**
                 * First row, pin top to 0 if isolated
                 */

                if (is_border_top_)
                {
                    y_fixed_top_left = 0;
                    y_fixed_top_right = 0;
                }
                else
                {
                    if (current_column >= 1)
                    {
                        y_fixed_top_left = previous_row_bottom_right_y_[current_column - 1];
                    }
                    else
                    {
                        y_fixed_top_left = previous_row_bottom_right_y_[current_column];     // @todo, should be currentColumn - 1
                    }

                    y_fixed_top_right = previous_row_bottom_right_y_[current_column];
                }
            }

            if (current_row == sub_tiles_per_row_ - 1)
            {
                /**
                 * Last row assuming we have a square tile, pin bottom to 0 if isolated
                 */

                if (is_border_bottom_)                                         // no need to reset these if not isolated
                {
                    y_fixed_bottom_left = 0;
                    y_free_bottom_right = 0;
                }
            }

            if (randomise_peaks_)
            {
                y_peak = y_fixed_top_left + distribution(gen);
            }
            else
            {
                y_peak = y_fixed_top_left;
            }

            // @todo: fix normals
            GLfloat tile_vertices[12 * ATTRIBUTES_PER_VERTEX] = {
                     // x                       y                                 z                    nx    ny    nz    r  g  b   s     t
                     x + x_increment_,          y_fixed_top_right,         z,                         1.0f, 0.0f, 0.0f,  1, 0, 0, 0.0f, 1.0f,   // +x face (right)
                     x + x_increment_,          y_free_bottom_right,       z + z_increment_,          1.0f, 0.0f, 0.0f,  1, 0, 0, 1.0f, 1.0f,
                     x + (x_increment_ / 2.0f), y_peak,                    z + (z_increment_ / 2.0f), 1.0f, 0.0f, 0.0f,  1, 0, 0, 0.5f, 0.5f,

                     x + (x_increment_ / 2.0f), y_peak,                    z + (z_increment_ / 2.0f), 0.0f, 0.0f, 1.0f,  0, 0, 1, 0.5f, 0.5f,   // +z face (facing camera)
                     x + x_increment_,          y_free_bottom_right,       z + z_increment_,          0.0f, 0.0f, 1.0f,  0, 0, 1, 1.0f, 1.0f,
                     x,                         y_fixed_bottom_left,       z + z_increment_,          0.0f, 0.0f, 1.0f,  0, 0, 1, 0.0f, 1.0f,

                     x,                         y_fixed_bottom_left,       z + z_increment_,          -1.0f, 0.0f, 0.0f, 0, 1, 0, 0.0f, 1.0f,  // -x face (left)
                     x,                         y_fixed_top_left,          z,                         -1.0f, 0.0f, 0.0f, 0, 1, 0, 0.0f, 0.0f,
                     x + (x_increment_ / 2.0f), y_peak,                    z + (z_increment_ / 2.0f), -1.0f, 0.0f, 0.0f, 0, 1, 0, 0.5f, 0.5f,

                     x + (x_increment_ / 2.0f), y_peak,                    z + (z_increment_ / 2.0f), 0.0f, 0.0f, -1.0f, 1, 0, 1, 0.5f, 0.5f,  // -z face (into screen)
                     x,                         y_fixed_top_left,          z,                         0.0f, 0.0f, -1.0f, 1, 0, 1, 0.0f, 0.0f,
                     x + x_increment_,          y_fixed_top_right,         z,                         0.0f, 0.0f, -1.0f, 1, 0, 1, 0.0f, 1.0f
            };

            memcpy(vertices_ + (current_vertices * ATTRIBUTES_PER_VERTEX), tile_vertices, sizeof(GLfloat) * vertices_per_subtile_ * ATTRIBUTES_PER_VERTEX);
            current_vertices += vertices_per_subtile_;

            current_column++;
        }

        previous_row_bottom_right_y_ = current_row_bottom_right_y_;
        current_row++;
    }

    glBindVertexArray(vao_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * total_vertices_ * ATTRIBUTES_PER_VERTEX, vertices_, GL_STATIC_DRAW);
}

void insight::primitive::Tesselation::setPreviousBottomRowBottomRightY(const std::vector<GLfloat>& initial_previous_bottom_row_bottom_right_y)
{
    previous_row_bottom_right_y_ = initial_previous_bottom_row_bottom_right_y;
    setBorderTop(false);
}

void insight::primitive::Tesselation::setPreviousRightColumnBottomRightY(const std::vector<GLfloat>& initial_previous_right_column_bottom_right_y)
{
    previous_right_column_bottom_right_y_ = initial_previous_right_column_bottom_right_y;
    setBorderLeft(false);
}

void insight::primitive::Tesselation::setIsolated()
{
    setBorderLeft(true);
    setBorderRight(true);
    setBorderTop(true);
    setBorderBottom(true);
}

void insight::primitive::Tesselation::setBorderLeft(bool border)
{
    is_border_left_ = border;
}

void insight::primitive::Tesselation::setBorderRight(bool border)
{
    is_border_right_ = border;
}

void insight::primitive::Tesselation::setBorderTop(bool border)
{
    is_border_top_ = border;
}

void insight::primitive::Tesselation::setBorderBottom(bool border)
{
    is_border_bottom_ = border;
}

void insight::primitive::Tesselation::setYFreeSeed(GLfloat seed_value)
{
    y_free_seed_ = seed_value;
}

GLfloat insight::primitive::Tesselation::getYFree()
{
    return y_free_;
}

std::vector<GLfloat> insight::primitive::Tesselation::getBottomRowBottomRightY()
{
    return current_row_bottom_right_y_;
}

std::vector<GLfloat> insight::primitive::Tesselation::getRightColumnBottomRightY()
{
    return current_right_column_bottom_right_y_;
}

void insight::primitive::Tesselation::setRandomisePeaks(bool peaks)
{
    randomise_peaks_ = peaks;
}

void insight::primitive::Tesselation::setType(Type type)
{
    tesselation_type_ = type;
}

void insight::primitive::Tesselation::draw()
{
    setActive();
    glDrawArrays(GL_TRIANGLES, 0, total_vertices_);
}