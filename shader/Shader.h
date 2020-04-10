#ifndef INSIGHT_SHADER_SHADER_H
#define INSIGHT_SHADER_SHADER_H

#include <GL/glew.h>

namespace insight {
namespace shader {

class Shader {
public:
    virtual ~Shader();

    // Make the shader active.
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

}   // namespace shader
}   // namespace insight

#endif //INSIGHT_SHADER_SHADER_H
