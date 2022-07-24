#include "GameMovementInputHandler.h"

#include "WindowManager.h"

namespace insight {

GameMovementInputHandler::GameMovementInputHandler()
{
    tracking_mouse_ = false;
    mouse_sensitivity_ = 0.001f;

    camera_speed_ = 10.0f;
    pitch_speed_ = 30.0f;
    yaw_speed_ = 30.0f;
}

void GameMovementInputHandler::resetCamera(const glm::vec3& camera_world_coords, const glm::vec3& up_vector, const glm::vec3& pointing_vector)
{
    camera_coords_ = camera_world_coords;
    camera_up_vector_ = up_vector;
    camera_pointing_vector_ = pointing_vector;

    // TODO: infer this from pointing_vector, otherwise it will be incorrect if z component is not -1
    camera_yaw_degrees_ = 270.0f;               // so that we're initially facing into screen (-z)
    camera_pitch_degrees_ = 0.0f;
}

void GameMovementInputHandler::handleKeystroke(insight::WindowManager *window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop)
{
    bool update_camera_coords = false;
    bool update_camera_pointing_vector = false;

    GLfloat camera_speed_increment = camera_speed_ * secs_since_last_renderloop;
    GLfloat pitch_speed_increment = pitch_speed_ * secs_since_last_renderloop;
    GLfloat yaw_speed_increment = yaw_speed_ * secs_since_last_renderloop;

    if (keystroke_event.type == SDL_KEYDOWN)
    {
        if (keystroke_event.key.keysym.sym == SDLK_w)
        {
            camera_coords_ += camera_speed_increment * camera_pointing_vector_;
            update_camera_coords = true;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_s)
        {
            camera_coords_ -= camera_speed_increment * camera_pointing_vector_;
            update_camera_coords = true;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_a)
        {
            // move left along normal to the camera direction and world up
            camera_coords_ -= glm::normalize(glm::cross(camera_pointing_vector_, camera_up_vector_)) * camera_speed_increment;
            update_camera_coords = true;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_d)
        {
            // move right along normal to camera direction and world up
            camera_coords_ += glm::normalize(glm::cross(camera_pointing_vector_, camera_up_vector_)) * camera_speed_increment;
            update_camera_coords = true;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_UP)
        {
            // adjust pitch (Euler angle around x-axis)
            camera_pitch_degrees_ += pitch_speed_increment;
            update_camera_pointing_vector = true;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_DOWN)
        {
            // adjust pitch (Euler angle around x-axis)
            camera_pitch_degrees_ -= pitch_speed_increment;
            update_camera_pointing_vector = true;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_LEFT)
        {
            // adjust yaw (Euler angle around y-axis)
            camera_yaw_degrees_ -= yaw_speed_increment;
            update_camera_pointing_vector = true;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_RIGHT)
        {
            // adjust yaw (Euler angle around y-axis)
            camera_yaw_degrees_ += yaw_speed_increment;
            update_camera_pointing_vector = true;
        }

        // clamp to sane values
        if (camera_pitch_degrees_ > 89.0f)
        {
            camera_pitch_degrees_ = 89.0f;
        }
        else if (camera_pitch_degrees_ < -89.0f)
        {
            camera_pitch_degrees_ = -89.0f;
        }
    }

    if (update_camera_coords)
    {
        window_manager->getDisplayManager()->setCameraCoords(camera_coords_);
    }

    if (update_camera_pointing_vector)
    {
        updateCameraPointingVector(window_manager->getDisplayManager());
    }
}

void GameMovementInputHandler::handleMouse(insight::WindowManager *window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop)
{
    if (mouse_event.type == SDL_MOUSEBUTTONDOWN && mouse_event.button.button == SDL_BUTTON_LEFT)
    {
        tracking_mouse_ = true;

        mouse_start_x_ = mouse_event.motion.x;
        mouse_start_y_ = mouse_event.motion.y;
    }
    else if (mouse_event.type == SDL_MOUSEBUTTONUP)
    {
        tracking_mouse_ = false;
    }
    else if (mouse_event.type == SDL_MOUSEMOTION && tracking_mouse_)
    {
        GLfloat mouse_diff_x = (mouse_start_x_ - mouse_event.motion.x) * -1.0f;     // flip left / right
        GLfloat mouse_diff_y = mouse_start_y_ - mouse_event.motion.y;

        camera_pitch_degrees_ += mouse_diff_y * mouse_sensitivity_;
        camera_yaw_degrees_ += mouse_diff_x * mouse_sensitivity_;

        updateCameraPointingVector(window_manager->getDisplayManager());
    }
}

void GameMovementInputHandler::updateCameraPointingVector(insight::DisplayManager* display_manager)
{
    camera_pointing_vector_.x = cos(glm::radians(camera_pitch_degrees_)) * cos(glm::radians(camera_yaw_degrees_));
    camera_pointing_vector_.y = sin(glm::radians(camera_pitch_degrees_));
    camera_pointing_vector_.z = cos(glm::radians(camera_pitch_degrees_)) * sin(glm::radians(camera_yaw_degrees_));
    camera_pointing_vector_ = glm::normalize(camera_pointing_vector_);

    display_manager->setCameraPointingVector(camera_pointing_vector_);
}

}   // namespace insight