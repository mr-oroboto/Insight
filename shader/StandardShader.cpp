#include "StandardShader.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>         // convert matrix to float

namespace insight {
namespace shader {
    
bool StandardShader::initialise()
{
    try
    {
        if ( ! Shader::initialise(vertex_source_, fragment_source_))
        {
            throw "Can't initialise base shader";
        }

        // These values come from the layout statements in the shader's GLSL program
        position_attribute_ = 0;
        normal_attribute_ = 1;
        colour_attribute_ = 2;
        texture_coords_attribute_ = 3;

        GLint reference;

        reference = glGetUniformLocation(shader_program_, "model");
        if (reference < 0)
        {
            throw "Can't find model transform in shader program";
        }
        uni_model_transform_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "view");
        if (reference < 0)
        {
            throw "Can't find view transform in shader program";
        }
        uni_view_transform_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "projection");
        if (reference < 0)
        {
            throw "Can't find projection transform in shader program";
        }
        uni_projection_transform_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "cameraPosition");
        if (reference < 0)
        {
            throw "Can't find view / camera position in shader program";
        }
        uni_camera_coords_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "doOverrideColour");
        if (reference < 0)
        {
            throw "Can't find model colour override toggle in shader program";
        }
        uni_model_do_override_colour_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "overrideColour");
        if (reference < 0)
        {
            throw "Can't find model override colour in shader program";
        }
        uni_model_override_colour_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "lightingOn");
        if (reference < 0)
        {
            throw "Can't find light control in shader program";
        }
        uni_lighting_on_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "lightColour");
        if (reference < 0)
        {
            throw "Can't find light colour in shader program";
        }
        uni_light_colour_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "lightIntensity");
        if (reference < 0)
        {
            throw "Can't find light intensity in shader program";
        }
        uni_light_intensity_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "lightPosition");
        if (reference < 0)
        {
            throw "Can't find light position in shader program";
        }
        uni_light_coords_ = static_cast<GLuint>(reference);

        reference = glGetUniformLocation(shader_program_, "texturesOn");
        if (reference < 0)
        {
            throw "Can't find texture control in shader program";
        }
        uni_textures_on_ = static_cast<GLuint>(reference);

        initialised_ = true;
    }
    catch (const char* exception)
    {
        std::cerr << exception << std::endl;
    }

    return initialised_;
}

void StandardShader::setCameraCoords(const glm::vec3 &world_coords)
{
    use();
    glUniform3f(uni_camera_coords_, world_coords.x, world_coords.y, world_coords.z);
}

void StandardShader::setLightingOn(bool on)
{
    use();
    glUniform1i(uni_lighting_on_, on ? 1 : 0);
}

void StandardShader::setLightCoords(const glm::vec3 &world_coords)
{
    use();
    glUniform3f(uni_light_coords_, world_coords.x, world_coords.y, world_coords.z);
}

void StandardShader::setLightColour(const glm::vec3 &colour, GLfloat intensity)
{
    use();
    glUniform3f(uni_light_colour_, colour.r, colour.g, colour.b);
    glUniform1f(uni_light_intensity_, intensity);
}

void StandardShader::setProjectionTransform(const glm::mat4& transform)
{
    use();
    glUniformMatrix4fv(uni_projection_transform_, 1, GL_FALSE, glm::value_ptr(transform));
}

void StandardShader::setViewTransform(const glm::mat4& transform)
{
    use();
    glUniformMatrix4fv(uni_view_transform_, 1, GL_FALSE, glm::value_ptr(transform));
}

void StandardShader::setModelTransform(const glm::mat4& transform)
{
    use();
    glUniformMatrix4fv(uni_model_transform_, 1, GL_FALSE, glm::value_ptr(transform));
}

void StandardShader::setOverrideModelColour(bool override, const glm::vec3& colour)
{
    use();
    if (override)
    {
        glUniform1i(uni_model_do_override_colour_, 1);
        glUniform3f(uni_model_override_colour_, colour.r, colour.g, colour.b);
    }
    else
    {
        glUniform1i(uni_model_do_override_colour_, 0);
    }
}

