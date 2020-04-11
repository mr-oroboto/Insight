#ifndef INSIGHT_CORE_FRAME_H
#define INSIGHT_CORE_FRAME_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "SceneObject.h"
#include "primitive/Primitive.h"

namespace insight {


class DisplayManager;

class Frame {
public:
    Frame(DisplayManager* display_manager, bool draw_object_position = false, bool draw_reference_axes = false, bool draw_floor = false);
    ~Frame();

    void addObject(primitive::Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour, const std::string& texture_name = "", GLfloat scale = 1.0f);
    void addObject(SceneObject* object);
    unsigned long addText(const std::string& text, GLfloat x, GLfloat y, GLfloat z = 0.0f, bool ortho = true, GLfloat scale = 1.0f, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));
    void addLine(const glm::vec3& from_world_coords, const glm::vec3& to_world_coords, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));

    void draw(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame);
    void updateObjects(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, void* context);

    std::shared_ptr<Frame> clone();     // prefer to copy ctor so subclasses of Frame keep control

    GLuint deleteObjectsAtPosition(const glm::vec3& world_coords, primitive::Primitive::Type primitive_type);
    GLuint deleteObjectsOutsideBoundary(const glm::vec3 &world_coords, GLfloat bounding_width, primitive::Primitive::Type primitive_type);
    void deleteText(unsigned long text_id);
    bool coordinatesAreOutsideBoundary(const glm::vec3& world_coords, const glm::vec3& reference_coords, GLfloat bounding_width);

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
    unsigned long next_text_id_;
    std::unordered_map<unsigned long, Text> texts_;
};


}   // namespace insight

#endif //INSIGHT_CORE_FRAME_H
