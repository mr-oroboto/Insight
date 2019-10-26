#ifndef INSIGHT_SHADER_SHADERCOLLECTION_H
#define INSIGHT_SHADER_SHADERCOLLECTION_H

class ShaderCollection
{
public:
    static const char* vertex_source_;
    static const char* fragment_source_;

    static const char* text_vertex_source_;
    static const char* text_fragment_source_;
};

#include "vertex.inc"
#include "fragment.inc"
#include "vertex_text.inc"
#include "fragment_text.inc"

#endif //INSIGHT_SHADER_SHADERCOLLECTION_H
