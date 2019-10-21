#ifndef INSIGHT_DISPLAYMANAGER_H
#define INSIGHT_DISPLAYMANAGER_H

#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)
#include "primitive/PrimitiveCollection.h"

class FrameQueue;

class DisplayManager {
public:
    DisplayManager();
    ~DisplayManager();

    void setFrameQueue(FrameQueue *queue);
    bool initialise(GLfloat wndWidthPx, GLfloat wndHeightPx);
    void drawScene();

    void setCameraLocation(GLfloat x, GLfloat y, GLfloat z);
    void setPerspective(GLfloat nearPlane, GLfloat farPlane, GLfloat fov);

    PrimitiveCollection* getPrimitiveCollection();

    GLuint getModelTransformUniform();
    GLuint getModelDoOverrideColourUniform();
    GLuint getModelOverrideColourUniform();

private:
    void teardown();

    bool shaderInitialised(GLuint shader);

    PrimitiveCollection* primitives;
    FrameQueue *frameQueue;

    bool initialised;

    GLuint shaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint uniModelTransform;
    GLuint uniViewTransform;
    GLuint uniProjectionTransform;
    GLuint uniModelDoOverrideColour;
    GLuint uniModelOverrideColour;

    GLfloat wndWidth;
    GLfloat wndHeight;
    glm::mat4 projectionTransform;

    GLfloat cameraX;
    GLfloat cameraY;
    GLfloat cameraZ;
};

#endif //INSIGHT_DISPLAYMANAGER_H
