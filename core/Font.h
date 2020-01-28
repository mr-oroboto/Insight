#ifndef INSIGHT_CORE_FONT_H
#define INSIGHT_CORE_FONT_H

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ft2build.h>
#include "freetype/freetype.h"
#include FT_FREETYPE_H

class Font
{
public:
    enum Type
    {
        FONT_DEFAULT = 0,
    };

    Font(const std::string& path, FT_Library freetype);
    ~Font();

    bool initialise();

    GLuint drawCharacter(char c, GLuint vbo, const glm::vec3& world_coords, GLfloat scale = 1.0);

private:
    struct Character
    {
        GLuint texture_id;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
    };

    bool initialised_;

    std::string path_;
    FT_Library freetype_;

    std::map<GLchar, Character> characters_;
};

#endif //INSIGHT_CORE_FONT_H
