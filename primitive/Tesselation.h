#ifndef INSIGHT_TESSELATION_H
#define INSIGHT_TESSELATION_H

#include "Primitive.h"
#include <vector>

class Tesselation : public Primitive
{
public:
    Tesselation(GLuint positionAttribute, GLuint colourAttribute);
    ~Tesselation();

    enum Type
    {
        FLAT = 0,
        RAMPED,
        SIN_LENGTH,
        HILLS
    };

    void setIsolated();
    void setBorderTop(bool border);
    void setBorderBottom(bool border);
    void setBorderLeft(bool border);
    void setBorderRight(bool border);
    void setZFreeSeed(GLfloat val);

    void setPreviousBottomRowBottomRightZ(std::vector<GLfloat> initialPreviousBottomRowBottomRightZ);
    void setPreviousRightColumnBottomRightZ(std::vector<GLfloat> initialPreviousRightColumnBottomRightZ);

    void resetSeamVertices();
    void initVertices();

    void setRandomPeaks(bool peaks);
    void setType(Type type);

    void draw();

    std::vector<GLfloat> getBottomRowBottomRightZ();
    std::vector<GLfloat> getRightColumnBottomRightZ();
    GLfloat getZFree();

protected:
    Type tesselationType;

    GLfloat width;
    GLfloat length;
    GLfloat xIncrement;
    GLfloat yIncrement;

    GLfloat zMin;
    GLfloat zMax;
    GLfloat zFreeSeed;
    GLfloat zFree;
    std::vector<GLfloat> prevRowBottomRightZ;
    std::vector<GLfloat> currentRowBottomRightZ;
    std::vector<GLfloat> prevRightColumnBottomRightZ;
    std::vector<GLfloat> currentRightColumnBottomRightZ;

    bool isBorderLeft;
    bool isBorderRight;
    bool isBorderTop;
    bool isBorderBottom;
    bool randomPeaks;

    GLuint totalVertices;
    GLuint verticesPerSubprimitive;
    GLuint subprimitivesPerRow;
};

#endif //INSIGHT_TESSELATION_H
