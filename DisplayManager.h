#ifndef INSIGHT_DISPLAYMANAGER_H
#define INSIGHT_DISPLAYMANAGER_H

#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)
#include <map>
#include <string>
#include "primitive/PrimitiveCollection.h"

class FrameQueue;

class DisplayManager
{
public:
    DisplayManager();
    ~DisplayManager();

    void setFrameQueue(FrameQueue *queue);
    bool initialise(GLfloat wndWidthPx, GLfloat wndHeightPx);
    void drawScene();
    void drawText(std::string text, GLfloat x, GLfloat y, GLfloat z, bool ortho = true, GLfloat scale = 1.0f, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));

    void setCameraLocation(GLfloat x, GLfloat y, GLfloat z);
    void setPerspective(GLfloat nearPlane, GLfloat farPlane, GLfloat fov);

    PrimitiveCollection* getPrimitiveCollection();

    GLuint getModelTransformUniform();
    GLuint getModelDoOverrideColourUniform();
    GLuint getModelOverrideColourUniform();

private:
    bool initialiseFreeType();
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
    glm::mat4 textProjectionTransform;

    GLfloat cameraX;
    GLfloat cameraY;
    GLfloat cameraZ;

    struct Character
    {
        GLuint textureId;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
    };

    std::map<GLchar, Character> characters;

    GLuint textShaderProgram;
    GLuint textVertexShader;
    GLuint textFragmentShader;

    GLuint textVao;
    GLuint textVbo;

    GLuint uniTextGlyphData;
    GLuint uniTextColour;
    GLuint uniTextProjectionTransform;
};

#endif //INSIGHT_DISPLAYMANAGER_H
