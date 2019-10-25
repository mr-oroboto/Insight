#ifndef INSIGHT_TESSELATION_H
#define INSIGHT_TESSELATION_H

#include "Primitive.h"
#include <vector>

class Tesselation : public Primitive
{
public:
    Tesselation(GLuint positionAttribute, GLuint colourAttribute);
    ~Tesselation();

    void setPreviousBottomRowBottomRightZ(std::vector<GLfloat> initialPreviousBottomRowBottomRightZ);
    void setPreviousRightColumnBottomRightZ(std::vector<GLfloat> initialPreviousRightColumnBottomRightZ);
    void setIsolated(bool iso);
    void resetSeamVertices();
    void setFreeZ(GLfloat val);

    void draw();

    std::vector<GLfloat> getBottomRowBottomRightZ();
    std::vector<GLfloat> getRightColumnBottomRightZ();
    GLfloat getFreeZ();

protected:
    void initVertices();

    GLfloat width;
    GLfloat length;
    GLfloat xIncrement;
    GLfloat yIncrement;

    GLfloat zMin;
    GLfloat zMax;
    GLfloat zFree;
    std::vector<GLfloat> prevRowBottomRightZ;
    std::vector<GLfloat> currentRowBottomRightZ;
    std::vector<GLfloat> prevRightColumnBottomRightZ;
    std::vector<GLfloat> currentRightColumnBottomRightZ;

    bool isolated;

    GLuint totalVertices;
    GLuint verticesPerSubprimitive;
    GLuint subprimitivesPerRow;
};

#endif //INSIGHT_TESSELATION_H
