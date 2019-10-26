#include <math.h>

#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

#include "core/DisplayManager.h"
#include "core/FrameQueue.h"
#include "scenario/MinHeap.h"

#define WINDOW_FULLSCREEN false
#define WINDOW_X_POS 10
#define WINDOW_Y_POS 10
#define WINDOW_X_SIZE 2560
#define WINDOW_Y_SIZE 1440

using namespace std;

/**********************************************************************************************************************
 * Entry point
 **********************************************************************************************************************/

int main(int argc, char *argv[])
{
    SDL_Window* window = nullptr;
    SDL_Event windowEvent;

    GLfloat cameraX = 0.0f, cameraY = 33.0f, cameraZ = 4.0f;
    GLfloat cameraRadius = 33.0f;
    GLfloat cameraRotationIncrementDeg = 1.0;
    GLfloat cameraAngle = 90.0f;

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
        cout << "Failed to initialise OpenGL" << endl;
        return -1;
    }

    {
        MinHeap minHeap(dm);
        minHeap.run();
    }

    bool continueRendering = true;

    while (continueRendering)
    {
        if (SDL_PollEvent(&windowEvent))                // gather clicks, keystrokes, window movements, resizes etc
        {
            if (windowEvent.type == SDL_QUIT) break;
            if (WINDOW_FULLSCREEN && windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;

            if (windowEvent.type == SDL_KEYDOWN)
            {
                /**
                 * When we move the camera around we actually want to move it around the outside of a sphere with a
                 * radius that reaches out from the origin of the world. Moving in any direction (up, down, left,
                 * right) moves us around on the surface of that sphere. Our (x,y and z) co-ordinates of the camera
                 * must change for each key press.
                 */
                if (windowEvent.key.keysym.sym == SDLK_UP)
                {
                    cameraZ += 1;
                }
                else if (windowEvent.key.keysym.sym == SDLK_DOWN)
                {
                    cameraZ -= 1;
                }
                else if (windowEvent.key.keysym.sym == SDLK_LEFT)
                {
                    cameraAngle -= cameraRotationIncrementDeg;

                    cameraX = cameraRadius * cos(cameraAngle / (2*M_PI));
                    cameraY = cameraRadius * sin(cameraAngle / (2*M_PI));
                }
                else if (windowEvent.key.keysym.sym == SDLK_RIGHT)
                {
                    cameraAngle += cameraRotationIncrementDeg;

                    cameraX = cameraRadius * cos(cameraAngle / (2*M_PI));
                    cameraY = cameraRadius * sin(cameraAngle / (2*M_PI));
                }

                dm->setCameraLocation(cameraX, cameraY, cameraZ);
            }
            else if (windowEvent.type == SDL_KEYUP)
            {
                if (windowEvent.key.keysym.sym == SDLK_q)
                {
                    continueRendering = false;
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