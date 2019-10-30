#ifndef INSIGHT_CORE_TEXTDRAWER_H
#define INSIGHT_CORE_TEXTDRAWER_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ft2build.h>
#include "freetype/freetype.h"
#include FT_FREETYPE_H

#include "shader/TextShader.h"
#include "Font.h"

class DisplayManager;

class TextDrawer
{
public:
    TextDrawer(DisplayManager* display_manager);
    ~TextDrawer();

    bool initialise(GLuint wnd_width, GLuint wnd_height);
    bool registerFont(Font::Type font_type, const std::string& path);

    void print(const std::string& text, const glm::vec3& world_coords, bool ortho = true, Font::Type font_type = Font::Type::FONT_VERA, GLfloat scale = 1.0f, const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));

private:
    bool initialised_;

    DisplayManager* display_manager_;
    TextShader* shader_;
    FT_Library freetype_;
    std::unordered_map<Font::Type, Font*> fonts_;

    GLuint vao_;
    GLuint vbo_;

    glm::mat4 ortho_projection_transform_;
};


#endif //INSIGHT_CORE_TEXTDRAWER_H
