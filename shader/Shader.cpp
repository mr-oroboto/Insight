#include "Shader.h"

#include <iostream>

Shader::Shader()
{
    initialised_ = false;
}

Shader::~Shader()
{
    glDeleteProgram(shader_program_);
    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);
}

bool Shader::initialise(const char* vertex_source, const char* fragment_source)
{
    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader_);
    if ( !isCompiled(vertex_shader_))
    {
        throw "Failed to initialise vertex shader";
    }

    fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader_);
    if ( !isCompiled(fragment_shader_))
    {
        throw "Failed to initialise fragment shader";
    }

    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader_);
    glAttachShader(shader_program_, fragment_shader_);
    glBindFragDataLocation(shader_program_, 0, "outColour");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);

    return true;
}

bool Shader::isCompiled(GLuint shader_subprogram)
{
    GLint shader_compile_status;

    glGetShaderiv(shader_subprogram, GL_COMPILE_STATUS, &shader_compile_status);
    if (shader_compile_status != GL_TRUE)
    {
        char errBuf[512];
        glGetShaderInfoLog(shader_subprogram, 512, NULL, errBuf);
        std::cerr << "Failed to compile vertex shader: " << errBuf << std::endl;

        return false;
    }

    return true;
}

void Shader::use()
{
    glUseProgram(shader_program_);
}