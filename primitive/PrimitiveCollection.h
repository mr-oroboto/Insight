#ifndef INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H
#define INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H

#include <unordered_map>

#include <GL/glew.h>

#include "Primitive.h"

class PrimitiveCollection
{
public:
    PrimitiveCollection(GLuint positionAttribute, GLuint colourAttribute);
    ~PrimitiveCollection();

    Primitive* selectPrimitive(Primitive::Type primitive);

private:
    std::unordered_map<Primitive::Type, Primitive*> primitives;
};

#endif //INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H
