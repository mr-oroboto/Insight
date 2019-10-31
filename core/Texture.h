#ifndef INSIGHT_CORE_TEXTURE_H
#define INSIGHT_CORE_TEXTURE_H

#include <iostream>

#include <GL/glew.h>

#include "shader/StandardShader.h"

class Texture
{
public:
    Texture(const std::string& path, StandardShader* shader);
    ~Texture();

    bool initialise();

    void use(GLuint texture_unit = 0);

private:
    bool initialised_;

    StandardShader* object_shader_;

    std::string path_;

    GLuint texture_id_;
    GLuint width_, height_;
    GLuint channels_;
};


#endif //INSIGHT_TEXTURE_H
