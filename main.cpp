#include <math.h>

#include <iostream>
#include <chrono>

#include <SDL2/SDL.h>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

#include "core/DisplayManager.h"
#include "core/FrameQueue.h"
#include "scenario/MinHeap.h"
#include "scenario/RotationTest.h"

#define WINDOW_FULLSCREEN false
#define WINDOW_X_POS 10
#define WINDOW_Y_POS 10
#define WINDOW_X_SIZE 2560
#define WINDOW_Y_SIZE 1440

/**********************************************************************************************************************
 * Entry point
 **********************************************************************************************************************/

int main(int argc, char *argv[])
{
    SDL_Window* window = nullptr;
    SDL_Event window_event;

    std::chrono::high_resolution_clock::time_point t_last_frame_drawn_at = std::chrono::high_resolution_clock::now();

    glm::vec3 camera_coords = glm::vec3(0, 5, 31);
    glm::vec3 camera_direction = glm::vec3(0, 0, -1);
    GLfloat pitch = 0.0f, yaw = 270.0f;     // to get z = -1 initially

    GLfloat camera_speed = 10.0f;    // how fast camera position moves per second (units / s)
    GLfloat pitch_speed = 30.0f;     // how fast camera pitches per second (degrees / s)
    GLfloat yaw_speed = 30.0f;       // how fast camera yaws per second (degrees / s)

    glm::vec3 light_coords = glm::vec3(5, 5, 3);
    GLfloat light_radius = 10.0f;
    GLfloat light_angle_degrees = 0.0f;

    // SDL is used to abstract the creation of an X window and event loop etc
    SDL_Init(SDL_INIT_EVERYTHING);

    // Before we can create the OpenGL context we hint that we want a forward compatible OpenGL 3.2 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);                // create a stencil buffer

    // Create a window (abstracted, uses the right underlying APIs whether on Windows, Mac or Linux)
    if ( ! WINDOW_FULLSCREEN)
    {
        window = SDL_CreateWindow("OpenGL", (int)WINDOW_X_POS, WINDOW_Y_POS, WINDOW_X_SIZE, WINDOW_Y_SIZE, SDL_WINDOW_OPENGL);
    }
    else
    {
        window = SDL_CreateWindow("OpenGL", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN);
    }

    // Create the OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Now that context is created we can use GLEW to register / import all available OpenGL API calls
    glewExperimental = GL_TRUE;
    glewInit();

    /************************************************ MAIN LINE *******************************************************/

    DisplayManager* dm = new DisplayManager();
    if ( ! dm->initialise(WINDOW_X_SIZE, WINDOW_Y_SIZE))
    {
        std::cout << "Failed to initialise OpenGL" << std::endl;
        return -1;
    }

    dm->setCameraCoords(camera_coords, camera_direction);
    dm->setLightCoords(light_coords);
    dm->setLightColour(glm::vec3(1, 1, 1), 0.5);

    {
        MinHeap scenario(dm);
//        RotationTest scenario(dm);
        scenario.run();
    }

    bool continue_rendering = true;
    bool lighting_on = true;

    while (continue_rendering)
    {
        auto t_now = std::chrono::high_resolution_clock::now();
        GLfloat secs_since_last_frame = std::chrono::duration_cast<std::chrono::duration<GLfloat>>(t_now - t_last_frame_drawn_at).count();
        t_last_frame_drawn_at = t_now;

        GLfloat camera_speed_increment = camera_speed * secs_since_last_frame;
        GLfloat pitch_speed_increment = pitch_speed * secs_since_last_frame;
        GLfloat yaw_speed_increment = yaw_speed * secs_since_last_frame;

        light_angle_degrees += 0.1;

        light_coords.x = light_radius * cos(light_angle_degrees / (2*M_PI));
        light_coords.z = light_radius * sin(light_angle_degrees / (2*M_PI));

        dm->setLightCoords(light_coords);

        if (SDL_PollEvent(&window_event))                // gather clicks, keystrokes, window movements, resizes etc
        {
            if (window_event.type == SDL_QUIT) break;
            if (WINDOW_FULLSCREEN && window_event.type == SDL_KEYUP && window_event.key.keysym.sym == SDLK_ESCAPE) break;

            if (window_event.type == SDL_KEYDOWN)
            {
                /**
                 * When we move the camera around we actually want to move it around the outside of a sphere with a
                 * radius that reaches out from the origin of the world. Moving in any direction (up, down, left,
                 * right) moves us around on the surface of that sphere. Our (x,y and z) co-ordinates of the camera
                 * must change for each key press.
                 */
                if (window_event.key.keysym.sym == SDLK_w)
                {
                    camera_coords += camera_speed_increment * dm->getCameraDirectionVector();
                }
                else if (window_event.key.keysym.sym == SDLK_s)
                {
                    camera_coords -= camera_speed_increment * dm->getCameraDirectionVector();
                }
                else if (window_event.key.keysym.sym == SDLK_a)
                {
                    // move left along normal to the camera direction and world up
                    camera_coords -= glm::normalize(glm::cross(dm->getCameraDirectionVector(), dm->getCameraUpVector())) * camera_speed_increment;
                }
                else if (window_event.key.keysym.sym == SDLK_d)
                {
                    // move right along normal to camera direction and world up
                    camera_coords += glm::normalize(glm::cross(dm->getCameraDirectionVector(), dm->getCameraUpVector())) * camera_speed_increment;
                }
                else if (window_event.key.keysym.sym == SDLK_UP)
                {
                    // adjust pitch (Euler angle around x-axis)
                    pitch += pitch_speed_increment;
                }
                else if (window_event.key.keysym.sym == SDLK_DOWN)
                {
                    // adjust pitch (Euler angle around x-axis)
                    pitch -= pitch_speed_increment;
                }
                else if (window_event.key.keysym.sym == SDLK_LEFT)
                {
                    // adjust yaw (Euler angle around y-axis)
                    yaw -= yaw_speed_increment;
                }
                else if (window_event.key.keysym.sym == SDLK_RIGHT)
                {
                    // adjust yaw (Euler angle around y-axis)
                    yaw += yaw_speed_increment;
                }

                // clamp to sane values
                if (pitch > 89.0f)
                {
                    pitch = 89.0f;
                }
                else if (pitch < -89.0f)
                {
                    pitch = -89.0f;
                }

                camera_direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
                camera_direction.y = sin(glm::radians(pitch));
                camera_direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
                camera_direction = glm::normalize(camera_direction);

                dm->setCameraCoords(camera_coords, camera_direction);
            }
            else if (window_event.type == SDL_KEYUP)
            {
                if (window_event.key.keysym.sym == SDLK_q)
                {
                    continue_rendering = false;
                }
                else if (window_event.key.keysym.sym == SDLK_l)
                {
                    lighting_on = ! lighting_on;
                    dm->setLightingOn(lighting_on);
                }
            }
        }

        dm->drawScene();

        // swap the (presumably prepared elsewhere) back-buffer into the front-buffer
        SDL_GL_SwapWindow(window);
    }

    delete dm;

    std::cout << "Cleanup complete, exiting" << std::endl;

    /******************************************************************************************************************/

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}