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
    typedef void (*UpdateSceneCallback)(GLfloat);

    DisplayManager();
    ~DisplayManager();

    bool initialise(GLuint wnd_width, GLuint wnd_height);

    void setFrameQueue(FrameQueue* queue);
    void setCameraCoords(const glm::vec3& world_coords, const glm::vec3& camera_direction_vector = glm::vec3(0, 0, -1));
    void setLightingOn(bool on);
    void setLightCoords(const glm::vec3& world_coords);
    void setLightColour(const glm::vec3& colour, GLfloat intensity = 1.0);
    void setPerspective(GLfloat near_plane, GLfloat far_plane, GLfloat fov);
    void setUpdateSceneCallback(std::function<void(GLfloat)> callback);

    PrimitiveCollection* getPrimitiveCollection();
    TextureCollection* getTextureCollection();
    StandardShader* getObjectShader();

    glm::vec3 getCameraDirectionVector();
    glm::vec3 getCameraUpVector();

    glm::mat4 getViewTransform();
    glm::mat4 getProjectionTransform();

    void drawScene(GLfloat secs_since_last_frame);
    void drawText(const std::string& text, const glm::vec3& world_coords, bool ortho = true, GLfloat scale = 1.0f, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));

private:
    void teardown();

    bool initialised_;

    PrimitiveCollection* primitives_;
    TextureCollection* textures_;
    FrameQueue* frame_queue_;
    std::function<void(GLfloat)> update_scene_callback_;

    GLuint wnd_width_, wnd_height_;

    StandardShader* object_shader_;
    TextDrawer* text_drawer_;

    glm::vec3 camera_coords_;
    glm::vec3 camera_direction_vector_;

    glm::mat4 projection_transform_;

    GLuint lighting_on_;
    glm::vec3 light_colour_;
    glm::vec3 light_coords_;
    GLfloat light_intensity_;
};

#endif //INSIGHT_CORE_DISPLAYMANAGER_H
