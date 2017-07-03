#include "../buffer/buffer.hpp"

#include <iostream>

using namespace Snippets;

int main() {
	std::cout << std::endl;

	std::cout << "Size of buffer class: " << sizeof(buffer) << " bytes"
			  << std::endl;
	std::cout << std::endl;

	std::cout << "Allocating buffer of size 8 bytes (2 ints)" << std::endl;
	buffer buf(sizeof(int) * 2);
	std::cout << "\tSuccess" << std::endl;

	std::cout << std::endl;

	std::cout << "Allocating int my_int in buffer" << std::endl;
	int* my_int = (int*)buf.alloc<int>();
	std::cout << "\tSuccess" << std::endl;
	*my_int = 4;
	std::cout << "\t*my_int: " << *my_int << std::endl;
	std::cout << std::endl;

	std::cout << "Allocating int my_second_int in buffer" << std::endl;
	int* my_second_int = (int*)buf.alloc(sizeof(int));
	std::cout << "\tSuccess" << std::endl;
	*my_second_int = 3;
	std::cout << "\t*my_second_int: " << *my_second_int << std::endl;
	std::cout << std::endl;

	std::cout << "Writing buffer to file 'test'" << std::endl;
	buf.save("test");
	std::cout << "\tSuccess" << std::endl;
	std::cout << std::endl;

	std::cout << "Creating new_buffer from file 'test'" << std::endl;
	buffer new_buffer("test");
	std::cout << "\tSuccess" << std::endl;
	std::cout << std::endl;

	std::cout << "Setting my_int and my_second_int to refer to the new_buffer "
				 "with offsets 0, 4"
			  << std::endl;
	my_int = (int*)new_buffer.offset(0);
	my_second_int = (int*)new_buffer.offset(4);
	std::cout << "\tmy_int: " << *my_int << std::endl;
	std::cout << "\tmy_second_int: " << *my_second_int << std::endl;
	std::cout << std::endl;

	std::cout
		<< "Attempting to throw allocation error by allocating a long on buffer"
		<< std::endl;
	try {
		long* my_long = (long*)buf.alloc<long>();
		std::cout << "\tAllocation error not caught" << std::endl;
	} catch (AllocationError ex) {
		std::cout << "\tCaught allocation error" << std::endl;
	}
	std::cout << std::endl;

	return 0;
}
