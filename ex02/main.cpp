#include "Base.hpp"

#include <iostream>

int main(void)
{
	Base *ptr = generate();

	std::cout << "Pointer identify: ";
	identify(ptr);
	std::cout << "Reference identify: ";
	identify(*ptr);
	delete ptr;
	return (0);
}
