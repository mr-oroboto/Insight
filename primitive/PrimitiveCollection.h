#ifndef INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H
#define INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H

#include <unordered_map>

#include <GL/glew.h>

#include "Primitive.h"
#include "shader/StandardShader.h"

namespace insight {
namespace primitive {

class PrimitiveCollection {
public:
    PrimitiveCollection(shader::StandardShader* object_shader);
    ~PrimitiveCollection();

    Primitive* selectPrimitive(Primitive::Type primitive);
    bool addPrimitive(Primitive::Type primitive_type, Primitive* primitive);

private:
    std::unordered_map<Primitive::Type, Primitive*> primitives_;
};

}   // namespace primitive
}   // namespace insight

#endif //INSIGHT_PRIMITIVE_PRIMITIVECOLLECTION_H
