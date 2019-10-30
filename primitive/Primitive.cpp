#include "Primitive.h"

#include <iostream>

Primitive::~Primitive()
{
    std::cout << "Primitive::~Primitive()" << std::endl;

    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);

    if (vertices_ != nullptr)
    {
        delete[] vertices_;
    }
}

void Primitive::setActive()
{
    glBindVertexArray(vao_);
}

Primitive::Type Primitive::getType()
{
    return type_;
}

void Primitive::initialise()
{

}

