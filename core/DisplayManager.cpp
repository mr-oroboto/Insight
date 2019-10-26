#include "DisplayManager.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate
#include <glm/gtc/type_ptr.hpp>         // convert matrix to float
#include <ft2build.h>
#include "freetype/freetype.h"

#include "shader/ShaderCollection.h"

DisplayManager::DisplayManager()
{
    initialised_ = false;

    frame_queue_ = nullptr;
    primitives_ = nullptr;
}

DisplayManager::~DisplayManager()
{
    std::cout << "DisplayManager::~DisplayManager()" << std::endl;

    if (initialised_)
    {
        teardown();
    }
}

bool DisplayManager::initialise(GLfloat wnd_width, GLfloat wnd_height)
{
    if (initialised_)
    {
        return true;
    }

    wnd_width_ = wnd_width;
    wnd_height_ = wnd_height;

    setPerspective(1.0f, 50.0f, 45.0f);
    setCameraLocation(0, 33, 4);

    try
    {
        if ( ! initialiseFreeType())
        {
            throw "Failed to initialise FreeType";
        }

        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_, 1, &ShaderCollection::vertex_source_, NULL);
        glCompileShader(vertex_shader_);
        if ( !isShaderInitialised(vertex_shader_))
        {
            throw "Failed to initialise vertex shader";
        }

        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_, 1, &ShaderCollection::fragment_source_, NULL);
        glCompileShader(fragment_shader_);
        if ( !isShaderInitialised(fragment_shader_))
        {
            throw "Failed to initialise fragment shader";
        }

        shader_program_ = glCreateProgram();
        glAttachShader(shader_program_, vertex_shader_);
        glAttachShader(shader_program_, fragment_shader_);
        glBindFragDataLocation(shader_program_, 0, "outColour");
        glLinkProgram(shader_program_);
        glUseProgram(shader_program_);

        GLint position_attribute = glGetAttribLocation(shader_program_, "position");
        if (position_attribute < 0)
        {
            throw "Can't find position attribute in shader program";
        }

        // Add an entirely new attribute mapping for the colour data we added to the vertex attributes
        GLint colour_attribute = glGetAttribLocation(shader_program_, "inColour");
        if (colour_attribute < 0)
        {
            throw "Can't find colour attribute in shader program";
        }

        primitives_ = new PrimitiveCollection(static_cast<GLuint>(position_attribute), static_cast<GLuint>(colour_attribute));

        GLint reference = glGetUniformLocation(shader_program_, "model");
        if (reference < 0)
        {
            throw "Can't find model transform in shader program";
        }
        uni_model_transform_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "view");
        if (reference < 0)
        {
            throw "Can't find view transform in shader program";
        }
        uni_view_transform_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "projection");
        if (reference < 0)
        {
            throw "Can't find projection transform in shader program";
        }
        uni_projection_transform_ = static_cast<GLuint>(reference);

        // Create a projection transformation matrix to apply our perspective to the eye co-ordinates and turn them into
        // clipping co-ordinates. This is essentially our way to mapping 3D space to 2D space of the screen.
        glUniformMatrix4fv(uni_projection_transform_, 1, GL_FALSE, glm::value_ptr(projection_transform_));

        reference = glGetUniformLocation(shader_program_, "doOverrideColour");
        if (reference < 0)
        {
            throw "Can't find model colour override toggle in shader program";
        }
        uni_model_do_override_colour_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "overrideColour");
        if (reference < 0)
        {
            throw "Can't find model override colour in shader program";
        }
        uni_model_override_colour_ = static_cast<GLuint>(reference);

        initialised_ = true;
    }
    catch (std::string e)
    {
        teardown();

        return false;
    }

    return true;
}

