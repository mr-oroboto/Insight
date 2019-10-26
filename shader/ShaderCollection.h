#ifndef INSIGHT_SHADER_SHADERCOLLECTION_H
#define INSIGHT_SHADER_SHADERCOLLECTION_H

class ShaderCollection
{
public:
    static const char* vertexSource;
    static const char* fragmentSource;

    static const char* textVertexSource;
    static const char* textFragmentSource;
};

#include "vertex.inc"
#include "fragment.inc"
#include "vertex_text.inc"
#include "fragment_text.inc"

#endif //INSIGHT_SHADER_SHADERCOLLECTION_H
