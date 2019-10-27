#include "PrimitiveCollection.h"

#include <iostream>

#include "Cube.h"
#include "Triangle.h"
#include "Line.h"
#include "Quad.h"
#include "Tesselation.h"

PrimitiveCollection::PrimitiveCollection(GLuint position_attribute, GLuint normal_attribute, GLuint colour_attribute)
{
    primitives_[Primitive::Type::CUBE] = new Cube(position_attribute, normal_attribute, colour_attribute);
    primitives_[Primitive::Type::TRIANGLE] = new Triangle(position_attribute, normal_attribute, colour_attribute);
    primitives_[Primitive::Type::LINE] = new Line(position_attribute, normal_attribute, colour_attribute);
    primitives_[Primitive::Type::QUAD] = new Quad(position_attribute, normal_attribute, colour_attribute);
    primitives_[Primitive::Type::TESSELATION] = new Tesselation(position_attribute, normal_attribute, colour_attribute);
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

