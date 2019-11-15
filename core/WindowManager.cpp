#include "WindowManager.h"

#include <chrono>
#include <cmath>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

WindowManager::WindowManager(GLuint wnd_size_x, GLuint wnd_size_y, bool fullscreen, const glm::vec3& initial_camera_coords)
{
    initialised_ = false;

    wnd_size_x_ = wnd_size_x;
    wnd_size_y_ = wnd_size_y;
    wnd_pos_x_ = wnd_pos_y_ = 10;
    fullscreen_ = fullscreen;

    tracking_mouse_ = false;
    mouse_sensitivity_ = 0.001f;

    window_ = nullptr;
    display_manager_ = nullptr;

    t_last_renderloop_at_ = std::chrono::high_resolution_clock::now();

    camera_speed_ = 10.0f;
    pitch_speed_ = 30.0f;
    yaw_speed_ = 30.0f;

    camera_coords_ = initial_camera_coords;
    camera_pointing_vector_ = glm::vec3(0, 0, -1);    // facing into screen
    camera_pitch_degrees_ = 0.0f;
    camera_yaw_degrees_ = 270.0f;               // so that we're initially facing into screen (-z)

    lighting_on_ = true;
    rotate_light_ = true;
    light_coords_ = glm::vec3(5, 5, 3);
    light_radius_ = 10.0f;
    light_angle_degrees_ = 0.0f;
}

WindowManager::~WindowManager()
{
    if (display_manager_)
    {
        delete display_manager_;
    }

    if (initialised_)
    {
        SDL_GL_DeleteContext(opengl_context_);
    }

    SDL_Quit();
}

bool WindowManager::initialise()
{
    if (initialised_)
    {
        return true;
    }

    // SDL is used to abstract the creation of an X window and event loop etc
    SDL_Init(SDL_INIT_EVERYTHING);

    // Before we can create the OpenGL context we hint that we want a forward compatible OpenGL 3.2 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);                // create a stencil buffer

    // Create a window (abstracted, uses the right underlying APIs whether on Windows, Mac or Linux)
    if ( ! fullscreen_)
    {
        window_ = SDL_CreateWindow("OpenGL", wnd_pos_x_, wnd_pos_y_, wnd_size_x_, wnd_size_y_, SDL_WINDOW_OPENGL);
    }
    else
    {
        window_ = SDL_CreateWindow("OpenGL", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN);
    }

    if ( ! window_)
    {
        return false;
    }

    // Create the OpenGL context
    opengl_context_ = SDL_GL_CreateContext(window_);

    // Now that context is created we can use GLEW to register / import all available OpenGL API calls
    glewExperimental = GL_TRUE;
    glewInit();

    display_manager_ = new DisplayManager();
    if ( ! display_manager_->initialise(wnd_size_x_, wnd_size_y_))
    {
        return false;
    }

    initialised_ = true;

    return true;
}

DisplayManager* WindowManager::getDisplayManager()
{
    return display_manager_;
}

bool WindowManager::run()
{
    if ( ! initialise())
    {
        return false;
    }

    t_rendering_started_at_ = t_last_renderloop_at_ =  std::chrono::high_resolution_clock::now();

    display_manager_->setCameraCoords(camera_coords_);
    display_manager_->setCameraPointingVector(camera_pointing_vector_);
    display_manager_->setLightCoords(light_coords_);
    display_manager_->setLightColour(glm::vec3(1, 1, 1), 0.5);

    bool continue_rendering = true;

    while (continue_rendering)
    {
        auto t_now = std::chrono::high_resolution_clock::now();

        GLfloat secs_since_last_renderloop = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - t_last_renderloop_at_).count();
        GLfloat secs_since_rendering_started = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - t_rendering_started_at_).count();
        t_last_renderloop_at_ = t_now;

        continue_rendering = processEvents(secs_since_last_renderloop);

        if (rotate_light_)
        {
            light_angle_degrees_ += 0.1;

            light_coords_.x = light_radius_ * cos(light_angle_degrees_ / (2*M_PI));
            light_coords_.z = light_radius_ * sin(light_angle_degrees_ / (2*M_PI));

            display_manager_->setLightCoords(light_coords_);
        }

        display_manager_->drawScene(secs_since_rendering_started, secs_since_last_renderloop);

        // swap the (presumably prepared elsewhere) back-buffer into the front-buffer
        SDL_GL_SwapWindow(window_);
    }

    return true;
}

