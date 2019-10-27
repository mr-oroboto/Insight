#ifndef INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H
#define INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H

#include <unordered_map>

#include <GL/glew.h>

#include "Primitive.h"

class PrimitiveCollection
{
public:
    PrimitiveCollection(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute);
    ~PrimitiveCollection();

    Primitive* selectPrimitive(Primitive::Type primitive);

private:
    std::unordered_map<Primitive::Type, Primitive*> primitives_;
};

#endif //INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H