bool DisplayManager::isShaderInitialised(GLuint shader)
{
    GLint shader_compile_status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compile_status);
    if (shader_compile_status != GL_TRUE)
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
                    static_cast<GLuint>(face->glyph->advance.x)
            };

            characters_.insert(std::pair<GLchar, Character>(c, character));
        }

        text_vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(text_vertex_shader_, 1, &ShaderCollection::text_vertex_source_, NULL);
        glCompileShader(text_vertex_shader_);
        if ( !isShaderInitialised(text_vertex_shader_))
        {
            throw "Failed to initialise text vertex shader";
        }

        text_fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(text_fragment_shader_, 1, &ShaderCollection::text_fragment_source_, NULL);
        glCompileShader(text_fragment_shader_);
        if ( !isShaderInitialised(text_fragment_shader_))
        {
            throw "Failed to initialise text fragment shader";
        }

        text_shader_program_ = glCreateProgram();
        glAttachShader(text_shader_program_, text_vertex_shader_);
        glAttachShader(text_shader_program_, text_fragment_shader_);
        glBindFragDataLocation(text_shader_program_, 0, "outColour");
        glLinkProgram(text_shader_program_);
        glUseProgram(text_shader_program_);

        GLint position_attribute = glGetAttribLocation(text_shader_program_, "position");
        if (position_attribute < 0)
        {
            throw "Can't find text position attribute in text shader program";
        }

        GLint tex_coords_attribute = glGetAttribLocation(text_shader_program_, "inTexCoords");
        if (tex_coords_attribute < 0)
        {
            throw "Can't find text texture co-ordinate attribute in text shader program";
        }

        GLint reference = glGetUniformLocation(text_shader_program_, "view");
        if (reference < 0)
        {
            throw "Can't find text view transform uniform in text shader program";
        }
        uni_text_view_transform_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(text_shader_program_, "projection");
        if (reference < 0)
        {
            throw "Can't find text projection transform uniform in text shader program";
        }
        uni_text_projection_transform_ = static_cast<GLuint>(reference);

        text_projection_transform_ = glm::ortho(0.0f,       // x == 0 is left of screen
                                             wnd_width_,   // right of screen
                                             0.0f,       // y == 0 is bottom of screen
                                             wnd_height_   // top of screen
        );
        glUniformMatrix4fv(uni_text_projection_transform_, 1, GL_FALSE, glm::value_ptr(text_projection_transform_));

        reference = glGetUniformLocation(text_shader_program_, "textColour");
        if (reference < 0)
        {
            throw "Can't find text colour uniform in text shader program";
        }
        uni_text_colour_ = static_cast<GLuint>(reference);

        glGenVertexArrays(1, &text_vao_);     // create a VAO for our texture vertex buffer (dynamically updated during drawing)
        glBindVertexArray(text_vao_);

        glGenBuffers(1, &text_vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);  // 2D quad (two triangles) needs 6 vertices with 5 attributes (3x pos, 2x tex co-ords) each

        glEnableVertexAttribArray(static_cast<GLuint>(position_attribute));
        glVertexAttribPointer(static_cast<GLuint>(position_attribute),
                              3,                  /* num of values to read from array per vertex */
                              GL_FLOAT,           /* type of those values */
                              GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                              5 * sizeof(float),  /* stride: each (x,y,z) pos has texture co-ords (s,t) in between */
                              0                   /* offset */);

        glEnableVertexAttribArray(static_cast<GLuint>(tex_coords_attribute));
        glVertexAttribPointer(static_cast<GLuint>(tex_coords_attribute),
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

void DisplayManager::teardown()
{
    glDeleteProgram(shader_program_);
    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);

    glDeleteProgram(text_shader_program_);
    glDeleteShader(text_vertex_shader_);
    glDeleteShader(text_fragment_shader_);

    if (primitives_)
    {
        delete primitives_;
        primitives_ = nullptr;
    }

    if (frame_queue_)
    {
        delete frame_queue_;
        frame_queue_ = nullptr;
    }

    initialised_ = false;
}

