#ifndef INSIGHT_CORE_DISPLAYMANAGER_H
#define INSIGHT_CORE_DISPLAYMANAGER_H

#include <map>
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

    void setFrameQueue(FrameQueue* queue);
    void setCameraCoords(const glm::vec3& world_coords);
    void setCameraUpVector(const glm::vec3& vector);
    void setCameraPointingVector(const glm::vec3& vector);
    void setLightingOn(bool on);
    void setLightCoords(const glm::vec3& world_coords);
    void setLightColour(const glm::vec3& colour, GLfloat intensity = 1.0);
    void setPerspective(GLfloat near_plane, GLfloat far_plane, GLfloat fov);
    void setUpdateSceneCallback(std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> callback);

    PrimitiveCollection* getPrimitiveCollection();
    TextureCollection* getTextureCollection();
    TextDrawer* getTextDrawer();
    StandardShader* getObjectShader();

    glm::vec3 getCameraPointingVector();
    glm::vec3 getCameraUpVector();

    glm::mat4 getViewTransform();
    glm::mat4 getProjectionTransform();

    void drawScene(GLfloat secs_since_rendering_started, GLfloat secs_since_last_renderloop);
    void drawText(const std::string& text, const glm::vec3& world_coords, bool ortho = true, GLfloat scale = 1.0f, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));

private:
    void teardown();

    bool initialised_;

    PrimitiveCollection* primitives_;
    TextureCollection* textures_;
    FrameQueue* frame_queue_;
    std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> update_scene_callback_;

    GLuint wnd_width_, wnd_height_;

    StandardShader* object_shader_;
    TextDrawer* text_drawer_;

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
