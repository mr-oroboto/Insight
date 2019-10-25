#include "Frame.h"
#include <iostream>
#include <math.h>
#include "primitive/Line.h"
#include "primitive/Tesselation.h"

Frame::Frame(DisplayManager* dm, bool drawObjectPos, bool drawRefAxes, bool drawGround)
{
    displayManager = dm;

    drawObjectPositions = drawObjectPos;
    drawReferenceAxes = drawRefAxes;
    drawFloor = drawGround;
}

Frame::~Frame()
{
    std::cout << "Frame::~Frame()" << std::endl;

    // Delete all the scene objects that belong to this frame.
    for (SceneObject* object : objects)
    {
        delete object;
    }
}

void Frame::addObject(Primitive::Type type, glm::vec3 worldPosition, glm::vec3 colour, GLfloat scale)
{
    SceneObject* object = new SceneObject(displayManager, type, worldPosition, colour);
    object->setScale(scale);
    objects.push_back(object);
}

void Frame::addLine(glm::vec3 from, glm::vec3 to, glm::vec3 colour)
{
    SceneObject* object = new SceneObject(displayManager, Primitive::Type::LINE, from, colour);
//    object->setScale(sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2) + pow(from.z - to.z, 2)));
    object->setAdditionalCoords(to);
    objects.push_back(object);
}

void Frame::addText(std::string text, GLfloat x, GLfloat y, GLfloat z, bool ortho, GLfloat scale, glm::vec3 colour)
{
    glm::vec3 position = glm::vec3(x, y, z);
    Text textObj {
            text,
            position,
            ortho,
            scale,
            colour
    };

    texts.push_back(textObj);
}

void Frame::draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame)
{
    if (drawFloor)
    {
        drawTesselatedFloor();
    }

    if (drawReferenceAxes)
    {
        glm::vec3 colour = glm::vec3(1);
        glm::vec3 from = glm::vec3(0, 0, 0);

        glm::vec3 to = glm::vec3(10, 0, 0);
        addLine(from, to, colour);

        to = glm::vec3(0, 10, 0);
        addLine(from, to, colour);

        to = glm::vec3(0, 0, 10);
        addLine(from, to, colour);

        addObject(Primitive::Type::CUBE, glm::vec3(0, 0, 0),   glm::vec3(1, 1, 1), 0.5);
        addObject(Primitive::Type::CUBE, glm::vec3(-10, 0, 0), glm::vec3(1, 0, 0), 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(10, 0, 0),  glm::vec3(0, 0, 1), 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, -10, 0), glm::vec3(1, 0, 0), 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, 10, 0),  glm::vec3(1, 1, 1), 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, 0, -10), glm::vec3(1, 0, 0), 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, 0, 10),  glm::vec3(1, 1, 0), 0.2);
    }

    // We must first render all objects before we render any text
    for (SceneObject* object : objects)
    {
        object->draw(secsSinceStart, secsSinceLastFrame);
    }

    // Now text can be rendered
    if (drawObjectPositions)
    {
        for (SceneObject* object : objects)
        {
            char msg[64];
            glm::vec3 textColour = glm::vec3(1.0, 1.0, 0.0);
            glm::vec3 objectPosition = object->getPosition();
            snprintf(msg, sizeof(msg), "(%.1f,%.1f,%.1f)", objectPosition.x, objectPosition.y, objectPosition.z);
            displayManager->drawText(msg, objectPosition, false, 0.015, textColour);
        }
    }

    for (Text text : texts)
    {
        displayManager->drawText(text.text, text.position, text.ortho, text.scale, text.colour);
    }
}

/**
 * Each tesselation tile must be drawn as it is created because its vertices change with each previous tile.
 */
void Frame::drawTesselatedFloor()
{
    GLfloat z = -3.0f;

    GLfloat floorWidth = 12.0;      // y-dimension
    GLfloat floorLength = 12.0;     // x-dimension
    GLfloat tileDimension = 6.0;    // assumes square tiles
    GLfloat x, y;
    GLuint tilesLong = floorLength / tileDimension;    // how many tiles wide (in y-dimension) is this surface?
    GLuint tilesWide = floorWidth / tileDimension;

    std::vector<std::vector<GLfloat>> prevBottomRowBottomRightZ(tilesWide);
    std::vector<GLfloat> prevRightColumnBottomRightZ;
    GLuint currentColumn = 0;
    GLuint currentRow = 0;
    GLfloat lastZFree;

    for (y = 0.0f; y < floorWidth; y += tileDimension)
    {
        currentColumn = 0;

        for (x = 0.0f; x < floorLength; x += tileDimension)
        {
            SceneObject* object = new SceneObject(displayManager, Primitive::Type::TESSELATION, glm::vec3(x, y, z),  glm::vec3(1, 1, 1));
            Tesselation* t = dynamic_cast<Tesselation*>(object->getPrimitive());

            t->setRandomPeaks(true);
            t->setType(Tesselation::Type::HILLS);
            t->setZFreeSeed(0);
            t->resetSeamVertices();
            t->initVertices();

            t->setBorderRight(false);       // we're growing the tesselation from all tiles
            t->setBorderBottom(false);      // we're growing the tesselation from all tiles
            t->setBorderTop(true);          // assume we're a border on the other edges unless reset by setPrevious...
            t->setBorderLeft(true);

            if (currentRow != 0)
            {
                t->setPreviousBottomRowBottomRightZ(prevBottomRowBottomRightZ[currentColumn]);
            }

            if (currentColumn != 0)
            {
//              t->setZFreeSeed(prevRightColumnBottomRightZ[0]);
                t->setZFreeSeed(lastZFree);
                t->setPreviousRightColumnBottomRightZ(prevRightColumnBottomRightZ);
            }

            if (currentColumn == tilesLong - 1)
            {
                t->setBorderRight(true);
            }

            if (currentRow == tilesWide - 1)
            {
                t->setBorderBottom(true);
            }

            t->initVertices();

            prevBottomRowBottomRightZ[currentColumn] = t->getBottomRowBottomRightZ();
            prevRightColumnBottomRightZ = t->getRightColumnBottomRightZ();
            lastZFree = t->getZFree();

            object->draw(0, 0, false);

            delete object;

            currentColumn++;
        }

        currentRow++;
    }

    return;
}

void Frame::drawTesselatedFloorWithIsolatedTiles()
{
    GLfloat z = -3.0;

    GLfloat floorWidth = 12.0;      // y-dimension
    GLfloat floorLength = 12.0;     // x-dimension
    GLfloat tileDimension = 6.0;    // assumes square tiles
    GLfloat x, y;

    for (y = 0.0f; y < floorWidth; y += tileDimension)
    {
        for (x = 0.0f; x < floorLength; x += tileDimension)
        {
            SceneObject* object = new SceneObject(displayManager, Primitive::Type::TESSELATION, glm::vec3(x, y, z),  glm::vec3(0.5, 0.5, 0.5));
            Tesselation* t = dynamic_cast<Tesselation*>(object->getPrimitive());

            t->setRandomPeaks(false);
            t->setZFreeSeed(0);
            t->setType(Tesselation::Type::RAMPED);
            t->setIsolated();
            t->initVertices();

            object->draw(0, 0, false);

            delete object;
        }
    }
}