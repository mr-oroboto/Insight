#ifndef INSIGHT_CORE_DISPLAYMANAGER_H
#define INSIGHT_CORE_DISPLAYMANAGER_H

#include <map>
#include <memory>
#include <string>
#include <functional>

#include <glm/glm.hpp>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

#include "FrameQueue.h"
#include "TextDrawer.h"
#include "primitive/PrimitiveCollection.h"
#include "shader/StandardShader.h"
#include "TextureCollection.h"

namespace insight {


class DisplayManager {
public:
    DisplayManager();
    ~DisplayManager();

    bool initialise(GLuint wnd_width, GLuint wnd_height);

    void resetCamera(const glm::vec3& camera_world_coords = glm::vec3(0, 5, 31), const glm::vec3& up_vector = glm::vec3(0.0f, 1.0f, 0.0f), const glm::vec3& pointing_vector = glm::vec3(0, 0, -1) /* pointing into screen */);

    void setFrameQueue(std::unique_ptr<FrameQueue> queue);
    void setCameraCoords(const glm::vec3& world_coords);
    void setCameraUpVector(const glm::vec3& vector);
    void setCameraPointingVector(const glm::vec3& vector);
    void setLightingOn(bool on);
    void toggleLighting();
    void setLightCoords(const glm::vec3& world_coords);
    void setLightColour(const glm::vec3& colour, GLfloat intensity = 1.0);
    void setPerspective(GLfloat near_plane, GLfloat far_plane, GLfloat fov);
    void setUpdateSceneCallback(std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> callback);
    void setResetCameraCallback(std::function<void(const glm::vec3&, const glm::vec3&, const glm::vec3&)> callback);

    bool registerFont(Font::Type font_type, const std::string& path);
    bool registerTexture(const std::string& path, const std::string& name);

    primitive::PrimitiveCollection* getPrimitiveCollection();
    shader::StandardShader* getObjectShader();

    glm::vec3 getCameraCoords();
    glm::vec3 getCameraPointingVector();
    glm::vec3 getCameraUpVector();

    // Generate a ray from the camera (origin) to a picked mouse position (ray casting).
    glm::vec3 getRayFromCamera(GLuint to_mouse_x, GLuint to_mouse_y);

    glm::mat4 getViewTransform();
    glm::mat4 getProjectionTransform();
    GLuint getWindowWidth();
    GLuint getWindowHeight();

    void drawScene(GLfloat secs_since_rendering_started, GLfloat secs_since_last_renderloop);
    void drawText(const std::string& text, const glm::vec3& world_coords, bool ortho = true, GLfloat scale = 1.0f, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));

private:
    friend class SceneObject;

    void teardown();
    Texture* getTexture(const std::string& texture_name);

    bool initialised_;

    primitive::PrimitiveCollection* primitives_;
    std::unique_ptr<TextureCollection> textures_;
    std::unique_ptr<FrameQueue> frame_queue_;
    std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> update_scene_callback_;

    GLuint wnd_width_, wnd_height_;

    shader::StandardShader* object_shader_;
    std::unique_ptr<TextDrawer> text_drawer_;

    glm::vec3 initial_camera_coords_;

    glm::vec3 camera_coords_;
    glm::vec3 camera_pointing_vector_;
    glm::vec3 camera_up_vector_;

    std::function<void(const glm::vec3&, const glm::vec3&, const glm::vec3&)> reset_camera_callback_;

    glm::mat4 projection_transform_;

    GLuint lighting_on_;
    glm::vec3 light_colour_;
    glm::vec3 light_coords_;
    GLfloat light_intensity_;
};


}   // namespace insight

#endif //INSIGHT_CORE_DISPLAYMANAGER_H
