#include "Tesselation.h"

#include <cstring>
#include <iostream>
#include <random>

Tesselation::Tesselation(GLuint positionAttribute, GLuint colourAttribute)
{
    type = Primitive::TESSELATION;

    tesselationType = Type::FLAT;
    randomPeaks = false;

    isBorderLeft = true;
    isBorderRight = true;
    isBorderTop = true;
    isBorderBottom = true;

    xIncrement = 0.5;
    yIncrement = 0.5;
    zMin = 0.05;
    zMax = 0.1;
    zFreeSeed = zFree = 0.0;

    width = 6.0f;
    length = width;                     // DRAGON: don't change this, we assume the tesselation is a square

    verticesPerSubprimitive = 12;       // 4 triangles with open base

    totalVertices = static_cast<GLuint>((width / xIncrement) * (length / yIncrement) * verticesPerSubprimitive);
    vertices = new GLfloat[totalVertices * 8];
    std::cout << "creating " << totalVertices << " vertices for tesselation, allocated " << (sizeof(GLfloat) * totalVertices * 8) << " byte vertice buffer at " << vertices << std::endl;

    resetSeamVertices();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Must be done after glBindVertexArray() so the mapping is associated with the vao (and inherently the vbo)
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute,
                          3,                  /* num of values to read from array per vertex */
                          GL_FLOAT,           /* type of those values */
                          GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                          8 * sizeof(float),  /* stride: each (x,y,z) pos now has RGBUV data in between */
                          0                   /* offset */);

    glEnableVertexAttribArray(colourAttribute);
    glVertexAttribPointer(colourAttribute,
                          3,                         /* num of values to read from array per vertex */
                          GL_FLOAT,                  /* type of those values */
                          GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                          8 * sizeof(float),         /* stride: each colour block has (x,y,z) data in between */
                          (void*)(3 * sizeof(float)) /* offset: the colour block starts 3 floats (x,y,z) into the array */);

    initVertices();
}

Tesselation::~Tesselation()
{
    std::cout << "Tesselation::~Tesselation()" << std::endl;

    if (vertices != nullptr)
    {
        delete vertices;
    }
}

void Tesselation::resetSeamVertices()
{
    currentRowBottomRightZ.clear();
    prevRowBottomRightZ.clear();

    currentRightColumnBottomRightZ.clear();
    prevRightColumnBottomRightZ.clear();

    subprimitivesPerRow = 0;
    for (GLfloat x = 0.0f; x < width; x += xIncrement)
    {
        prevRowBottomRightZ.push_back(0);
        prevRightColumnBottomRightZ.push_back(0);           // assumes square tesselation

        currentRowBottomRightZ.push_back(0);
        currentRightColumnBottomRightZ.push_back(0);        // assumes sqaure tesselation

        subprimitivesPerRow++;
    }
}

