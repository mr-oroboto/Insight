#ifndef INSIGHT_CORE_DISPLAYMANAGER_H
#define INSIGHT_CORE_DISPLAYMANAGER_H

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)

#include "FrameQueue.h"
#include "primitive/PrimitiveCollection.h"

class DisplayManager
{
public:
    DisplayManager();
    ~DisplayManager();

    void setFrameQueue(FrameQueue *queue);
    bool initialise(GLfloat wndWidthPx, GLfloat wndHeightPx);
    void drawScene();
    void drawText(std::string text, glm::vec3 position, bool ortho = true, GLfloat scale = 1.0f, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));

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

    GLuint uniTextViewTransform;
    GLuint uniTextProjectionTransform;
    GLuint uniTextColour;
};

#endif //INSIGHT_CORE_DISPLAYMANAGER_H
