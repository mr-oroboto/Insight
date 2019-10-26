#ifndef INSIGHT_CORE_DISPLAYMANAGER_H
#define INSIGHT_CORE_DISPLAYMANAGER_H

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

#include "FrameQueue.h"
#include "primitive/PrimitiveCollection.h"

class DisplayManager
{
public:
    DisplayManager();
    ~DisplayManager();

    bool initialise(GLfloat wnd_width, GLfloat wnd_height);

    void setFrameQueue(FrameQueue* queue);
    void setCameraLocation(GLfloat x, GLfloat y, GLfloat z);
    void setPerspective(GLfloat near_plane, GLfloat far_plane, GLfloat fov);

    PrimitiveCollection* getPrimitiveCollection();

    GLuint getModelTransformUniform();
    GLuint getModelDoOverrideColourUniform();
    GLuint getModelOverrideColourUniform();

    void drawScene();
    void drawText(std::string text, glm::vec3 position, bool ortho = true, GLfloat scale = 1.0f, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));

private:
    struct Character
    {
        GLuint texture_id;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
    };

    bool initialiseFreeType();
    void teardown();
    bool isShaderInitialised(GLuint shader);

    PrimitiveCollection* primitives_;
    FrameQueue* frame_queue_;

    bool initialised_;

    GLuint shader_program_;
    GLuint vertex_shader_;
    GLuint fragment_shader_;

    GLuint uni_model_transform_;
    GLuint uni_view_transform_;
    GLuint uni_projection_transform_;
    GLuint uni_model_do_override_colour_;
    GLuint uni_model_override_colour_;

    GLfloat wnd_width_;
    GLfloat wnd_height_;
    glm::mat4 projection_transform_;
    glm::mat4 text_projection_transform_;

    GLfloat camera_x_;
    GLfloat camera_y_;
    GLfloat camera_z_;

    std::map<GLchar, Character> characters_;

    GLuint text_shader_program_;
    GLuint text_vertex_shader_;
    GLuint text_fragment_shader_;

    GLuint text_vao_;
    GLuint text_vbo_;

    GLuint uni_text_view_transform_;
    GLuint uni_text_projection_transform_;
    GLuint uni_text_colour_;
};

#endif //INSIGHT_CORE_DISPLAYMANAGER_H
