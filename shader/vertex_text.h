#ifndef INSIGHT_VERTEXTEXT_H
#define INSIGHT_VERTEXTEXT_H

#include "ShaderCollection.h"

const char* ShaderCollection::textVertexSource = R"glsl(
    #version 330 core

    in vec3 position;
    in vec2 inTexCoords;

    out vec2 TexCoords;

    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * vec4(position.x, position.y, position.z, 1.0);   // simple matrix * column vector
        TexCoords = inTexCoords;
    }
)glsl";

#endif //INSIGHT_VERTEXTEXT_H
