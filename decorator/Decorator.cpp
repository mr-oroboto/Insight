#include "Decorator.h"

#include <iostream>

Decorator::~Decorator()
{
    std::cout << "Decorator::~Decorator()" << std::endl;
}