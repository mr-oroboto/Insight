#include "StandardShader.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>         // convert matrix to float

StandardShader::StandardShader()
{
}

StandardShader::~StandardShader()
{
    std::cout << "StandardShader::~StandardShader()" << std::endl;
}

bool StandardShader::initialise()
{
    try
    {
        if ( ! Shader::initialise(vertex_source_, fragment_source_))
        {
            throw "Can't initialise base shader";
        }

        GLint reference;

        reference = glGetAttribLocation(shader_program_, "position");
        if (reference < 0)
        {
            throw "Can't find position attribute in shader program";
        }
        position_attribute_ = static_cast<GLuint>(reference);

        reference = glGetAttribLocation(shader_program_, "inNormal");
        if (reference < 0)
        {
            throw "Can't find normal attribute in shader program";
        }
        normal_attribute_ = static_cast<GLuint>(reference);

        // Add an entirely new attribute mapping for the model colour data we added to the vertex attributes
        reference = glGetAttribLocation(shader_program_, "inColour");
        if (reference < 0)
        {
            throw "Can't find colour attribute in shader program";
        }
        colour_attribute_ = static_cast<GLuint>(reference);

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


const char* StandardShader::vertex_source_ = R"glsl(
    #version 150 core

    in vec3 position;
    in vec3 inNormal;
    in vec3 inColour;

    out vec3 colour;
    out vec3 normal;
    out vec3 fragmentPosition;      // in world (not normalised device) co-ords for lighting

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform int doOverrideColour;
    uniform vec3 overrideColour;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0);  // simple matrix * column vector
        fragmentPosition = vec3(model * vec4(position, 1.0));
//      normal = inNormal;                                              // doesn't support object scaling
        normal = mat3(transpose(inverse(model))) * inNormal;

        if (doOverrideColour == 1)
        {
           colour = overrideColour;
        }
        else
        {
           colour = inColour;
        }
    }
)glsl";

const char* StandardShader::fragment_source_ = R"glsl(
    #version 150 core

    in vec3 colour;                     // object / vertex colour
    in vec3 normal;                     // normal to vertex face
    in vec3 fragmentPosition;           // vertex position in world (not normalised device) co-ords

    uniform int lightingOn;
    uniform vec3 lightPosition;
    uniform vec3 lightColour;
    uniform float lightIntensity;
    uniform vec3 cameraPosition;        // view / camera location in world co-ords

    out vec4 outColour;                 // final fragment colour

    void main()
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

        vec3 fragmentColour = (ambient + diffuse + specular) * colour;

        if (lightingOn == 0)
        {
            fragmentColour = colour;
        }

        outColour = vec4(fragmentColour, 1.0);
    }
)glsl";