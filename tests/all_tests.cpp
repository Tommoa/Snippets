#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include "../buffer/buffer.hpp"
#include "../buffer/buffer_complex.hpp"
#include "../rptr/rptr.hpp"

using namespace Snippets;

void buffer_complex_test() {
	std::cout << std::endl << "Start of complex buffer test" << std::endl;
	std::cout << std::endl;
	std::cout << "size of buffer: " << sizeof(buffer_complex) << std::endl
			  << std::endl;
	Snippets::buffer_complex complex_buffer =
		Snippets::buffer_complex(); // Will use malloc, has no special traits.
	std::cout << "Assigned new complex buffer with malloc" << std::endl;
	int* test_int_1 = (int*)complex_buffer.allocate(sizeof(int));
	long* test_long_1 = (long*)complex_buffer.allocate(sizeof(long));
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
	test_int_1 = (int*)cb.offset(0);
	test_long_1 = (long*)cb.offset(4);
	std::cout << *test_int_1 << std::endl << *test_long_1 << std::endl;
}

void rptr_test() {
	typedef struct pen {
		int t;
		rptr<pen, long> ptr;
	} pen;

	std::cout << std::endl;

	typedef struct {
		int t;
		rptr<int, int> ptr;
	} packed;

	typedef struct {
		int t;
		int* ptr;
	} unpacked;

	int a = rand();
	int b = rand();

	std::cout << "Original values: " << std::endl
			  << "\ta: " << a << std::endl
			  << "\tb: " << b << std::endl
			  << std::endl;

	packed t;
	t.t = a;
	t.ptr = &b;

	unpacked s;
	s.t = a;
	s.ptr = &b;

	std::cout << "Relative pointer struct"
			  << " has size of: " << sizeof(t) << std::endl;
	std::cout << "\tstruct.t: " << t.t << std::endl;
	std::cout << "\tstruct.ptr: " << *t.ptr << std::endl;
	std::cout << std::endl;

	std::cout << "Standard pointer struct"
			  << " has size of: " << sizeof(s) << std::endl;
	std::cout << "\tstruct.t: " << s.t << std::endl;
	std::cout << "\tstruct.ptr: " << *s.ptr << std::endl;
	std::cout << std::endl;

	std::cout << std::endl;

	std::cout << "Relative pointer relative to start of struct" << std::endl;
	t.ptr = rptr<int, int>(&b, (void*)&t);
	std::cout << "\tstruct.ptr: " << t.ptr * &t << std::endl;
	std::cout << std::endl;

	std::cout << std::endl;

	std::cout << "Relative pointer to a struct" << std::endl;
	pen p1, p2;
	p1.t = a;
	p2.t = b;
	std::cout << "\tp1.t: " << p1.t << std::endl;
	std::cout << "\tp2.t: " << p2.t << std::endl;
	std::cout << std::endl;

	p1.ptr = rptr<pen, long>(&p2, (void*)&p1);
	p2.ptr = rptr<pen, long>(&p1, (void*)&p2);
	std::cout << "\tp1.ptr.t: " << (p1.ptr * &p1).t << std::endl;
	std::cout << "\tp2.ptr.t: " << (p2.ptr * &p2).t << std::endl;
	std::cout << "\tp1.ptr.ptr.t: " << ((p1.ptr * &p1).ptr * &p2).t
			  << std::endl;
	std::cout << "\tp2.ptr.ptr.t: " << ((p2.ptr * &p2).ptr * &p1).t
			  << std::endl;
	std::cout << std::endl;

	std::cout << std::endl;

	std::cout << "Relative pointer into a buffer of nulled chars" << std::endl;
	char buf[8] = {0};
	rptr<int, int> intptr;
	intptr = rptr<int, int>((int*)&buf, (void*)&buf);
	intptr.assign(rand(), &buf);
	std::cout << "\tAssigned " << intptr * &buf << " to the char array"
			  << std::endl;
	std::cout << std::endl;
	std::cout << "Char buffer contents:" << std::endl;
	for (int i = 0; i < 8; i++) {
		std::cout << "\t" << i << ": " << buf[i] << std::endl;
	}
	std::cout << std::endl;
	rptr<int, int> nptr((int*)&buf, (void*)&buf);
	std::cout << "Pulled " << nptr * &buf << " out of the char buffer"
			  << std::endl;

	std::cout << std::endl;
}

void buffer_custom_allocators() {
	std::cout << std::endl;

	std::size_t base_buffer_size = 64;
	std::cout << "Allocating base buffer of size: " << base_buffer_size
			  << " bytes" << std::endl;
	buffer base(base_buffer_size, &buffer::default_allocator);
	std::cout << "\tSuccess" << std::endl;
	std::cout << std::endl;

	std::size_t buffer_size = 8;
	std::cout << "Creating buffer with custom allocator of size: "
			  << buffer_size << " bytes" << std::endl;
	buffer buf(buffer_size, &buffer::recursive_allocator, &base);
	std::cout << "\tSuccess" << std::endl;
	std::cout << std::endl;

	std::cout << "Creating int pointers to the beginning of buffer with custom "
				 "allocator and base buffer"
			  << std::endl;
	int* base_int = (int*)base.offset(0);
	int* buf_int = (int*)buf.offset(0);
	*buf_int = rand();
	std::cout << "\tSetting custom allocator int to " << *buf_int << std::endl;
	std::cout << std::endl;
	std::cout << "\tOverlapping buffer int: " << *base_int << std::endl;
	std::cout << std::endl;
}

