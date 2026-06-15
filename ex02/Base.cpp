#include "Base.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

Base::~Base()
{
}

Base *generate(void)
{
	static bool seeded = false;
	if (!seeded)
	{
		std::srand(static_cast<unsigned int>(std::time(0)));
		seeded = true;
	}

	switch (std::rand() % 3)
	{
		case 0:
			return (new A);
		case 1:
			return (new B);
		default:
			return (new C);
	}
}

void identify(Base *p)
{
	if (dynamic_cast<A *>(p) != 0)
		std::cout << "A" << std::endl;
	else if (dynamic_cast<B *>(p) != 0)
		std::cout << "B" << std::endl;
	else if (dynamic_cast<C *>(p) != 0)
		std::cout << "C" << std::endl;
	else
		std::cout << "Unknown" << std::endl;
}

void identify(Base &p)
{
	try
	{
		(void)dynamic_cast<A &>(p);
		std::cout << "A" << std::endl;
		return;
	}
	catch (...)
	{
	}
	try
	{
		(void)dynamic_cast<B &>(p);
		std::cout << "B" << std::endl;
		return;
	}
	catch (...)
	{
	}
	try
	{
		(void)dynamic_cast<C &>(p);
		std::cout << "C" << std::endl;
		return;
	}
	catch (...)
	{
	}
}