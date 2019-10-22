#ifndef INSIGHT_FRAGMENT_TEXT_H
#define INSIGHT_FRAGMENT_TEXT_H

#include "ShaderCollection.h"

const char* ShaderCollection::textFragmentSource = R"glsl(
    #version 330 core

    in vec2 TexCoords;
    out vec4 outColour;

    uniform sampler2D text;     // mono coloured bitmap image of glyph
    uniform vec3 textColour;    // to adjust final text colour

    void main()
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);     // all stored in red component
        outColour = vec4(textColour, 1.0) * sampled;
    }
)glsl";

#endif //INSIGHT_FRAGMENT_TEXT_H
