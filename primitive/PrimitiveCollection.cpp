#include "PrimitiveCollection.h"
#include "Cube.h"
#include "Triangle.h"
#include <iostream>

PrimitiveCollection::PrimitiveCollection(GLuint positionAttribute, GLuint colourAttribute)
{
    primitives[Primitive::Type::CUBE] = new Cube(positionAttribute, colourAttribute);
    primitives[Primitive::Type::TRIANGLE] = new Triangle(positionAttribute, colourAttribute);
}

PrimitiveCollection::~PrimitiveCollection()
{
    std::cout << "PrimitiveCollection::~PrimitiveCollection()" << std::endl;

    delete primitives[Primitive::Type::CUBE];
    primitives[Primitive::Type::CUBE] = nullptr;

    delete primitives[Primitive::Type::TRIANGLE];
    primitives[Primitive::Type::TRIANGLE] = nullptr;
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

