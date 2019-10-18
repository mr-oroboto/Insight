#include "PrimitiveCollection.h"
#include "Cube.h"
#include "Triangle.h"
#include <iostream>

PrimitiveCollection::PrimitiveCollection(GLuint positionAttribute, GLuint colourAttribute)
{
    primitives[Type::CUBE] = new Cube(positionAttribute, colourAttribute);
    primitives[Type::TRIANGLE] = new Triangle(positionAttribute, colourAttribute);
}

PrimitiveCollection::~PrimitiveCollection()
{
    std::cout << "PrimitiveCollection::~PrimitiveCollection()" << std::endl;

    delete primitives[Type::CUBE];
    primitives[Type::CUBE] = nullptr;

    delete primitives[Type::TRIANGLE];
    primitives[Type::TRIANGLE] = nullptr;
}

Primitive* PrimitiveCollection::selectPrimitive(Type primitive)
{
    if (primitives.find(primitive) == primitives.end() || primitives[primitive] == nullptr)
    {
        return nullptr;
    }

    primitives[primitive]->setActive();

    return primitives[primitive];
}

