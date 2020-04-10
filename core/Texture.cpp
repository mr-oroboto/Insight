#include "Texture.h"

#include "stb_image.h"

Texture::Texture(const std::string& path, insight::shader::StandardShader* shader)
{
    path_ = path;
    object_shader_ = shader;
    width_ = height_ = channels_ = 0;

    initialised_ = false;
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture_id_);
}

bool Texture::initialise()
{
    if (initialised_)
    {
        return true;
    }

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char* img_data = stbi_load(path_.c_str(), &width, &height, &channels, 0);

    if (img_data)
    {
        width_ = static_cast<GLuint>(width);
        height_ = static_cast<GLuint>(height);
        channels_ = static_cast<GLuint>(channels);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(img_data);

        initialised_ = true;
    }

    return initialised_;
}

void Texture::use(GLuint texture_unit)
{
    if ( ! initialised_)
    {
        return;
    }

    assert(texture_unit == 0);  // need to update glActiveTexture() below if not

    // Tell the shader to associate the sampler with the specified texture unit (default: 0)
    object_shader_->setTextureSamplerTextureUnit(texture_unit);

    // Activate texture unit 0 so we can bind this texture to it. OpenGL has at least 16 texture units for simultaneous use.
    glActiveTexture(GL_TEXTURE0);

    // Bind this texture to the texture unit (which is implicitly referenced by the sampler in the fragment shader)
    glBindTexture(GL_TEXTURE_2D, texture_id_);
}