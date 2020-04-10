#include "TextDrawer.h"

#include <iostream>
#include <utility>

#include <glm/gtc/matrix_transform.hpp>

#include "core/DisplayManager.h"

TextDrawer::TextDrawer(DisplayManager* display_manager)
        : initialised_(false),
          display_manager_(display_manager) {}

TextDrawer::~TextDrawer()
{
    FT_Done_FreeType(freetype_);

    for (std::unordered_map<Font::Type, Font*>::iterator it = fonts_.begin(); it != fonts_.end(); it++)
    {
        if (it->second != nullptr)
        {
            delete it->second;
        }
    }
}

bool TextDrawer::initialise(GLuint wnd_width, GLuint wnd_height)
{
    if (initialised_)
    {
        return true;
    }

    try
    {
        if (FT_Init_FreeType(&freetype_))
        {
            throw "Failed to initialise FreeType";
        }

        shader_ = std::make_unique<TextShader>();
        if ( ! shader_->initialise())
        {
            throw "Failed to initialise text shader";
        }

        ortho_projection_transform_ = glm::ortho(0.0f,                            // x == 0 is left of screen
                                                static_cast<GLfloat>(wnd_width),  // right of screen
                                                0.0f,                             // y == 0 is bottom of screen
                                                static_cast<GLfloat>(wnd_height)  // top of screen
        );

        glGenVertexArrays(1, &vao_);            // create a VAO for our texture vertex buffer (dynamically updated during drawing)
        glBindVertexArray(vao_);

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);    // bind to VAO
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);  // 2D quad (two triangles) needs 6 vertices with 5 attributes (3x pos, 2x tex co-ords) each

        glEnableVertexAttribArray(shader_->getPositionAttribute());
        glVertexAttribPointer(shader_->getPositionAttribute(),
                              3,                  /* num of values to read from array per vertex */
                              GL_FLOAT,           /* type of those values */
                              GL_FALSE,           /* normalise to -1.0, 1.0 if not floats? */
                              5 * sizeof(float),  /* stride: each (x,y,z) pos has texture co-ords (s,t) in between */
                              0                   /* offset */);

        glEnableVertexAttribArray(shader_->getTextureCoordsAttribute());
        glVertexAttribPointer(shader_->getTextureCoordsAttribute(),
                              2,                         /* num of values to read from array per vertex */
                              GL_FLOAT,                  /* type of those values */
                              GL_FALSE,                  /* normalise to -1.0, 1.0 if not floats? */
                              5 *
                              sizeof(float),         /* stride: each (s,t) co-ords has vertex position (x,y,z) in between */
                              (void *) (3 * sizeof(float)) /* offset */);

        glBindBuffer(GL_ARRAY_BUFFER, 0);   // @todo: do we need this? just so we don't accidentally add stuff to it in future...
        glBindVertexArray(0);

        initialised_ = true;
    }
    catch (const char* exception)
    {
        std::cerr << exception << std::endl;
    }

    return initialised_;
}

bool TextDrawer::registerFont(Font::Type font_type, const std::string& path)
{
    if (fonts_.find(font_type) != fonts_.end() && fonts_[font_type] == nullptr)
    {
        delete fonts_[font_type];
        fonts_[font_type] = nullptr;
    }

    Font* font = new Font(path, freetype_);
    if ( ! font->initialise())
    {
        delete font;
        return false;
    }

    fonts_[font_type] = font;

    return true;
}

void TextDrawer::print(const std::string& text, const glm::vec3& world_coords, bool ortho, Font::Type font_type, GLfloat scale, const glm::vec3& colour)
{
    if ( ! initialised_ || fonts_.find(font_type) == fonts_.end() || fonts_[font_type] == nullptr)
    {
        return;
    }

    Font* font = fonts_[font_type];
    glm::vec3 text_world_coords = world_coords;

    glEnable(GL_BLEND);                                 // this is disabled during any object rendering function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // because red channel is used as alpha in shaders

    shader_->use();

    if (ortho)
    {
        glm::mat4 view_transform = glm::mat4(1.0f);

        shader_->setViewTransform(view_transform);                                      // identity matrix
        shader_->setProjectionTransform(ortho_projection_transform_);                   // orthographic projection
    }
    else
    {
        shader_->setViewTransform(display_manager_->getViewTransform());
        shader_->setProjectionTransform(display_manager_->getProjectionTransform());    // normal perspective projection
    }

    shader_->setTextColour(colour);

    glActiveTexture(GL_TEXTURE0);         // sampler in shader is bound to texture unit 0, make this texture unit active so we can bind our texture to it
    glBindVertexArray(vao_);              // make our VBO and vertex attribute mapping active

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        GLuint advance = font->drawCharacter(*c, vbo_, text_world_coords, scale);
        text_world_coords.x += (advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}