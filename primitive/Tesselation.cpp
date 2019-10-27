#include "Tesselation.h"

#include <cstring>
#include <iostream>
#include <random>

Tesselation::Tesselation(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute)
{
    type_ = Primitive::TESSELATION;

    tesselation_type_ = Type::FLAT;
    randomise_peaks_ = false;

    is_border_left_ = true;
    is_border_right_ = true;
    is_border_top_ = true;
    is_border_bottom_ = true;

    x_increment_ = 0.5;
    y_increment_ = 0.5;
    z_min_ = 0.05;
    z_max_ = 0.1;
    z_free_seed_ = z_free_ = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(0.01, 1.5);

    for (size_t i = 0; i < 1024; i++)
    {
        random_height_seeds_.push_back(distribution(gen));
    }

    width_ = 6.0f;
    length_ = width_;                     // DRAGON: don't change this, we assume the tesselation is a square

    vertices_per_subtile_ = 12;       // 4 triangles with open base

    total_vertices_ = static_cast<GLuint>((width_ / x_increment_) * (length_ / y_increment_) * vertices_per_subtile_);
    vertices_ = new GLfloat[total_vertices_ * 9];
    std::cout << "creating " << total_vertices_ << " vertices for tesselation, allocated " << (sizeof(GLfloat) * total_vertices_ * 8) << " byte vertice buffer at " << vertices_ << std::endl;

    resetSeamVertices();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

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
                          3,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),         /* stride: each (x,y,z) pos now has RGBUV data in between */
                          (void*)(3 * sizeof(float)) /* offset */);

    glEnableVertexAttribArray(colour_attribute);
    glVertexAttribPointer(colour_attribute,
                          3,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          9 * sizeof(float),         /* stride: each colour block has (x,y,z) data in between */
                          (void*)(6 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

    initVertices();
}

Tesselation::~Tesselation()
{
    std::cout << "Tesselation::~Tesselation()" << std::endl;
}

void Tesselation::resetSeamVertices()
{
    current_row_bottom_right_z_.clear();
    previous_row_bottom_right_z_.clear();

    current_right_column_bottom_right_z_.clear();
    previous_right_column_bottom_right_z_.clear();

    sub_tiles_per_row_ = 0;
    for (GLfloat x = 0.0f; x < width_; x += x_increment_)
    {
        previous_row_bottom_right_z_.push_back(0);
        previous_right_column_bottom_right_z_.push_back(0);           // assumes square tesselation

        current_row_bottom_right_z_.push_back(0);
        current_right_column_bottom_right_z_.push_back(0);        // assumes sqaure tesselation

        sub_tiles_per_row_++;
    }
}

void Tesselation::initVertices()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(z_min_, z_max_);

    GLuint current_vertices = 0;
    GLuint current_column = 0, current_row = 0, current_tile = 0;
    GLfloat x, y;

    for (y = 0.0f; y < length_ && current_vertices < total_vertices_; y += y_increment_)
    {
        current_column = 0;

        // Of the tile's four corners, we only get to choose the Z value of one of them (the bottom right corner), the
        // rest come from the fact the tesselation itself is a border tile on that corner (ie. z = 0), or the tesselation
        // is bordered by another tesselation, in which case we must smoothly flow on from that tesselation's values.
        //
        // All zFreeBottomRight values of the tile's left side start from the same seed value (which could also be from
        // a neighbouring tesselation).
        z_free_ = z_free_seed_;

        for (x = 0.0f; x < width_ && current_vertices < total_vertices_; x += x_increment_)
        {
            GLfloat z_fixed_top_left, z_fixed_top_right, z_fixed_bottom_left, z_free_bottom_right, z_peak;

            if (tesselation_type_ == Type::RANDOM)
            {
                z_free_ = random_height_seeds_[current_tile++ % random_height_seeds_.size()];
            }

            // Set our only free height variable, the other corners come from previous tiles (in this, or a past tesselation if not isolated)
            switch (tesselation_type_)
            {
                case Type::HILLS:
                    z_free_ += 0.1;
                    z_free_bottom_right = sqrt(sin(z_free_) * sin(z_free_)) + sqrt(cos(z_free_) * tan(z_free_));
                    break;

                case Type::RAMPED:
                    z_free_ += 0.1;
                    z_free_bottom_right = z_free_;
                    break;

                case Type::SIN_LENGTH:
                    z_free_ += 0.1;
                    z_free_bottom_right = sqrt(sin(z_free_) * sin(z_free_));
                    break;

                case Type::FLAT:
                default:
                    z_free_bottom_right = z_free_;
                    break;
            }

            current_row_bottom_right_z_[current_column] = z_free_bottom_right;

            if (current_column == 0)
            {
                /**
                 * First column, pin left side to 0 (or right column of previous tesselation)
                 */

                if (is_border_left_)
                {
                    z_fixed_top_left = 0;
                    z_fixed_bottom_left = 0;
                }
                else
                {
                    if (current_row >= 1)
                    {
                        z_fixed_top_left = previous_right_column_bottom_right_z_[current_row - 1];
                    }
                    else
                    {
                        z_fixed_top_left = previous_right_column_bottom_right_z_[current_row];    // @todo, should be currentRow - 1
                    }

                    z_fixed_bottom_left = previous_right_column_bottom_right_z_[current_row];
                }

                z_fixed_top_right = previous_row_bottom_right_z_[current_column];
            }
            else if (current_column == sub_tiles_per_row_ - 1)
            {
                /**
                 * Last column, pin right side to 0 if isolated
                 */

                z_fixed_top_left = previous_row_bottom_right_z_[current_column - 1];
                z_fixed_bottom_left = current_row_bottom_right_z_[current_column - 1];

                if (is_border_right_)
                {
                    z_fixed_top_right = 0;
                    z_free_bottom_right = 0;
                }
                else
                {
                    z_fixed_top_right = previous_row_bottom_right_z_[current_column]; // zFreeBottomRight remains free (selected earlier)
                }

                current_right_column_bottom_right_z_[current_row] = z_free_bottom_right;
            }
            else
            {
                z_fixed_top_left = previous_row_bottom_right_z_[current_column - 1];
                z_fixed_bottom_left = current_row_bottom_right_z_[current_column - 1];
                z_fixed_top_right = previous_row_bottom_right_z_[current_column];
            }

            if (current_row == 0)
            {
                /**
                 * First row, pin top to 0 if isolated
                 */

                if (is_border_top_)
                {
                    z_fixed_top_left = 0;
                    z_fixed_top_right = 0;
                }
                else
                {
                    if (current_column >= 1)
                    {
                        z_fixed_top_left = previous_row_bottom_right_z_[current_column - 1];
                    }
                    else
                    {
                        z_fixed_top_left = previous_row_bottom_right_z_[current_column];     // @todo, should be currentColumn - 1
                    }

                    z_fixed_top_right = previous_row_bottom_right_z_[current_column];
                }
            }

            if (current_row == sub_tiles_per_row_ - 1)
            {
                /**
                 * Last row assuming we have a square tile, pin bottom to 0 if isolated
                 */

                if (is_border_bottom_)                                         // no need to reset these if not isolated
                {
                    z_fixed_bottom_left = 0;
                    z_free_bottom_right = 0;
                }
            }

            if (randomise_peaks_)
            {
                z_peak = z_fixed_top_left + distribution(gen);
            }
            else
            {
                z_peak = z_fixed_top_left;
            }

            // @todo: fix normals
            GLfloat tile_vertices[12 * 9] = {
                     // x                       y                                 z              nx    ny    nz   r  g  b
                     x + x_increment_,          y,                         z_fixed_top_right,   1.0f, 0.0f, 0.0f, 1, 0, 0,  // +x face (left)
                     x + x_increment_,          y + y_increment_,          z_free_bottom_right, 1.0f, 0.0f, 0.0f, 1, 0, 0,
                     x + (x_increment_ / 2.0f), y + (y_increment_ / 2.0f), z_peak,              1.0f, 0.0f, 0.0f, 1, 0, 0,

                     x + (x_increment_ / 2.0f), y + (y_increment_ / 2.0f), z_peak,              0.0f, 1.0f, 0.0f, 0, 0, 1,  // +y face (facing camera)
                     x + x_increment_,          y + y_increment_,          z_free_bottom_right, 0.0f, 1.0f, 0.0f, 0, 0, 1,
                     x,                         y + y_increment_,          z_fixed_bottom_left, 0.0f, 1.0f, 0.0f, 0, 0, 1,

                     x,                         y + y_increment_,          z_fixed_bottom_left, -1.0f, 0.0f, 0.0f, 0, 1, 0,  // -x face (right)
                     x,                         y,                         z_fixed_top_left,    -1.0f, 0.0f, 0.0f, 0, 1, 0,
                     x + (x_increment_ / 2.0f), y + (y_increment_ / 2.0f), z_peak,              -1.0f, 0.0f, 0.0f, 0, 1, 0,

                     x + (x_increment_ / 2.0f), y + (y_increment_ / 2.0f), z_peak,              0.0f, -1.0f, 0.0f, 1, 0, 1,  // -y face
                     x,                         y,                         z_fixed_top_left,    0.0f, -1.0f, 0.0f, 1, 0, 1,
                     x + x_increment_,          y,                         z_fixed_top_right,   0.0f, -1.0f, 0.0f, 1, 0, 1
            };

            memcpy(vertices_ + (current_vertices * 9), tile_vertices, sizeof(GLfloat) * vertices_per_subtile_ * 9);
            current_vertices += vertices_per_subtile_;

            current_column++;
        }

        previous_row_bottom_right_z_ = current_row_bottom_right_z_;
        current_row++;
    }

    glBindVertexArray(vao_);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * total_vertices_ * 9, vertices_, GL_STATIC_DRAW);
}

