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
    Frame(DisplayManager* display_manager, bool draw_object_position = false, bool draw_reference_axes = false, bool draw_floor = true);
    ~Frame();

    void addObject(Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour, GLfloat scale = 1.0f);
    void addText(const std::string& text, GLfloat x, GLfloat y, GLfloat z = 0.0f, bool ortho = true, GLfloat scale = 1.0f, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));
    void addLine(const glm::vec3& from_world_coords, const glm::vec3& to_world_coords, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));

    void draw(GLfloat secs_since_start, GLfloat secs_since_last_frame);

private:
    struct Text
    {
        std::string text;
        glm::vec3 position;
        bool ortho;
        GLfloat scale;
        glm::vec3 colour;
    };

    void drawTesselatedFloor();
    void drawTesselatedFloorWithIsolatedTiles();

    bool draw_object_positions_;
    bool draw_reference_axes_;
    bool draw_floor_;

    DisplayManager* display_manager_;
    std::vector<SceneObject*> objects_;
    std::vector<Text> texts_;
};

#endif //INSIGHT_CORE_FRAME_H
