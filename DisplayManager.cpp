#include "DisplayManager.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader/ShaderCollection.h"
#include "FrameQueue.h"

DisplayManager::DisplayManager()
{
    initialised = false;
    frameQueue = nullptr;
    primitives = nullptr;
}

DisplayManager::~DisplayManager()
{
    if (initialised)
    {
        teardown();
    }
}

bool DisplayManager::initialise(GLfloat wndWidthPx, GLfloat wndHeightPx)
{
    if (initialised)
    {
        return true;
    }

    wndWidth = wndWidthPx;
    wndHeight = wndHeightPx;

    setPerspective(1.0f, 50.0f, 45.0f);
    setCameraLocation(0, 33, 0);

    try
    {
        if ( ! initialiseFreeType())
        {
            throw "Failed to initialise FreeType";
        }

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
        glUniformMatrix4fv(uniProjectionTransform, 1, GL_FALSE, glm::value_ptr(projectionTransform));

        reference = glGetUniformLocation(shaderProgram, "doOverrideColour");
        if (reference < 0)
        {
            throw "Can't find model colour override toggle in shader program";
        }
        uniModelDoOverrideColour = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shaderProgram, "overrideColour");
        if (reference < 0)
        {
            throw "Can't find model override colour in shader program";
        }
        uniModelOverrideColour = static_cast<GLuint>(reference);

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

bool DisplayManager::initialiseFreeType()
{
    bool success = false;

    FT_Library ft;
    FT_Face face;

    try
    {
        if (FT_Init_FreeType(&ft))
        {
            throw "Failed to initialise FreeType";
        }

        if (FT_New_Face(ft, "/home/sysop/ClionProjects/Insight/font/Vera.ttf", 0, &face))
        {
            throw "Failed to find fonts";
        }

        FT_Set_Pixel_Sizes(face, 0, 24);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable 4 byte alignment

        for (GLubyte c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cerr << "Failed to load glyph for '" << c << "'" << std::endl;
                continue;
            }

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);              // make the texture we just generated the active one
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RED,                                // internalFormat
                         face->glyph->bitmap.width,             // texture width  (mapped to 0..1 in texture co-ords)
                         face->glyph->bitmap.rows,              // texture height (mapped to 0..1 in texture co-ords)
                         0,
                         GL_RED,                                // format, GL_RED as glyph bitmap is greyscale single byte
                         GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer             // texture data
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // how texture co-ords < 0 or > 1 are interpreted in x dimension
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    // how texture co-ords < 0 or > 1 are interpreted in y dimension
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       // how to scale texture down
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       // how to scale texture up

            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
            };

            characters.insert(std::pair<GLchar, Character>(c, character));
        }

        textVertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(textVertexShader, 1, &ShaderCollection::textVertexSource, NULL);
        glCompileShader(textVertexShader);
        if ( ! shaderInitialised(textVertexShader))
        {
            throw "Failed to initialise text vertex shader";
        }

        textFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(textFragmentShader, 1, &ShaderCollection::textFragmentSource, NULL);
        glCompileShader(textFragmentShader);
        if ( ! shaderInitialised(textFragmentShader))
        {
            throw "Failed to initialise text fragment shader";
        }

        textShaderProgram = glCreateProgram();
        glAttachShader(textShaderProgram, textVertexShader);
        glAttachShader(textShaderProgram, textFragmentShader);
        glBindFragDataLocation(textShaderProgram, 0, "outColour");
        glLinkProgram(textShaderProgram);
        glUseProgram(textShaderProgram);

        GLint positionAttribute = glGetAttribLocation(textShaderProgram, "position");
        if (positionAttribute < 0)
        {
            throw "Can't find text position attribute in text shader program";
        }

        GLint texCoordsAttribute = glGetAttribLocation(textShaderProgram, "inTexCoords");
        if (texCoordsAttribute < 0)
        {
            throw "Can't find text texture co-ordinate attribute in text shader program";
        }

        GLint reference = glGetUniformLocation(textShaderProgram, "view");
        if (reference < 0)
        {
            throw "Can't find text view transform uniform in text shader program";
        }
        uniTextViewTransform = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(textShaderProgram, "projection");
        if (reference < 0)
        {
            throw "Can't find text projection transform uniform in text shader program";
        }
        uniTextProjectionTransform = static_cast<GLuint>(reference);

        textProjectionTransform = glm::ortho(0.0f,       // x == 0 is left of screen
                                             wndWidth,   // right of screen
                                             0.0f,       // y == 0 is bottom of screen
                                             wndHeight   // top of screen
        );
        glUniformMatrix4fv(uniTextProjectionTransform, 1, GL_FALSE, glm::value_ptr(textProjectionTransform));

        reference = glGetUniformLocation(textShaderProgram, "textColour");
        if (reference < 0)
        {
            throw "Can't find text colour uniform in text shader program";
        }
        uniTextColour = static_cast<GLuint>(reference);

        glGenVertexArrays(1, &textVao);     // create a VAO for our texture vertex buffer (dynamically updated during drawing)
        glBindVertexArray(textVao);

        glGenBuffers(1, &textVbo);
        glBindBuffer(GL_ARRAY_BUFFER, textVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);  // 2D quad (two triangles) needs 6 vertices with 5 attributes (3x pos, 2x tex co-ords) each

        glEnableVertexAttribArray(static_cast<GLuint>(positionAttribute));
        glVertexAttribPointer(static_cast<GLuint>(positionAttribute),
                              3,                  /* num of values to read from array per vertex */
                              GL_FLOAT,           /* type of those values */
                              GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                              5 * sizeof(float),  /* stride: each (x,y,z) pos has texture co-ords (s,t) in between */
                              0                   /* offset */);

        glEnableVertexAttribArray(static_cast<GLuint>(texCoordsAttribute));
        glVertexAttribPointer(static_cast<GLuint>(texCoordsAttribute),
                              2,                         /* num of values to read from array per vertex */
                              GL_FLOAT,                  /* type of those values */
                              GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                              5 * sizeof(float),         /* stride: each (s,t) co-ords has vertex position (x,y,z) in between */
                              (void*)(3 * sizeof(float)) /* offset */);

        glBindBuffer(GL_ARRAY_BUFFER, 0);   // @todo: do we need this? just so we don't accidentally add stuff to it in future...
        glBindVertexArray(0);

        success = true;
    }
    catch (std::string exception)
    {
        std::cerr << exception << std::endl;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return success;
}

