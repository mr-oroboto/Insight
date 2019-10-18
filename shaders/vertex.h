#ifndef INSIGHT_VERTEX_H
#define INSIGHT_VERTEX_H

#include "ShaderCollection.h"

const char* ShaderCollection::vertexSource = R"glsl(
    #version 150 core

    in vec3 position;
    in vec3 inColour;

    out vec3 colour;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0);   // simple matrix * column vector
        colour = inColour;
    }
)glsl";

#endif // INSIGHT_VERTEX_H