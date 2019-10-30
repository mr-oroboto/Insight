#include "PrimitiveCollection.h"

#include <iostream>

#include "Cube.h"
#include "Triangle.h"
#include "Line.h"
#include "Quad.h"
#include "Tesselation.h"
#include "Rectangle.h"
#include "shader/StandardShader.h"

PrimitiveCollection::PrimitiveCollection(StandardShader* shader)
{
    primitives_[Primitive::Type::CUBE] = new Cube(shader);
    primitives_[Primitive::Type::TRIANGLE] = new Triangle(shader);
    primitives_[Primitive::Type::LINE] = new Line(shader);
    primitives_[Primitive::Type::QUAD] = new Quad(shader);
    primitives_[Primitive::Type::TESSELATION] = new Tesselation(shader);
    primitives_[Primitive::Type::RECTANGLE] = new Rectangle(shader);
}

PrimitiveCollection::~PrimitiveCollection()
{
    std::cout << "PrimitiveCollection::~PrimitiveCollection()" << std::endl;

    delete primitives_[Primitive::Type::CUBE];
    primitives_[Primitive::Type::CUBE] = nullptr;

    delete primitives_[Primitive::Type::TRIANGLE];
    primitives_[Primitive::Type::TRIANGLE] = nullptr;

    delete primitives_[Primitive::Type::LINE];
    primitives_[Primitive::Type::LINE] = nullptr;

    delete primitives_[Primitive::Type::QUAD];
    primitives_[Primitive::Type::QUAD] = nullptr;

    delete primitives_[Primitive::Type::TESSELATION];
    primitives_[Primitive::Type::TESSELATION] = nullptr;

    delete primitives_[Primitive::Type::RECTANGLE];
    primitives_[Primitive::Type::RECTANGLE] = nullptr;
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

