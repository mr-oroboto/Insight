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
//      addObject(Primitive::Type::QUAD, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 20.0);
        drawTesselatedFloor();
    }

    if (drawReferenceAxes || true)
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

    bool firstRow = true;
    bool firstColumn = true;
    std::vector<GLfloat> prevBottomRowBottomRightZ;
    std::vector<GLfloat> prevRightColumnBottomRightZ;

    for (GLfloat y = 0.0f; y < 12.0f; y += 6.0)
    {
        firstColumn = true;

        for (GLfloat x = 0.0f; x < 12.0f; x += 6.0)
        {
            SceneObject* object = new SceneObject(displayManager, Primitive::Type::TESSELATION, glm::vec3(x, y, z),  glm::vec3(0.5, 0.5, 0.5));
            Tesselation* t = dynamic_cast<Tesselation*>(object->getPrimitive());
            t->resetSeamVertices();
            t->setFreeZ(0);
            t->setIsolated(false);

            if ( ! firstRow)
            {
                t->setPreviousBottomRowBottomRightZ(prevBottomRowBottomRightZ);
            }

            if ( ! firstColumn)
            {
                std::vector<GLfloat> temp = t->getRightColumnBottomRightZ();
                t->setFreeZ(temp[0]);
                t->setPreviousRightColumnBottomRightZ(prevRightColumnBottomRightZ);

            }

            prevBottomRowBottomRightZ = t->getBottomRowBottomRightZ();
            prevRightColumnBottomRightZ = t->getRightColumnBottomRightZ();

            object->draw(0, 0);

            firstColumn = false;
        }

        firstRow = false;
    }

    return;

    for (GLfloat y = 0.0f; y < 6.0f; y += 6.0)
    {
        for (GLfloat x = 0.0f; x < 6.0f; x += 6.0)
        {
            SceneObject* object = new SceneObject(displayManager, Primitive::Type::TESSELATION, glm::vec3(x, y, z),  glm::vec3(0.5, 0.5, 0.5));
            Tesselation* t = dynamic_cast<Tesselation*>(object->getPrimitive());
            t->setIsolated(true);
            object->draw(0, 0);
        }
    }
}