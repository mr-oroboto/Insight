#ifndef INSIGHT_DISPLAYMANAGER_H
#define INSIGHT_DISPLAYMANAGER_H

#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)
#include "primitives/PrimitiveCollection.h"

class DisplayManager {
public:
    DisplayManager();
    virtual ~DisplayManager();

    bool initialise(GLfloat wndWidth, GLfloat wndHeight);
    void drawScene();

    void setCameraLocation(GLfloat x, GLfloat y, GLfloat z);

private:
    void teardown();

    bool shaderInitialised(GLuint shader);

    PrimitiveCollection* primitives;

    bool initialised;

    GLuint shaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint uniModelTransform;
    GLuint uniViewTransform;
    GLuint uniProjectionTransform;

    GLfloat cameraX;
    GLfloat cameraY;
    GLfloat cameraZ;
};

#endif //INSIGHT_DISPLAYMANAGER_H
