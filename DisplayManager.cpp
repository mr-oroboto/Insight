#include "DisplayManager.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float
#include "shader/ShaderCollection.h"
#include "FrameQueue.h"

DisplayManager::DisplayManager()
{

}

DisplayManager::~DisplayManager()
{
    if (initialised)
    {
        teardown();
    }
}

bool DisplayManager::initialise(GLfloat wndWidth, GLfloat wndHeight)
{
    if (initialised)
    {
        return true;
    }

    setCameraLocation(6.5, 6.5, 1.5);

    try
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &ShaderCollection::vertexSource, NULL);
        glCompileShader(vertexShader);
        if ( ! shaderInitialised(vertexShader))
        {
            throw "Failed to initialise vertex shader";
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &ShaderCollection::fragmentSource, NULL);
        glCompileShader(fragmentShader);
        if ( ! shaderInitialised(fragmentShader))
        {
            throw "Failed to initialise fragment shader";
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glBindFragDataLocation(shaderProgram, 0, "outColour");
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        GLint positionAttribute = glGetAttribLocation(shaderProgram, "position");
        if (positionAttribute < 0)
        {
            throw "Can't find position attribute in shader program";
        }

        // Add an entirely new attribute mapping for the colour data we added to the vertex attributes
        GLint colourAttribute = glGetAttribLocation(shaderProgram, "inColour");
        if (colourAttribute < 0)
        {
            throw "Can't find colour attribute in shader program";
        }

        primitives = new PrimitiveCollection(static_cast<GLuint>(positionAttribute), static_cast<GLuint>(colourAttribute));

        GLint reference = glGetUniformLocation(shaderProgram, "model");
        if (reference < 0)
        {
            throw "Can't find model transform in shader program";
        }
        uniModelTransform = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shaderProgram, "view");
        if (reference < 0)
        {
            throw "Can't find view transform in shader program";
        }
        uniViewTransform = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shaderProgram, "projection");
        if (reference < 0)
        {
            throw "Can't find projection transform in shader program";
        }
        uniProjectionTransform = static_cast<GLuint>(reference);

        // Create a projection transformation matrix to apply our perspective to the eye co-ordinates and turn them into
        // clipping co-ordinates. This is essentially our way to mapping 3D space to 2D space of the screen.
        glm::mat4 projectionTransform = glm::perspective(
                glm::radians(45.0f),    // vertical field-of-view (see open.gl/transformations)
                wndWidth / wndHeight,   // aspect ratio of the screen
                1.0f,                   // near clipping plane (any vertex closer to camera than this disappears)
                50.0f                   // far clipping plane (any vertex further from camera than this disappears)
        );
        glUniformMatrix4fv(uniProjectionTransform, 1, GL_FALSE, glm::value_ptr(projectionTransform));

        glEnable(GL_DEPTH_TEST);

        initialised = true;
    }
    catch (std::string e)
    {
        teardown();

        return false;
    }

    return true;
}

bool DisplayManager::shaderInitialised(GLuint shader)
{
    GLint shaderCompileStatus;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (shaderCompileStatus != GL_TRUE)
    {
        char errBuf[512];
        glGetShaderInfoLog(shader, 512, NULL, errBuf);
        std::cerr << "Failed to compile vertex shader: " << errBuf << std::endl;

        return false;
    }

    return true;
}

void DisplayManager::teardown()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (primitives)
    {
        delete primitives;
        primitives = nullptr;
    }
}

void DisplayManager::setCameraLocation(GLfloat x, GLfloat y, GLfloat z)
{
    cameraX = x;
    cameraY = y;
    cameraZ = z;
}

void DisplayManager::drawScene()
{
    std::cout << "camera at (" << cameraX << "," << cameraY << "," << cameraZ << ")" << std::endl;

    /**
     * View Transform
     */
    glm::mat4 viewTransform = glm::lookAt(
            glm::vec3(cameraX, cameraY, cameraZ),     // camera position within world co-ordinates
            glm::vec3(0.0f, 0.0f, 0.0f),              // world co-ordinates to be center of the screen
            glm::vec3(0.0f, 0.0f, 1.0f)               // the "up" axis (z, making xy plane the "ground")
    );
    glUniformMatrix4fv(uniViewTransform, 1, GL_FALSE, glm::value_ptr(viewTransform));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // clear screen to black, otherwise we'll paint over the last frame which looks weird
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the colour buffer if we've got GL_DEPTH_TEST enabled (this fixes the glitchy graphics)

    if (frameQueue)
    {
        frameQueue->drawCurrentFrame();
    }
}

PrimitiveCollection* DisplayManager::getPrimitiveCollection()
{
    return primitives;
}

void DisplayManager::setFrameQueue(FrameQueue* queue)
{
    // If we already have a FrameQueue, destroy it.
    if (frameQueue)
    {
        delete frameQueue;
    }

    frameQueue = queue;
}

GLuint DisplayManager::getModelTransformUniform()
{
    return uniModelTransform;
}