void StandardShader::setTexturesOn(bool on)
{
    use();
    glUniform1i(uni_textures_on_, on ? 1 : 0);
}

void StandardShader::setTextureSamplerTextureUnit(GLuint texture_unit)
{
    // This shader has one texture sampler, tell it which texture unit to sample from (Texture assigns the texture to that unit)
    use();
    glUniform1i(uni_texture_sampler_, texture_unit);
}

GLuint StandardShader::getPositionAttribute()
{
    return position_attribute_;
}

GLuint StandardShader::getNormalAttribute()
{
    return normal_attribute_;
}

GLuint StandardShader::getColourAttribute()
{
    return colour_attribute_;
}

GLuint StandardShader::getTextureCoordsAttribute()
{
    return texture_coords_attribute_;
}


const char* StandardShader::vertex_source_ = R"glsl(
    #version 330 core

    layout (location = 0) in vec3 inPosition;
    layout (location = 1) in vec3 inNormal;
    layout (location = 2) in vec3 inColour;
    layout (location = 3) in vec2 inTexCoords;

    out vec3 colour;
    out vec3 normal;
    out vec3 fragmentPosition;      // in world (not normalised device) co-ords for lighting
    out vec2 texCoords;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform int doOverrideColour;
    uniform vec3 overrideColour;

    void main()
    {
        gl_Position = projection * view * model * vec4(inPosition, 1.0);  // simple matrix * column vector

        fragmentPosition = vec3(model * vec4(inPosition, 1.0));

        normal = mat3(transpose(inverse(model))) * inNormal;

        if (doOverrideColour == 1)
        {
           colour = overrideColour;
        }
        else
        {
           colour = inColour;
        }

        texCoords = inTexCoords;
    }
)glsl";

const char* StandardShader::fragment_source_ = R"glsl(
    #version 330 core

    in vec3 colour;                     // object / vertex colour
    in vec3 normal;                     // normal to vertex face
    in vec3 fragmentPosition;           // vertex position in world (not normalised device) co-ords
    in vec2 texCoords;

    uniform int lightingOn;
    uniform vec3 lightPosition;
    uniform vec3 lightColour;
    uniform float lightIntensity;
    uniform vec3 cameraPosition;        // view / camera location in world co-ords

    uniform int texturesOn;
    uniform sampler2D textureSampler;

    out vec4 outColour;                 // final fragment colour

    void main()
    {
        vec3 fragmentColour = colour;

        if (lightingOn == 1)
        {
            float specularStrength = 0.5;

            vec3 ambient = lightIntensity * lightColour;

            // Calculate the direction vector between the light source and the fragment
            vec3 lightingDirection = normalize(lightPosition - fragmentPosition);

            // Calculate the angle (actually just dot product) between the light direction vector and the fragment face's normal
            vec3 normalisedNormal = normalize(normal);
            float diffuseIntensity = max(dot(normalisedNormal, lightingDirection), 0.0);

            // Calculate the diffuse light intensity based on the angle between the light and the normal
            vec3 diffuse = diffuseIntensity * lightColour;

            // Calculate the direction vector between the camera / view location and the fragment (to determine reflection intensity)
            vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
            vec3 reflectionDirection = reflect(-lightingDirection, normalisedNormal);

            float specularIntensity = pow(max(dot(cameraDirection, reflectionDirection), 0.0), 32);   // 32 is the shininess of the object
            vec3 specular = specularStrength * specularIntensity * lightColour;

            fragmentColour = (ambient + diffuse + specular) * colour;
        }

        outColour = vec4(fragmentColour, 1.0);

        if (texturesOn == 1)
        {
            outColour = texture(textureSampler, texCoords) * vec4(fragmentColour, 1.0);
        }
    }
)glsl";

}   // namespace shader
}   // namespace insight