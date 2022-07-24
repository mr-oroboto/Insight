#ifndef INSIGHT_CORE_WINDOWMANAGER_H
#define INSIGHT_CORE_WINDOWMANAGER_H

#include <chrono>
#include <vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

#include "DisplayManager.h"
#include "DefaultInputHandler.h"
#include "InputHandler.h"

namespace insight {


class WindowManager {
public:
    WindowManager(GLuint wnd_size_x, GLuint wnd_size_y, bool fullscreen = false);
    ~WindowManager();

    bool initialise();

    DisplayManager* getDisplayManager();
    bool run();
    void stop();

    void pushInputHandler(InputHandler* input_handler);
    void popInputHandler();

    // Called by DisplayManager when the camera is reset, allows us to propagate to input handlers.
    void resetCameraCallback(const glm::vec3& camera_world_coords, const glm::vec3& up_vector, const glm::vec3& pointing_vector);

private:
    void processEvents(GLfloat secs_since_last_renderloop);

    SDL_Window* window_;
    SDL_GLContext opengl_context_;
    bool initialised_;

    DisplayManager* display_manager_;
    GLuint wnd_size_x_, wnd_size_y_;
    GLuint wnd_pos_x_, wnd_pos_y_;
    bool fullscreen_;

    bool continue_rendering_;
    std::chrono::high_resolution_clock::time_point t_last_renderloop_at_;
    std::chrono::high_resolution_clock::time_point t_rendering_started_at_;

    std::vector<InputHandler*> input_handlers_;     // unowned and must outlive this class
    DefaultInputHandler* default_input_handler_;

    bool rotate_light_;             // does the light rotate around the origin? (helps to visualise lighting)
    glm::vec3 light_coords_;
    GLfloat light_radius_;
    GLfloat light_angle_degrees_;
};


}   // namespace insight

#endif //INSIGHT_CORE_WINDOWMANAGER_H
