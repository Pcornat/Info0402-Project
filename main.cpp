#include <iostream>
#include "Set.hpp"

int main() {
	Set<int> s;
	std::cout << "Hello, World!" << std::endl;
	s.insert(5);
	std::cout << *(s.begin()) << " size : " << s.getSize() << std::endl;
	return EXIT_SUCCESS;
}