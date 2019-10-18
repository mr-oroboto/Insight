#ifndef INSIGHT_PRIMITIVECOLLECTION_H
#define INSIGHT_PRIMITIVECOLLECTION_H

#include <unordered_map>
#include <GL/glew.h>                    // must be included before gl.h (which is via SDL_opengl.h)
#include "Primitive.h"

class PrimitiveCollection
{
public:
    PrimitiveCollection() {}
    PrimitiveCollection(GLuint positionAttribute, GLuint colourAttribute);
    virtual ~PrimitiveCollection();

    enum Type
    {
        CUBE = 0,
        TRIANGLE
    };

    Primitive* selectPrimitive(Type primitive);

private:
    std::unordered_map<Type, Primitive*> primitives;
};

#endif //INSIGHT_PRIMITIVECOLLECTION_H
