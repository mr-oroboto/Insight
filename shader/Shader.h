#ifndef INSIGHT_SHADER_SHADER_H
#define INSIGHT_SHADER_SHADER_H

#include <GL/glew.h>

class Shader
{
public:
    virtual ~Shader();

    void use();

protected:
    Shader();

    bool initialise(const char* vertex_source, const char* fragment_source);

    bool isCompiled(GLuint shader_subprogram);

    bool initialised_;

    GLuint shader_program_;
    GLuint vertex_shader_;
    GLuint fragment_shader_;
};

#endif //INSIGHT_SHADER_SHADER_H
