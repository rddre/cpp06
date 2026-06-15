#include "Data.hpp"
#include "Serializer.hpp"

#include <iostream>

int main(void)
{
	Data data;
	Data *originalPtr = &data;
	uintptr_t raw = Serializer::serialize(originalPtr);
	Data *restoredPtr = Serializer::deserialize(raw);

	std::cout << "original: " << originalPtr << std::endl;
	std::cout << "raw: " << raw << std::endl;
	std::cout << "restored: " << restoredPtr << std::endl;
	std::cout << "equal: " << (originalPtr == restoredPtr ? "true" : "false") << std::endl;
	return (0);
}
