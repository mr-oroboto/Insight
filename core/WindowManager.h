#ifndef INSIGHT_CORE_WINDOWMANAGER_H
#define INSIGHT_CORE_WINDOWMANAGER_H

#include <chrono>

#include <SDL2/SDL.h>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

#include "DisplayManager.h"

class WindowManager
{
public:
    WindowManager(GLuint wnd_size_x, GLuint wnd_size_y, bool fullscreen = false, const glm::vec3& initial_camera_coords = glm::vec3(0, 5, 31));
    ~WindowManager();

    bool initialise();

    void setHandleKeystrokeCallback(std::function<bool(WindowManager*, SDL_Event, GLfloat)> callback);
    void resetCamera();

    DisplayManager* getDisplayManager();
    bool run();

private:
    bool processEvents(GLfloat secs_since_last_renderloop);
    bool handleKeystroke(SDL_Event keystroke_event, GLfloat secs_since_last_renderloop, bool& update_camera);

    SDL_Window* window_;
    SDL_GLContext opengl_context_;
    bool initialised_;

    DisplayManager* display_manager_;
    GLuint wnd_size_x_, wnd_size_y_;
    GLuint wnd_pos_x_, wnd_pos_y_;
    bool fullscreen_;

    bool tracking_mouse_;
    GLfloat mouse_start_x_, mouse_start_y_;
    GLfloat mouse_sensitivity_;
    std::function<bool(WindowManager*, SDL_Event, GLfloat)> handle_keystroke_callback_;

    std::chrono::high_resolution_clock::time_point t_last_renderloop_at_;
    std::chrono::high_resolution_clock::time_point t_rendering_started_at_;

    glm::vec3 camera_coords_;
    glm::vec3 initial_camera_coords_;
    glm::vec3 camera_pointing_vector_;
    GLfloat camera_pitch_degrees_;
    GLfloat camera_yaw_degrees_;

    GLfloat camera_speed_;
    GLfloat pitch_speed_;           // how fast camera pitches (degrees / sec)
    GLfloat yaw_speed_;             // how fast camera yaws (degrees / sec)

    bool lighting_on_;
    bool rotate_light_;             // does the light rotate around the origin? (helps to visualise lighting)
    glm::vec3 light_coords_;
    GLfloat light_radius_;
    GLfloat light_angle_degrees_;
};

#endif //INSIGHT_CORE_WINDOWMANAGER_H
