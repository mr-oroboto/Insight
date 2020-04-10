#ifndef INSIGHT_SHADER_STANDARDSHADER_H
#define INSIGHT_SHADER_STANDARDSHADER_H

#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace insight {
namespace shader {

class StandardShader : public Shader {
public:
    StandardShader() = default;
    ~StandardShader() = default;

    bool initialise();

    GLuint getPositionAttribute();

    GLuint getNormalAttribute();
    GLuint getColourAttribute();
    GLuint getTextureCoordsAttribute();

    void setCameraCoords(const glm::vec3 &world_coords);

    void setLightingOn(bool on);
    void setLightCoords(const glm::vec3 &world_coords);
    void setLightColour(const glm::vec3 &colour, GLfloat intensity = 1.0f);

    void setModelTransform(const glm::mat4 &transform);
    void setViewTransform(const glm::mat4 &transform);
    void setProjectionTransform(const glm::mat4 &transform);

    void setOverrideModelColour(bool override, const glm::vec3 &colour = glm::vec3(1.0f, 1.0f, 1.0f));

    void setTexturesOn(bool on);
    void setTextureSamplerTextureUnit(GLuint texture_unit);

protected:
    static const char *vertex_source_;
    static const char *fragment_source_;

    GLuint position_attribute_;
    GLuint normal_attribute_;
    GLuint colour_attribute_;
    GLuint texture_coords_attribute_;

    GLuint uni_model_transform_;
    GLuint uni_model_do_override_colour_;
    GLuint uni_model_override_colour_;

    GLuint uni_camera_coords_;
    GLuint uni_view_transform_;
    GLuint uni_projection_transform_;

    GLuint uni_lighting_on_;
    GLuint uni_light_colour_;
    GLuint uni_light_coords_;
    GLuint uni_light_intensity_;

    GLuint uni_textures_on_;
    GLuint uni_texture_sampler_;
};

}   // namespace shader
}   // namespace insight

#endif //INSIGHT_SHADER_STANDARDSHADER_H
