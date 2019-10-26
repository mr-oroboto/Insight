#ifndef INSIGHT_SHADER_FRAGMENT_H
#define INSIGHT_SHADER_FRAGMENT_H

#include "ShaderCollection.h"

const char* ShaderCollection::fragment_source_ = R"glsl(
    #version 150 core

    in vec3 colour;
    out vec4 outColour;

    void main()
    {
        outColour = vec4(colour, 1.0);
    }
)glsl";

#endif //INSIGHT_SHADER_FRAGMENT_H