void DisplayManager::drawText(std::string text, glm::vec3 position, bool ortho, GLfloat scale, glm::vec3 colour)
{
    glEnable(GL_BLEND);                                 // this is disabled during any object rendering function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // because red channel is used as alpha in shaders

    glUseProgram(textShaderProgram);

    if (ortho)
    {
        glm::mat4 viewTransform = glm::mat4(1.0f);

        glUniformMatrix4fv(uniTextViewTransform, 1, GL_FALSE, glm::value_ptr(viewTransform));                   // identity matrix
        glUniformMatrix4fv(uniTextProjectionTransform, 1, GL_FALSE, glm::value_ptr(textProjectionTransform));   // orthographic projection
    }
    else
    {
        glm::mat4 viewTransform = glm::lookAt(
                glm::vec3(cameraX, cameraY, cameraZ),     // camera position within world co-ordinates
                glm::vec3(0.0f, 0.0f, 0.0f),              // world co-ordinates to be center of the screen
                glm::vec3(0.0f, 0.0f, 1.0f)               // the "up" axis (z, making xy plane the "ground")
        );

        glUniformMatrix4fv(uniTextViewTransform, 1, GL_FALSE, glm::value_ptr(viewTransform));
        glUniformMatrix4fv(uniTextProjectionTransform, 1, GL_FALSE, glm::value_ptr(projectionTransform));       // normal perspective projection
    }

    glUniform3f(uniTextColour, colour.r, colour.g, colour.b);

    glActiveTexture(GL_TEXTURE0);       // sampler in shader is bound to texture unit 0, make this texture unit active so we can bind our texture to it

    glBindVertexArray(textVao);         // make our VBO and vertex attribute mapping active

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        GLfloat xpos = position.x + ch.bearing.x * scale;
        GLfloat ypos = position.y;
        GLfloat zpos = position.z;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        if (ortho)
        {
            ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

            GLfloat vertices[6][5] = {
                    //  x          y       z     s    t     (where s and t are the texture co-ordinates to sample from)
                    { xpos,     ypos + h, 0, 0.0, 0.0 }, // triangle 0 of 2D quad
                    { xpos,     ypos,     0, 0.0, 1.0 },
                    { xpos + w, ypos,     0, 1.0, 1.0 },

                    { xpos,     ypos + h, 0, 0.0, 0.0 }, // triangle 1 of 2D quad
                    { xpos + w, ypos,     0, 1.0, 1.0 },
                    { xpos + w, ypos + h, 0, 1.0, 0.0 }
            };

            glBindBuffer(GL_ARRAY_BUFFER, textVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            position.x += (ch.advance >> 6) * scale;
        }
        else
        {
            zpos = position.z - (ch.size.y - ch.bearing.y) * scale;

            // render onto a xz plane
            GLfloat vertices[6][5] = {
                    //  x          y       z     s    t     (where s and t are the texture co-ordinates to sample from)
                    { xpos,     ypos, zpos + h, 1.0, 0.0 }, // triangle 0 of 2D quad
                    { xpos,     ypos, zpos,     1.0, 1.0 },
                    { xpos + w, ypos, zpos,     0.0, 1.0 },

                    { xpos,     ypos, zpos + h, 1.0, 0.0 }, // triangle 1 of 2D quad
                    { xpos + w, ypos, zpos,     0.0, 1.0 },
                    { xpos + w, ypos, zpos + h, 0.0, 0.0 }
            };

            glBindBuffer(GL_ARRAY_BUFFER, textVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            position.x -= (ch.advance >> 6) * scale;
        }

        glBindTexture(GL_TEXTURE_2D, ch.textureId); // bind the texture to texture unit 0 (made active above)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

void DisplayManager::teardown()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glDeleteProgram(textShaderProgram);
    glDeleteShader(textVertexShader);
    glDeleteShader(textFragmentShader);

    if (primitives)
    {
        delete primitives;
        primitives = nullptr;
    }

    if (frameQueue)
    {
        delete frameQueue;
        frameQueue = nullptr;
    }

    initialised = false;
}

void DisplayManager::setCameraLocation(GLfloat x, GLfloat y, GLfloat z)
{
    cameraX = x;
    cameraY = y;
    cameraZ = z;

    std::cout << "camera at (" << x << ", " << y << ", " << z << ")" << std::endl;
}

void DisplayManager::setPerspective(GLfloat nearPlane, GLfloat farPlane, GLfloat fov)
{
    projectionTransform = glm::perspective(
            glm::radians(fov),    // vertical field-of-view (see open.gl/transformations)
            wndWidth / wndHeight, // aspect ratio of the screen
            nearPlane,            // near clipping plane (any vertex closer to camera than this disappears)
            farPlane              // far clipping plane (any vertex further from camera than this disappears)
    );

//  glUniformMatrix4fv(uniProjectionTransform, 1, GL_FALSE, glm::value_ptr(projectionTransform));
}

void DisplayManager::drawScene()
{
    glUseProgram(shaderProgram);

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

GLuint DisplayManager::getModelDoOverrideColourUniform()
{
    return uniModelDoOverrideColour;
}

GLuint DisplayManager::getModelOverrideColourUniform()
{
    return uniModelOverrideColour;
}