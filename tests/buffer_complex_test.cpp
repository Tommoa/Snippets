#include "../buffer/buffer_complex.hpp"

#include <fstream>
#include <iostream>

using namespace Snippets;

int main() {
	std::cout << std::endl << "Start of complex buffer test" << std::endl;
	std::cout << std::endl;
	std::cout << "size of buffer: " << sizeof(buffer_complex) << std::endl
			  << std::endl;
	Snippets::buffer_complex complex_buffer =
		Snippets::buffer_complex(); // Will use malloc, has no special traits.
	std::cout << "Assigned new complex buffer with malloc" << std::endl;
	int *test_int_1 = (int *)complex_buffer.allocate(sizeof(int));
	long *test_long_1 = (long *)complex_buffer.allocate(sizeof(long));
	std::cout << "Allocated a new int and a new long on the buffer"
			  << std::endl;
	*test_int_1 = rand();
	*test_long_1 = rand();
	std::cout << "\ttest_int_1: " << *test_int_1 << std::endl;
	std::cout << "\ttestlong_1: " << *test_long_1 << std::endl;
	std::cout << std::endl;

	std::ofstream out("buffer_complex_test");
	complex_buffer.save(out);
	out.close();

	std::cout << "Saved buffer to file" << std::endl;

	std::ifstream in("buffer_complex_test");
	buffer_complex cb = buffer_complex();
	cb.load(in);
	in.close();

	std::cout << "Read buffer to new buffer 'cb'" << std::endl;
	std::cout << std::endl;

	std::cout << "Attempting to read back the int and the long" << std::endl;
	test_int_1 = (int *)cb.offset(0);
	test_long_1 = (long *)cb.offset(4);
	std::cout << *test_int_1 << std::endl << *test_long_1 << std::endl;
}
