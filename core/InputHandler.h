#ifndef INSIGHT_CORE_INPUT_HANDLER_H
#define INSIGHT_CORE_INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

namespace insight {

class WindowManager;

class InputHandler {
public:
    virtual ~InputHandler() {};

    virtual void resetCamera(const glm::vec3& camera_world_coords, const glm::vec3& up_vector, const glm::vec3& pointing_vector) {};
    virtual void handleKeystroke(insight::WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop) = 0;
    virtual void handleMouse(insight::WindowManager* window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop) = 0;
};

}    // namespace insight

#endif //INSIGHT_CORE_INPUT_HANDLER_H
