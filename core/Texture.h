#ifndef INSIGHT_CORE_TEXTURE_H
#define INSIGHT_CORE_TEXTURE_H

#include <string>

#include <GL/glew.h>

#include "shader/StandardShader.h"

namespace insight {


class Texture {
public:
    Texture(const std::string& path, shader::StandardShader* shader);
    ~Texture();

    bool initialise();

    void use(GLuint texture_unit = 0);

private:
    bool initialised_;

    shader::StandardShader* object_shader_;

    std::string path_;

    GLuint texture_id_;
    GLuint width_, height_;
    GLuint channels_;
};


}   // namespace insight

#endif //INSIGHT_TEXTURE_H
