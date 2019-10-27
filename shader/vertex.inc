#ifndef INSIGHT_SHADER_VERTEX_H
#define INSIGHT_SHADER_VERTEX_H

#include "ShaderCollection.h"

const char* ShaderCollection::vertex_source_ = R"glsl(
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

#endif // INSIGHT_SHADER_VERTEX_H