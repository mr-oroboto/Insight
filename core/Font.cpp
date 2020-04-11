#include "Font.h"

#include <iostream>

#include <ft2build.h>
#include "freetype/freetype.h"

namespace insight {


Font::Font(const std::string& path, FT_Library freetype)
{
    path_ = path;
    freetype_ = freetype;
}

bool Font::initialise()
{
    FT_Face face;

    if (FT_New_Face(freetype_, path_.c_str(), 0, &face))
    {
        throw "Failed to find font";
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

    FT_Done_Face(face);

    return true;
}

GLuint Font::drawCharacter(char c, GLuint vbo, const glm::vec3& world_coords, GLfloat scale)
{
    Character ch = characters_[c];

    GLfloat x_pos = world_coords.x + ch.bearing.x * scale;
    GLfloat y_pos = world_coords.y - (ch.size.y - ch.bearing.y) * scale;
    GLfloat z_pos = world_coords.z;

    GLfloat width = ch.size.x * scale;
    GLfloat height = ch.size.y * scale;

    GLfloat vertices[6][5] = {
            //  x              y             z       s    t     (where s and t are the texture co-ordinates to sample from)
            { x_pos,         y_pos + height, z_pos, 0.0, 0.0 }, // triangle 0 of 2D quad
            { x_pos,         y_pos,          z_pos, 0.0, 1.0 },
            { x_pos + width, y_pos,          z_pos, 1.0, 1.0 },

            { x_pos,         y_pos + height, z_pos, 0.0, 0.0 }, // triangle 1 of 2D quad
            { x_pos + width, y_pos,          z_pos, 1.0, 1.0 },
            { x_pos + width, y_pos + height, z_pos, 1.0, 0.0 }
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindTexture(GL_TEXTURE_2D, ch.texture_id); // bind the texture to texture unit 0 (made active above)
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return ch.advance;
}


}   // namespace insight