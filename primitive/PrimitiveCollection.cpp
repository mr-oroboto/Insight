#include "PrimitiveCollection.h"

#include "Cube.h"
#include "Triangle.h"
#include "Line.h"
#include "Quad.h"
#include "Tesselation.h"
#include "Rectangle.h"
#include "TransformingRectangle.h"

namespace insight {
namespace primitive {

    
PrimitiveCollection::PrimitiveCollection(shader::StandardShader* shader)
{
    primitives_[Primitive::Type::CUBE] = new Cube(shader);
    primitives_[Primitive::Type::TRIANGLE] = new Triangle(shader);
    primitives_[Primitive::Type::LINE] = new Line(shader);
    primitives_[Primitive::Type::QUAD] = new Quad(shader);
    primitives_[Primitive::Type::RECTANGLE] = new Rectangle(shader);
    primitives_[Primitive::Type::TRANSFORMING_RECTANGLE] = new TransformingRectangle(shader);
}

PrimitiveCollection::~PrimitiveCollection()
{
    for (std::unordered_map<Primitive::Type, Primitive*>::iterator i = primitives_.begin(); i != primitives_.end(); i++)
    {
        delete reinterpret_cast<Primitive*>(i->second);
        primitives_[i->first] = nullptr;
    }
}

bool PrimitiveCollection::addPrimitive(Primitive::Type primitive_type, Primitive* primitive)
{
    if (primitives_.find(primitive_type) != primitives_.end())
    {
        return false;
    }

    primitives_[primitive_type] = primitive;

    return true;
}

Primitive* PrimitiveCollection::selectPrimitive(Primitive::Type primitive)
{
    if (primitives_.find(primitive) == primitives_.end() || primitives_[primitive] == nullptr)
    {
        return nullptr;
    }

    primitives_[primitive]->setActive();

    return primitives_[primitive];
}


}   // namespace primitive
}   // namespace insight