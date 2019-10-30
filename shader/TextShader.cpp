#include "TextShader.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>         // convert matrix to float

TextShader::TextShader()
{
}

TextShader::~TextShader()
{
    std::cout << "TextShader::~TextShader()" << std::endl;
}

bool TextShader::initialise()
{
    try
    {
        if ( ! Shader::initialise(vertex_source_, fragment_source_))
        {
            throw "Failed to initialise base shader";
        }

        GLint reference;

        reference = glGetAttribLocation(shader_program_, "position");
        if (reference < 0)
        {
            throw "Can't find text position attribute in text shader program";
        }
        position_attribute_ = static_cast<GLuint>(reference);

        reference = glGetAttribLocation(shader_program_, "inTexCoords");
        if (reference < 0)
        {
            throw "Can't find text texture co-ordinate attribute in text shader program";
        }
        texture_coords_attribute_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "view");
        if (reference < 0)
        {
            throw "Can't find text view transform uniform in text shader program";
        }
        uni_view_transform_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "projection");
        if (reference < 0)
        {
            throw "Can't find text projection transform uniform in text shader program";
        }
        uni_projection_transform_ = static_cast<GLuint>(reference);

//      glUniformMatrix4fv(uni_text_projection_transform_, 1, GL_FALSE, glm::value_ptr(text_projection_transform_));

        reference = glGetUniformLocation(shader_program_, "textColour");
        if (reference < 0)
        {
            throw "Can't find text colour uniform in text shader program";
        }
        uni_text_colour_ = static_cast<GLuint>(reference);

        initialised_ = true;
    }
    catch (const char* exception)
    {
        std::cerr << exception << std::endl;
    }

    return initialised_;
}

GLuint TextShader::getPositionAttribute()
{
    return position_attribute_;
}

GLuint TextShader::getTextureCoordsAttribute()
{
    return texture_coords_attribute_;
}

void TextShader::setProjectionTransform(const glm::mat4& transform)
{
    use();
    glUniformMatrix4fv(uni_projection_transform_, 1, GL_FALSE, glm::value_ptr(transform));
}

void TextShader::setViewTransform(const glm::mat4& transform)
{
    use();
    glUniformMatrix4fv(uni_view_transform_, 1, GL_FALSE, glm::value_ptr(transform));
}

void TextShader::setTextColour(const glm::vec3& colour)
{
    use();
    glUniform3f(uni_text_colour_, colour.r, colour.g, colour.b);
}


const char* TextShader::vertex_source_ = R"glsl(
    #version 330 core

    in vec3 position;
    in vec2 inTexCoords;

    out vec2 TexCoords;

    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * vec4(position.x, position.y, position.z, 1.0);   // simple matrix * column vector
        TexCoords = inTexCoords;
    }
)glsl";

const char* TextShader::fragment_source_ = R"glsl(
    #version 330 core

    in vec2 TexCoords;
    out vec4 outColour;

    uniform sampler2D text;     // mono coloured bitmap image of glyph
    uniform vec3 textColour;    // to adjust final text colour

    void main()
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);     // all stored in red component
        outColour = vec4(textColour, 1.0) * sampled;
    }
)glsl";
