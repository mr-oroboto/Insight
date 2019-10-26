#include "PrimitiveCollection.h"

#include <iostream>

#include "Cube.h"
#include "Triangle.h"
#include "Line.h"
#include "Quad.h"
#include "Tesselation.h"

PrimitiveCollection::PrimitiveCollection(GLuint positionAttribute, GLuint colourAttribute)
{
    primitives[Primitive::Type::CUBE] = new Cube(positionAttribute, colourAttribute);
    primitives[Primitive::Type::TRIANGLE] = new Triangle(positionAttribute, colourAttribute);
    primitives[Primitive::Type::LINE] = new Line(positionAttribute, colourAttribute);
    primitives[Primitive::Type::QUAD] = new Quad(positionAttribute, colourAttribute);
    primitives[Primitive::Type::TESSELATION] = new Tesselation(positionAttribute, colourAttribute);
}

PrimitiveCollection::~PrimitiveCollection()
{
    std::cout << "PrimitiveCollection::~PrimitiveCollection()" << std::endl;

    delete primitives[Primitive::Type::CUBE];
    primitives[Primitive::Type::CUBE] = nullptr;

    delete primitives[Primitive::Type::TRIANGLE];
    primitives[Primitive::Type::TRIANGLE] = nullptr;

    delete primitives[Primitive::Type::LINE];
    primitives[Primitive::Type::LINE] = nullptr;

    delete primitives[Primitive::Type::QUAD];
    primitives[Primitive::Type::QUAD] = nullptr;

    delete primitives[Primitive::Type::TESSELATION];
    primitives[Primitive::Type::TESSELATION] = nullptr;
}

Primitive* PrimitiveCollection::selectPrimitive(Primitive::Type primitive)
{
    if (primitives.find(primitive) == primitives.end() || primitives[primitive] == nullptr)
    {
        return nullptr;
    }

    primitives[primitive]->setActive();

    return primitives[primitive];
}

