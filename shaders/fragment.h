#ifndef INSIGHT_FRAGMENT_H
#define INSIGHT_FRAGMENT_H

#include "ShaderCollection.h"

const char* ShaderCollection::fragmentSource = R"glsl(
    #version 150 core

    in vec3 colour;
    out vec4 outColour;

    void main()
    {
        outColour = vec4(colour, 1.0);
    }
)glsl";

#endif //INSIGHT_FRAGMENT_H
