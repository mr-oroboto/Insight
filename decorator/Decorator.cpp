#include "Decorator.h"

#include <iostream>

Decorators::Decorator::~Decorator()
{
    std::cout << "Decorator::~Decorator()" << std::endl;
}