bool WindowManager::processEvents(GLfloat secs_since_last_renderloop)
{
    bool continue_rendering = true;
    bool update_camera = false;

    SDL_Event window_event;

    GLfloat camera_speed_increment = camera_speed_ * secs_since_last_renderloop;
    GLfloat pitch_speed_increment = pitch_speed_ * secs_since_last_renderloop;
    GLfloat yaw_speed_increment = yaw_speed_ * secs_since_last_renderloop;

    while (SDL_PollEvent(&window_event))                // gather clicks, keystrokes, window movements, resizes etc
    {
        if (window_event.type == SDL_QUIT)
        {
            continue_rendering = false;
            break;
        }

        if (fullscreen_ && window_event.type == SDL_KEYUP && window_event.key.keysym.sym == SDLK_ESCAPE)
        {
            continue_rendering = false;
            break;
        }

        if (window_event.type == SDL_MOUSEBUTTONDOWN && window_event.button.button == SDL_BUTTON_LEFT)
        {
            tracking_mouse_ = true;

            mouse_start_x_ = window_event.motion.x;
            mouse_start_y_ = window_event.motion.y;
        }
        else if (window_event.type == SDL_MOUSEBUTTONUP)
        {
            tracking_mouse_ = false;
        }

        if (window_event.type == SDL_MOUSEMOTION && tracking_mouse_)
        {
            GLfloat mouse_diff_x = (mouse_start_x_ - window_event.motion.x) * -1.0f;     // flip left / right
            GLfloat mouse_diff_y = mouse_start_y_ - window_event.motion.y;

            camera_pitch_degrees_ += mouse_diff_y * mouse_sensitivity_;
            camera_yaw_degrees_ += mouse_diff_x * mouse_sensitivity_;

            update_camera = true;
        }

        if (window_event.type == SDL_KEYDOWN)
        {
            if (window_event.key.keysym.sym == SDLK_w)
            {
                camera_coords_ += camera_speed_increment * display_manager_->getCameraPointingVector();
            }
            else if (window_event.key.keysym.sym == SDLK_s)
            {
                camera_coords_ -= camera_speed_increment * display_manager_->getCameraPointingVector();
            }
            else if (window_event.key.keysym.sym == SDLK_a)
            {
                // move left along normal to the camera direction and world up
                camera_coords_ -= glm::normalize(glm::cross(display_manager_->getCameraPointingVector(), display_manager_->getCameraUpVector())) * camera_speed_increment;
            }
            else if (window_event.key.keysym.sym == SDLK_d)
            {
                // move right along normal to camera direction and world up
                camera_coords_ += glm::normalize(glm::cross(display_manager_->getCameraPointingVector(), display_manager_->getCameraUpVector())) * camera_speed_increment;
            }
            else if (window_event.key.keysym.sym == SDLK_UP)
            {
                // adjust pitch (Euler angle around x-axis)
                camera_pitch_degrees_ += pitch_speed_increment;
            }
            else if (window_event.key.keysym.sym == SDLK_DOWN)
            {
                // adjust pitch (Euler angle around x-axis)
                camera_pitch_degrees_ -= pitch_speed_increment;
            }
            else if (window_event.key.keysym.sym == SDLK_LEFT)
            {
                // adjust yaw (Euler angle around y-axis)
                camera_yaw_degrees_ -= yaw_speed_increment;
            }
            else if (window_event.key.keysym.sym == SDLK_RIGHT)
            {
                // adjust yaw (Euler angle around y-axis)
                camera_yaw_degrees_ += yaw_speed_increment;
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

            update_camera = true;
        }
        else if (window_event.type == SDL_KEYUP)
        {
            if (window_event.key.keysym.sym == SDLK_q)
            {
                continue_rendering = false;
                break;
            }
            else if (window_event.key.keysym.sym == SDLK_l)
            {
                lighting_on_ = ! lighting_on_;
                display_manager_->setLightingOn(lighting_on_);
            }
        }
    }

    if (update_camera)
    {
        camera_pointing_vector_.x = cos(glm::radians(camera_pitch_degrees_)) * cos(glm::radians(camera_yaw_degrees_));
        camera_pointing_vector_.y = sin(glm::radians(camera_pitch_degrees_));
        camera_pointing_vector_.z = cos(glm::radians(camera_pitch_degrees_)) * sin(glm::radians(camera_yaw_degrees_));
        camera_pointing_vector_ = glm::normalize(camera_pointing_vector_);

        display_manager_->setCameraCoords(camera_coords_);
        display_manager_->setCameraPointingVector(camera_pointing_vector_);
    }

    return continue_rendering;
}