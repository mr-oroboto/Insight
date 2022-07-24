#ifndef INSIGHT_CORE_GAMEMOVEMENTINPUTHANDLER_H
#define INSIGHT_CORE_GAMEMOVEMENTINPUTHANDLER_H

#include <iostream>

#include "InputHandler.h"

#include <glm/glm.hpp>

namespace insight {

class DisplayManager;

class GameMovementInputHandler : public InputHandler {
public:
    GameMovementInputHandler();
    ~GameMovementInputHandler() { std::cout << "Destroying GameMovementInputHandler" << std::endl; }

    void resetCamera(const glm::vec3& camera_world_coords, const glm::vec3& up_vector, const glm::vec3& pointing_vector);
    void handleKeystroke(insight::WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop);
    void handleMouse(insight::WindowManager* window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop);

private:
    void updateCameraPointingVector(insight::DisplayManager* display_manager);

    bool tracking_mouse_;
    GLfloat mouse_start_x_, mouse_start_y_;
    GLfloat mouse_sensitivity_;

    // These are copies of values sourced from DisplayManager (set via resetCamera).
    glm::vec3 camera_coords_;
    glm::vec3 camera_pointing_vector_;
    glm::vec3 camera_up_vector_;

    GLfloat camera_pitch_degrees_;
    GLfloat camera_yaw_degrees_;

    GLfloat camera_speed_;
    GLfloat pitch_speed_;           // how fast camera pitches (degrees / sec)
    GLfloat yaw_speed_;             // how fast camera yaws (degrees / sec)
};

}   // namespace insight

#endif //INSIGHT_CORE_GAMEMOVEMENTINPUTHANDLER_H
