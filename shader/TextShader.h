#ifndef INSIGHT_SHADER_TEXTSHADER_H
#define INSIGHT_SHADER_TEXTSHADER_H

#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class TextShader : public Shader
{
public:
    TextShader();
    ~TextShader();

    bool initialise();

    GLuint getPositionAttribute();
    GLuint getTextureCoordsAttribute();

    void setViewTransform(const glm::mat4& transform);
    void setProjectionTransform(const glm::mat4& transform);
    void setTextColour(const glm::vec3& colour);

protected:
    static const char* vertex_source_;
    static const char* fragment_source_;

    GLuint position_attribute_;
    GLuint texture_coords_attribute_;

    GLuint uni_view_transform_;
    GLuint uni_projection_transform_;
    GLuint uni_text_colour_;
};

#endif //INSIGHT_SHADER_TEXTSHADER_H
