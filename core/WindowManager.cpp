#include "WindowManager.h"

#include <cmath>
#include <iostream>

#include <SDL2/SDL_mouse.h>

#include "DefaultInputHandler.h"

namespace insight {


WindowManager::WindowManager(GLuint wnd_size_x, GLuint wnd_size_y, bool fullscreen)
{
    initialised_ = false;

    wnd_size_x_ = wnd_size_x;
    wnd_size_y_ = wnd_size_y;
    wnd_pos_x_ = wnd_pos_y_ = 10;
    fullscreen_ = fullscreen;

    window_ = nullptr;
    display_manager_ = nullptr;

    t_last_renderloop_at_ = std::chrono::high_resolution_clock::now();
    default_input_handler_ = new DefaultInputHandler();
    pushInputHandler(default_input_handler_);

    rotate_light_ = true;
    light_coords_ = glm::vec3(5, 5, 3);
    light_radius_ = 10.0f;
    light_angle_degrees_ = 0.0f;
}

WindowManager::~WindowManager()
{
    std::cerr << "Destroying WindowManager" << std::endl;

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

    display_manager_->setResetCameraCallback(std::bind(&WindowManager::resetCameraCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    display_manager_->resetCamera();

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

    continue_rendering_ = true;
    t_rendering_started_at_ = t_last_renderloop_at_ =  std::chrono::high_resolution_clock::now();

    display_manager_->setLightCoords(light_coords_);
    display_manager_->setLightColour(glm::vec3(1, 1, 1), 0.5);

    while (continue_rendering_)
    {
        auto t_now = std::chrono::high_resolution_clock::now();

        GLfloat secs_since_last_renderloop = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - t_last_renderloop_at_).count();
        GLfloat secs_since_rendering_started = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - t_rendering_started_at_).count();
        t_last_renderloop_at_ = t_now;

        processEvents(secs_since_last_renderloop);

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

void WindowManager::stop()
{
    continue_rendering_ = false;
}

void WindowManager::processEvents(GLfloat secs_since_last_renderloop)
{
    SDL_Event window_event;

    while (SDL_PollEvent(&window_event))                // gather clicks, keystrokes, window movements, resizes etc
    {
        if (window_event.type == SDL_QUIT)
        {
            stop();
            break;
        }

        if (fullscreen_ && window_event.type == SDL_KEYUP && window_event.key.keysym.sym == SDLK_ESCAPE)
        {
            stop();
            break;
        }

        if (window_event.type == SDL_MOUSEBUTTONDOWN || window_event.type == SDL_MOUSEBUTTONUP || window_event.type == SDL_MOUSEMOTION)
        {
            for (const auto handler : input_handlers_)
            {
                handler->handleMouse(this, window_event, secs_since_last_renderloop);
            }
        }

        if (window_event.type == SDL_KEYDOWN || window_event.type == SDL_KEYUP)
        {
            for (const auto handler : input_handlers_)
            {
                handler->handleKeystroke(this, window_event, secs_since_last_renderloop);
            }
        }
    }
}

void WindowManager::pushInputHandler(InputHandler* input_handler)
{
    input_handlers_.push_back(input_handler);
}

void WindowManager::popInputHandler()
{
    if (input_handlers_.size() <= 1)
    {
        std::cerr << "Can't remove the default input handler" << std::endl;
        return;
    }

    input_handlers_.pop_back();
}

void WindowManager::resetCameraCallback(const glm::vec3& camera_world_coords, const glm::vec3& up_vector, const glm::vec3& pointing_vector)
{
    for (const auto handler : input_handlers_)
    {
        handler->resetCamera(camera_world_coords, up_vector, pointing_vector);
    }
}


}   // namespace insight