void DisplayManager::drawText(std::string text, glm::vec3 position, bool ortho, GLfloat scale, glm::vec3 colour)
{
    glEnable(GL_BLEND);                                 // this is disabled during any object rendering function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // because red channel is used as alpha in shaders

    glUseProgram(text_shader_program_);

    if (ortho)
    {
        glm::mat4 view_transform = glm::mat4(1.0f);

        glUniformMatrix4fv(uni_text_view_transform_, 1, GL_FALSE, glm::value_ptr(view_transform));                   // identity matrix
        glUniformMatrix4fv(uni_text_projection_transform_, 1, GL_FALSE, glm::value_ptr(text_projection_transform_));   // orthographic projection
    }
    else
    {
        glm::mat4 view_transform = glm::lookAt(
                glm::vec3(camera_x_, camera_y_, camera_z_),     // camera position within world co-ordinates
                glm::vec3(0.0f, 0.0f, 0.0f),              // world co-ordinates to be center of the screen
                glm::vec3(0.0f, 0.0f, 1.0f)               // the "up" axis (z, making xy plane the "ground")
        );

        glUniformMatrix4fv(uni_text_view_transform_, 1, GL_FALSE, glm::value_ptr(view_transform));
        glUniformMatrix4fv(uni_text_projection_transform_, 1, GL_FALSE, glm::value_ptr(projection_transform_));       // normal perspective projection
    }

    glUniform3f(uni_text_colour_, colour.r, colour.g, colour.b);

    glActiveTexture(GL_TEXTURE0);       // sampler in shader is bound to texture unit 0, make this texture unit active so we can bind our texture to it

    glBindVertexArray(text_vao_);         // make our VBO and vertex attribute mapping active

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters_[*c];

        GLfloat x_pos = position.x + ch.bearing.x * scale;
        GLfloat y_pos = position.y;
        GLfloat z_pos = position.z;

        GLfloat width = ch.size.x * scale;
        GLfloat height = ch.size.y * scale;

        if (ortho)
        {
            y_pos = position.y - (ch.size.y - ch.bearing.y) * scale;

            GLfloat vertices[6][5] = {
                    //  x          y       z     s    t     (where s and t are the texture co-ordinates to sample from)
                    { x_pos,         y_pos + height, 0, 0.0, 0.0 }, // triangle 0 of 2D quad
                    { x_pos,         y_pos,          0, 0.0, 1.0 },
                    { x_pos + width, y_pos,          0, 1.0, 1.0 },

                    { x_pos,         y_pos + height, 0, 0.0, 0.0 }, // triangle 1 of 2D quad
                    { x_pos + width, y_pos,          0, 1.0, 1.0 },
                    { x_pos + width, y_pos + height, 0, 1.0, 0.0 }
            };

            glBindBuffer(GL_ARRAY_BUFFER, text_vbo_);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            position.x += (ch.advance >> 6) * scale;
        }
        else
        {
            z_pos = position.z - (ch.size.y - ch.bearing.y) * scale;

            // render onto a xz plane
            GLfloat vertices[6][5] = {
                    //  x          y       z     s    t     (where s and t are the texture co-ordinates to sample from)
                    { x_pos,     y_pos, z_pos + height, 1.0, 0.0 }, // triangle 0 of 2D quad
                    { x_pos,     y_pos, z_pos,     1.0, 1.0 },
                    { x_pos + width, y_pos, z_pos,     0.0, 1.0 },

                    { x_pos,     y_pos, z_pos + height, 1.0, 0.0 }, // triangle 1 of 2D quad
                    { x_pos + width, y_pos, z_pos,     0.0, 1.0 },
                    { x_pos + width, y_pos, z_pos + height, 0.0, 0.0 }
            };

            glBindBuffer(GL_ARRAY_BUFFER, text_vbo_);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            position.x -= (ch.advance >> 6) * scale;
        }

        glBindTexture(GL_TEXTURE_2D, ch.texture_id); // bind the texture to texture unit 0 (made active above)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

void DisplayManager::setCameraLocation(GLfloat x, GLfloat y, GLfloat z)
{
    camera_x_ = x;
    camera_y_ = y;
    camera_z_ = z;

    std::cout << "camera at (" << x << ", " << y << ", " << z << ")" << std::endl;
}

void DisplayManager::setPerspective(GLfloat near_plane, GLfloat far_plane, GLfloat fov)
{
    projection_transform_ = glm::perspective(
            glm::radians(fov),    // vertical field-of-view (see open.gl/transformations)
            wnd_width_ / wnd_height_, // aspect ratio of the screen
            near_plane,            // near clipping plane (any vertex closer to camera than this disappears)
            far_plane              // far clipping plane (any vertex further from camera than this disappears)
    );

//  glUniformMatrix4fv(uni_projection_transform_, 1, GL_FALSE, glm::value_ptr(projection_transform_));
}

void DisplayManager::drawScene()
{
    glUseProgram(shader_program_);

    /**
     * View Transform
     */
    glm::mat4 view_transform = glm::lookAt(
            glm::vec3(camera_x_, camera_y_, camera_z_),     // camera position within world co-ordinates
            glm::vec3(0.0f, 0.0f, 0.0f),              // world co-ordinates to be center of the screen
            glm::vec3(0.0f, 0.0f, 1.0f)               // the "up" axis (z, making xy plane the "ground")
    );
    glUniformMatrix4fv(uni_view_transform_, 1, GL_FALSE, glm::value_ptr(view_transform));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // clear screen to black, otherwise we'll paint over the last frame which looks weird
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the colour buffer if we've got GL_DEPTH_TEST enabled (this fixes the glitchy graphics)

    if (frame_queue_)
    {
        frame_queue_->drawCurrentFrame();
    }
}

PrimitiveCollection* DisplayManager::getPrimitiveCollection()
{
    return primitives_;
}

void DisplayManager::setFrameQueue(FrameQueue* queue)
{
    // If we already have a FrameQueue, destroy it.
    if (frame_queue_)
    {
        delete frame_queue_;
    }

    frame_queue_ = queue;
}

GLuint DisplayManager::getModelTransformUniform()
{
    return uni_model_transform_;
}

GLuint DisplayManager::getModelDoOverrideColourUniform()
{
    return uni_model_do_override_colour_;
}

GLuint DisplayManager::getModelOverrideColourUniform()
{
    return uni_model_override_colour_;
}