#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)
#include "DisplayManager.h"
#include "FrameQueue.h"
#include "Frame.h"

#define WINDOW_FULLSCREEN false
#define WINDOW_X_POS 100
#define WINDOW_Y_POS 100
#define WINDOW_X_SIZE 800
#define WINDOW_Y_SIZE 600

using namespace std;

/**********************************************************************************************************************
 * Entry point
 **********************************************************************************************************************/

int main(int argc, char *argv[])
{
    SDL_Window* window = nullptr;
    SDL_Event windowEvent;

    GLfloat cameraX = 6.5f, cameraY = 6.5f, cameraZ = 1.5f;
    GLfloat cameraIncrement = 0.05f;
    GLfloat cameraSphereRadius = 6.5f;
    GLfloat cameraTheta = 0.0f;

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

    FrameQueue* frameQueue = new FrameQueue(dm, true);
    Frame* frame = frameQueue->newFrame();

    frame->addObject(Primitive::Type::CUBE, glm::vec3(0, 0, 0));
    frame->addObject(Primitive::Type::TRIANGLE, glm::vec3(2, 0, 0));

    frameQueue->enqueueFrame(frame);
    frameQueue->setReady();
    frameQueue->setActive();

    while (true)
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
                    if ((cameraZ += 0.01f) > 3.0f)
                    {
                        cameraZ = 3.0f;
                    }
                }
                else if (windowEvent.key.keysym.sym == SDLK_DOWN)
                {
                    if ((cameraZ -= 0.01f) < -3.0f)
                    {
                        cameraZ = -3.0f;
                    }
                }
                else if (windowEvent.key.keysym.sym == SDLK_LEFT)
                {
                    cameraTheta += cameraIncrement;

                    cameraX = cameraSphereRadius * sin(cameraTheta);
                    cameraY = cameraSphereRadius * cos(cameraTheta);
                }
                else if (windowEvent.key.keysym.sym == SDLK_RIGHT)
                {
                    cameraTheta -= cameraIncrement;

                    cameraX = cameraSphereRadius * sin(cameraTheta);
                    cameraY = cameraSphereRadius * cos(cameraTheta);
                }
            }
        }

        dm->drawScene();

        // swap the (presumably prepared elsewhere) back-buffer into the front-buffer
        SDL_GL_SwapWindow(window);
    }

    /******************************************************************************************************************/


    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}