void buffer_test() {
	char testfilename[] = "_buffer_test_file_";
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
	*my_int = rand();
	std::cout << "\t*my_int: " << *my_int << std::endl;
	std::cout << std::endl;

	std::cout << "Allocating int my_second_int in buffer" << std::endl;
	int* my_second_int = (int*)buf.alloc(sizeof(int));
	std::cout << "\tSuccess" << std::endl;
	*my_second_int = rand();
	std::cout << "\t*my_second_int: " << *my_second_int << std::endl;
	std::cout << std::endl;

	std::cout << "Writing buffer to file 'test'" << std::endl;
	std::ofstream otestfile(testfilename, std::ofstream::binary);
	buf.save(otestfile);
	otestfile.close();
	std::cout << "\tSuccess" << std::endl;
	std::cout << std::endl;

	std::cout << "Creating new_buffer from file 'test'" << std::endl;
	std::ifstream itestfile(testfilename, std::ifstream::binary);
	buffer new_buffer(itestfile);
	itestfile.close();
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
		*my_long = 1;
		std::cout << "\tAllocation error not caught" << std::endl;
	} catch (AllocationError ex) {
		std::cout << "\tCaught allocation error" << std::endl;
	}
	std::remove(testfilename);
	std::cout << std::endl;
}

void rptr_buffer_test() {
	char rptrfilename[] = "_rptr_buffer_test_rptrs_";
	char longfilename[] = "_rptr_buffer_test_longs_";

	buffer longbuffer(sizeof(long) * 4);
	buffer rptrbuffer(sizeof(int) * 4);

	std::cout << std::endl;

	long* t;
	std::cout << "Original values" << std::endl;
	for (int i = 1; i <= 4; i++) {
		t = (long*)longbuffer.alloc<long>();
		*t = rand();
	}
	std::cout << std::endl;

	rptr<long, int>* testrptr;
	for (int i = 0; i < 4; i++) {
		testrptr = (rptr<long, int>*)rptrbuffer.alloc<int>();
		testrptr->change((long*)longbuffer.offset(sizeof(long) * i),
						 longbuffer.offset(0));
	}

	rptr<long, int>* looprptr;
	std::cout << "No realloc through memory" << std::endl;
	for (int i = 0; i < 4; i++) {
		looprptr = (rptr<long, int>*)rptrbuffer.offset(sizeof(int) * i);
		std::cout << i + 1 << ": " << (*looprptr) * longbuffer.offset(0)
				  << std::endl;
	}
	std::cout << std::endl;

	try {
		std::ofstream olongbuffer(longfilename, std::ofstream::binary);
		longbuffer.save(olongbuffer);
		olongbuffer.close();
		std::cout << "Saved longs to file successfully" << std::endl;

		std::ofstream orptrbuffer(rptrfilename, std::ifstream::binary);
		rptrbuffer.save(orptrbuffer);
		orptrbuffer.close();
		std::cout << "Saved rptrs to file successfully" << std::endl;

		std::cout << std::endl;
	} catch (std::exception ex) {
		std::cout << "Error saving files!" << std::endl
				  << ex.what() << std::endl;
	}

	buffer longbuffer_file;
	buffer rptrbuffer_file;
	try {
		std::ifstream ilongfile(longfilename, std::ifstream::binary);
		longbuffer_file.load(ilongfile);
		ilongfile.close();
		std::cout << "Loaded longs from file successfully" << std::endl;

		std::ifstream irptrfile(rptrfilename, std::ifstream::binary);
		rptrbuffer_file.load(irptrfile);
		irptrfile.close();
		std::cout << "Loaded rptrs from file successfully" << std::endl;
	} catch (std::exception ex) {
		std::cout << "Error loading files!" << std::endl
				  << ex.what() << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Both from files" << std::endl;
	rptr<long, int>* ptrrptr;
	for (int i = 0; i < 4; i++) {
		ptrrptr = (rptr<long, int>*)rptrbuffer_file.offset(sizeof(int) * i);
		std::cout << i + 1 << ": " << (*ptrrptr) * longbuffer_file.offset(0)
				  << std::endl;
	}
	std::remove(rptrfilename);
	std::remove(longfilename);
	std::cout << std::endl;
}

typedef void (*test_function)(void);
std::vector<std::pair<std::string, test_function>> test_functions;

void test_all() {
	std::string current;
	try {
		for (auto psf : test_functions) {
			if (psf.first != "run all") {
				current = psf.first;
				psf.second();
			}
		}
		std::cout << "\e[1;1H\e[2J" << std::endl;
		std::cout << "Completed all tests successfully!" << std::endl;
	} catch (std::exception ex) {
		std::cout << "\e[1;1H\e[2J" << std::endl;
		std::cout << "Error in function " << current << "!" << std::endl;
		std::cout << "\t" << ex.what() << std::endl;

		std::cout << std::endl;
	}
}

int main() {
	test_functions.push_back(std::make_pair("run all", test_all));
	test_functions.push_back(std::make_pair("rptr", rptr_test));
	test_functions.push_back(std::make_pair("buffer", buffer_test));
	test_functions.push_back(
		std::make_pair("buffer custom allocators", buffer_custom_allocators));
	test_functions.push_back(std::make_pair("rptr+buffer", rptr_buffer_test));
	test_functions.push_back(
		std::make_pair("buffer complex", buffer_complex_test));

	while (true) {
		int execute = 0;
		std::cout << std::endl << "Commands: " << std::endl;
		std::cout << std::endl;
		std::cout << 0 << ": exit" << std::endl;
		for (int i = 0; i < test_functions.size(); i++) {
			std::cout << i + 1 << ": " << test_functions[i].first;
			std::cout << std::endl;
		}
		std::cout << std::endl << ": ";
		std::cin >> execute;
		if (--execute > -1 && execute < test_functions.size()) {
			std::cout << "\e[1;1H\e[2J" << std::endl;
			test_functions[execute].second();
		} else
			break;
	}

	return 0;
}
