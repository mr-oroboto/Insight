#ifndef INSIGHT_SHADERCOLLECTION_H
#define INSIGHT_SHADERCOLLECTION_H

class ShaderCollection
{
public:
    static const char* vertexSource;
    static const char* fragmentSource;

    static const char* textVertexSource;
    static const char* textFragmentSource;
};

#include "vertex.h"
#include "fragment.h"
#include "vertex_text.h"
#include "fragment_text.h"

#endif //INSIGHT_SHADERCOLLECTION_H
