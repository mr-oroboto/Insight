#include "Primitive.h"

#include <iostream>

Primitive::~Primitive()
{
    std::cout << "Primitive::~Primitive()" << std::endl;

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    if (vertices != nullptr)
    {
        delete[] vertices;
    }
}

void Primitive::setActive()
{
    glBindVertexArray(vao);
}

Primitive::Type Primitive::getType()
{
    return type;
}

