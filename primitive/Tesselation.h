#ifndef INSIGHT_PRIMITIVE_TESSELATION_H
#define INSIGHT_PRIMITIVE_TESSELATION_H

#include <vector>

#include <GL/glew.h>

#include "Primitive.h"

class Tesselation : public Primitive
{
public:
    Tesselation(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute);
    ~Tesselation();

    enum Type
    {
        FLAT = 0,
        RAMPED,
        SIN_LENGTH,
        HILLS,
        RANDOM
    };

    void setIsolated();
    void setBorderTop(bool border);
    void setBorderBottom(bool border);
    void setBorderLeft(bool border);
    void setBorderRight(bool border);
    void setYFreeSeed(GLfloat seed_value);

    void setPreviousBottomRowBottomRightY(const std::vector<GLfloat>& initial_previous_bottom_row_bottom_right_y);
    void setPreviousRightColumnBottomRightY(const std::vector<GLfloat>& initial_previous_right_column_bottom_right_y);

    void resetSeamVertices();
    void initVertices();

    void setRandomisePeaks(bool peaks);
    void setType(Type type);

    void draw();

    std::vector<GLfloat> getBottomRowBottomRightY();
    std::vector<GLfloat> getRightColumnBottomRightY();
    GLfloat getYFree();

protected:
    Type tesselation_type_;

    GLfloat width_;
    GLfloat length_;
    GLfloat x_increment_;
    GLfloat z_increment_;

    GLfloat y_min_;
    GLfloat y_max_;
    GLfloat y_free_seed_;
    GLfloat y_free_;
    std::vector<GLfloat> previous_row_bottom_right_y_;
    std::vector<GLfloat> current_row_bottom_right_y_;
    std::vector<GLfloat> previous_right_column_bottom_right_y_;
    std::vector<GLfloat> current_right_column_bottom_right_y_;
    std::vector<GLfloat> random_height_seeds_;

    bool is_border_left_;
    bool is_border_right_;
    bool is_border_top_;
    bool is_border_bottom_;
    bool randomise_peaks_;

    GLuint total_vertices_;
    GLuint vertices_per_subtile_;
    GLuint sub_tiles_per_row_;
};

#endif //INSIGHT_PRIMITIVE_TESSELATION_H
