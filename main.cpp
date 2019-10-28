#include <math.h>

#include <iostream>

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

    glm::vec3 camera_coords = glm::vec3(11, 11, 31);
    glm::vec3 light_coords = glm::vec3(5, 5, 3);
    GLfloat camera_radius = 33.0f;
    GLfloat camera_rotation_increment_degrees = 1.0;
    GLfloat camera_angle_degrees = 90.0f;
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

    dm->setCameraCoords(camera_coords);
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
                if (window_event.key.keysym.sym == SDLK_UP)
                {
                    camera_coords.y += 1;
                }
                else if (window_event.key.keysym.sym == SDLK_DOWN)
                {
                    camera_coords.y -= 1;
                }
                else if (window_event.key.keysym.sym == SDLK_LEFT)
                {
                    camera_angle_degrees -= camera_rotation_increment_degrees;

                    camera_coords.x = camera_radius * cos(camera_angle_degrees / (2*M_PI));
                    camera_coords.z = camera_radius * sin(camera_angle_degrees / (2*M_PI));
                }
                else if (window_event.key.keysym.sym == SDLK_RIGHT)
                {
                    camera_angle_degrees += camera_rotation_increment_degrees;

                    camera_coords.x = camera_radius * cos(camera_angle_degrees / (2*M_PI));
                    camera_coords.z = camera_radius * sin(camera_angle_degrees / (2*M_PI));
                }

                dm->setCameraCoords(camera_coords);
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