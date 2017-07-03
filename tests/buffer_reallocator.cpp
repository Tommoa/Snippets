#include "../buffer/buffer.hpp"

#include <iostream>

using namespace Snippets;

int main() {
	std::cout << std::endl;

	size_t base_buffer_size = 64;
	std::cout << "Allocating base buffer of size: " << base_buffer_size
			  << " bytes" << std::endl;
	buffer base(base_buffer_size, &buffer::default_allocator);
	std::cout << "\tSuccess" << std::endl;
	std::cout << std::endl;

	size_t buffer_size = 8;
	std::cout << "Creating buffer with custom allocator of size: "
			  << buffer_size << " bytes" << std::endl;
	buffer buf(buffer_size, &buffer::recursive_allocator, &base);
	std::cout << "\tSuccess" << std::endl;
	std::cout << std::endl;

	std::cout << "Creating int pointers to the beginning of buffer with custom "
				 "allocator and base buffer"
			  << std::endl;
	int *base_int = (int *)base.offset(0);
	int *buf_int = (int *)buf.offset(0);
	*buf_int = rand();
	std::cout << "\tSetting custom allocator int to " << *buf_int << std::endl;
	std::cout << std::endl;
	std::cout << "\tOverlapping buffer int: " << *base_int << std::endl;
	std::cout << std::endl;
}