void Tesselation::setPreviousBottomRowBottomRightZ(std::vector<GLfloat> initial_previous_bottom_row_bottom_right_z)
{
    previous_row_bottom_right_z_ = initial_previous_bottom_row_bottom_right_z;
    setBorderTop(false);
}

void Tesselation::setPreviousRightColumnBottomRightZ(std::vector<GLfloat> initial_previous_right_column_bottom_right_z)
{
    previous_right_column_bottom_right_z_ = initial_previous_right_column_bottom_right_z;
    setBorderLeft(false);
}

void Tesselation::setIsolated()
{
    setBorderLeft(true);
    setBorderRight(true);
    setBorderTop(true);
    setBorderBottom(true);
}

void Tesselation::setBorderLeft(bool border)
{
    is_border_left_ = border;
}

void Tesselation::setBorderRight(bool border)
{
    is_border_right_ = border;
}

void Tesselation::setBorderTop(bool border)
{
    is_border_top_ = border;
}

void Tesselation::setBorderBottom(bool border)
{
    is_border_bottom_ = border;
}

void Tesselation::setZFreeSeed(GLfloat seed_value)
{
    z_free_seed_ = seed_value;
}

GLfloat Tesselation::getZFree()
{
    return z_free_;
}

std::vector<GLfloat> Tesselation::getBottomRowBottomRightZ()
{
    return current_row_bottom_right_z_;
}

std::vector<GLfloat> Tesselation::getRightColumnBottomRightZ()
{
    return current_right_column_bottom_right_z_;
}

void Tesselation::setRandomisePeaks(bool peaks)
{
    randomise_peaks_ = peaks;
}

void Tesselation::setType(Type type)
{
    tesselation_type_ = type;
}

void Tesselation::draw()
{
    setActive();
    glDrawArrays(GL_TRIANGLES, 0, total_vertices_);
}