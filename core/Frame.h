#ifndef INSIGHT_CORE_FRAME_H
#define INSIGHT_CORE_FRAME_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "SceneObject.h"
#include "primitive/Primitive.h"

class DisplayManager;

class Frame
{
public:
    Frame(DisplayManager* dm, bool drawObjectPos = false, bool drawRefAxes = false, bool drawFloor = true);
    ~Frame();

    void addObject(Primitive::Type type, glm::vec3 worldPosition, glm::vec3 colour, GLfloat scale = 1.0f);
    void addText(std::string text, GLfloat x, GLfloat y, GLfloat z = 0.0f, bool ortho = true, GLfloat scale = 1.0f, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));
    void addLine(glm::vec3 from, glm::vec3 to, glm::vec3 colour);

    void draw(GLfloat secsSinceStart, GLfloat secsSinceLastFrame);

protected:
    void drawTesselatedFloor();
    void drawTesselatedFloorWithIsolatedTiles();

    struct Text
    {
        std::string text;
        glm::vec3 position;
        bool ortho;
        GLfloat scale;
        glm::vec3 colour;
    };

    bool drawObjectPositions;
    bool drawReferenceAxes;
    bool drawFloor;

    DisplayManager* displayManager;
    std::vector<SceneObject*> objects;
    std::vector<Text> texts;
};

#endif //INSIGHT_CORE_FRAME_H
