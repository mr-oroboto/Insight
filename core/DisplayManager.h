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

class DisplayManager
{
public:
    DisplayManager();
    ~DisplayManager();

    bool initialise(GLuint wnd_width, GLuint wnd_height);

    void setFrameQueue(std::unique_ptr<FrameQueue> queue);
    void setCameraCoords(const glm::vec3& world_coords);
    void setCameraUpVector(const glm::vec3& vector);
    void setCameraPointingVector(const glm::vec3& vector);
    void setLightingOn(bool on);
    void setLightCoords(const glm::vec3& world_coords);
    void setLightColour(const glm::vec3& colour, GLfloat intensity = 1.0);
    void setPerspective(GLfloat near_plane, GLfloat far_plane, GLfloat fov);
    void setUpdateSceneCallback(std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> callback);

    bool registerFont(Font::Type font_type, const std::string& path);
    bool registerTexture(const std::string& path, const std::string& name);

    PrimitiveCollection* getPrimitiveCollection();
    StandardShader* getObjectShader();

    glm::vec3 getCameraCoords();
    glm::vec3 getCameraPointingVector();
    glm::vec3 getCameraUpVector();
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

    PrimitiveCollection* primitives_;
    std::unique_ptr<TextureCollection> textures_;
    std::unique_ptr<FrameQueue> frame_queue_;
    std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> update_scene_callback_;

    GLuint wnd_width_, wnd_height_;

    StandardShader* object_shader_;
    std::unique_ptr<TextDrawer> text_drawer_;

    glm::vec3 camera_coords_;
    glm::vec3 camera_pointing_vector_;
    glm::vec3 camera_up_vector_;

    glm::mat4 projection_transform_;

    GLuint lighting_on_;
    glm::vec3 light_colour_;
    glm::vec3 light_coords_;
    GLfloat light_intensity_;
};

#endif //INSIGHT_CORE_DISPLAYMANAGER_H
