#ifndef INSIGHT_VERTEXTEXT_H
#define INSIGHT_VERTEXTEXT_H

#include "ShaderCollection.h"

const char* ShaderCollection::textVertexSource = R"glsl(
    #version 330 core

    layout (location = 0) in vec4 vertex;       // <vec2 pos, vec2 tex>
    out vec2 TexCoords;

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);   // simple matrix * column vector
        TexCoords = vertex.zw;
    }
)glsl";

#endif //INSIGHT_VERTEXTEXT_H