void Tesselation::initVertices()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(zMin, zMax);

    GLuint currentVertices = 0;
    GLuint currentColumn = 0, currentRow = 0;
    GLfloat x, y;


    for (y = 0.0f; y < length && currentVertices < totalVertices; y += yIncrement)
    {
        currentColumn = 0;

        // Of the tile's four corners, we only get to choose the Z value of one of them (the bottom right corner), the
        // rest come from the fact the tesselation itself is a border tile on that corner (ie. z = 0), or the tesselation
        // is bordered by another tesselation, in which case we must smoothly flow on from that tesselation's values.
        //
        // All zFreeBottomRight values of the tile's left side start from the same seed value (which could also be from
        // a neighbouring tesselation).
        zFree = zFreeSeed;

        for (x = 0.0f; x < width && currentVertices < totalVertices; x += xIncrement)
        {
            GLfloat zFixedTopLeft, zFixedTopRight, zFixedBottomLeft, zFreeBottomRight, zPeak;

            // Set our only free height variable, the other corners come from previous tiles (in this, or a past tesselation if not isolated)
            switch (tesselationType)
            {
                case Type::HILLS:
                    zFree += 0.1;
                    zFreeBottomRight = sqrt(sin(zFree) * sin(zFree)) + sqrt(cos(zFree) * tan(zFree));
                    break;

                case Type::RAMPED:
                    zFree += 0.1;
                    zFreeBottomRight = zFree;
                    break;

                case Type::SIN_LENGTH:
                    zFree += 0.1;
                    zFreeBottomRight = sqrt(sin(zFree) * sin(zFree));
                    break;

                case Type::FLAT:
                default:
                    zFreeBottomRight = zFree;
                    break;
            }

            currentRowBottomRightZ[currentColumn] = zFreeBottomRight;

            if (currentColumn == 0)
            {
                /**
                 * First column, pin left side to 0 (or right column of previous tesselation)
                 */

                if (isBorderLeft)
                {
                    zFixedTopLeft = 0;
                    zFixedBottomLeft = 0;
                }
                else
                {
                    if (currentRow >= 1)
                    {
                        zFixedTopLeft = prevRightColumnBottomRightZ[currentRow - 1];
                    }
                    else
                    {
                        zFixedTopLeft = prevRightColumnBottomRightZ[currentRow];    // @todo, should be currentRow - 1
                    }

                    zFixedBottomLeft = prevRightColumnBottomRightZ[currentRow];
                }

                zFixedTopRight = prevRowBottomRightZ[currentColumn];
            }
            else if (currentColumn == subprimitivesPerRow - 1)
            {
                /**
                 * Last column, pin right side to 0 if isolated
                 */

                zFixedTopLeft = prevRowBottomRightZ[currentColumn - 1];
                zFixedBottomLeft = currentRowBottomRightZ[currentColumn - 1];

                if (isBorderRight)
                {
                    zFixedTopRight = 0;
                    zFreeBottomRight = 0;
                }
                else
                {
                    zFixedTopRight = prevRowBottomRightZ[currentColumn]; // zFreeBottomRight remains free (selected earlier)
                }

                currentRightColumnBottomRightZ[currentRow] = zFreeBottomRight;
            }
            else
            {
                zFixedTopLeft = prevRowBottomRightZ[currentColumn - 1];
                zFixedBottomLeft = currentRowBottomRightZ[currentColumn - 1];
                zFixedTopRight = prevRowBottomRightZ[currentColumn];
            }

            if (currentRow == 0)
            {
                /**
                 * First row, pin top to 0 if isolated
                 */

                if (isBorderTop)
                {
                    zFixedTopLeft = 0;
                    zFixedTopRight = 0;
                }
                else
                {
                    if (currentColumn >= 1)
                    {
                        zFixedTopLeft = prevRowBottomRightZ[currentColumn - 1];
                    }
                    else
                    {
                        zFixedTopLeft = prevRowBottomRightZ[currentColumn];     // @todo, should be currentColumn - 1
                    }

                    zFixedTopRight = prevRowBottomRightZ[currentColumn];
                }
            }

            if (currentRow == subprimitivesPerRow - 1)
            {
                /**
                 * Last row assuming we have a square tile, pin bottom to 0 if isolated
                 */

                if (isBorderBottom)                                         // no need to reset these if not isolated
                {
                    zFixedBottomLeft = 0;
                    zFreeBottomRight = 0;
                }
            }

            if (randomPeaks)
            {
                zPeak = zFixedTopLeft + distribution(gen);
            }
            else
            {
                zPeak = zFixedTopLeft;
            }

            GLfloat tileVertices[12 * 8] = {
                     // x            y                                       z            r     g     b     u     v
                     x + xIncrement,          y,                        zFixedTopRight,   1, 0, 0, 0.0f, 0.0f,  // +x face (left)
                     x + xIncrement,          y + yIncrement,           zFreeBottomRight, 1, 0, 0, 0.0f, 0.0f,
                     x + (xIncrement / 2.0f),  y + (yIncrement / 2.0f), zPeak,            1, 0, 0, 0.0f, 0.0f,

                     x + (xIncrement / 2.0f),  y + (yIncrement / 2.0f), zPeak,            0, 0, 1, 0.0f, 0.0f,  // +y face (facing camera)
                     x + xIncrement,          y + yIncrement,           zFreeBottomRight, 0, 0, 1, 0.0f, 0.0f,
                     x,                       y + yIncrement,           zFixedBottomLeft, 0, 0, 1, 0.0f, 0.0f,

                     x,                      y + yIncrement,            zFixedBottomLeft, 0, 1, 0, 0.0f, 0.0f,  // -x face (right)
                     x,                      y,                         zFixedTopLeft,    0, 1, 0, 0.0f, 0.0f,
                     x + (xIncrement / 2.0f), y + (yIncrement / 2.0f),  zPeak,            0, 1, 0, 0.0f, 0.0f,

                     x + (xIncrement / 2.0f), y + (yIncrement / 2.0f),  zPeak,            1, 0, 1, 0.0f, 0.0f,  // -y face
                     x,                      y,                         zFixedTopLeft,    1, 0, 1, 0.0f, 0.0f,
                     x + xIncrement,         y,                         zFixedTopRight,   1, 0, 1, 0.0f, 0.0f
            };

            memcpy(vertices + (currentVertices * 8), tileVertices, sizeof(GLfloat) * verticesPerSubprimitive * 8);
            currentVertices += verticesPerSubprimitive;

            currentColumn++;
        }

        prevRowBottomRightZ = currentRowBottomRightZ;
        currentRow++;
    }

    glBindVertexArray(vao);

    // Must be done after glBindVertexArray() so the vbo is associated with the vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * totalVertices * 8, vertices, GL_STATIC_DRAW);
}

void Tesselation::setPreviousBottomRowBottomRightZ(std::vector<GLfloat> initialPreviousBottomRowBottomRightZ)
{
    prevRowBottomRightZ = initialPreviousBottomRowBottomRightZ;
    setBorderTop(false);
}

void Tesselation::setPreviousRightColumnBottomRightZ(std::vector<GLfloat> initialPreviousRightColumnBottomRightZ)
{
    prevRightColumnBottomRightZ = initialPreviousRightColumnBottomRightZ;
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
    isBorderLeft = border;
}

void Tesselation::setBorderRight(bool border)
{
    isBorderRight = border;
}

void Tesselation::setBorderTop(bool border)
{
    isBorderTop = border;
}

void Tesselation::setBorderBottom(bool border)
{
    isBorderBottom = border;
}

void Tesselation::setZFreeSeed(GLfloat val)
{
    zFreeSeed = val;
}

GLfloat Tesselation::getZFree()
{
    return zFree;
}

std::vector<GLfloat> Tesselation::getBottomRowBottomRightZ()
{
    return currentRowBottomRightZ;
}

std::vector<GLfloat> Tesselation::getRightColumnBottomRightZ()
{
    return currentRightColumnBottomRightZ;
}

void Tesselation::setRandomPeaks(bool peaks)
{
    randomPeaks = peaks;
}

void Tesselation::setType(Type type)
{
    tesselationType = type;
}

void Tesselation::draw()
{
    setActive();
    glDrawArrays(GL_TRIANGLES, 0, totalVertices);
}