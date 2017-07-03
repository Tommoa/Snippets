#include "../rptr/rptr.hpp"
#include <cstdlib>
#include <iostream>

using namespace Snippets;

typedef struct pen {
	int t;
	rptr<pen, long> ptr;
} pen;

int main() {
	std::cout << std::endl;

	typedef struct {
		int t;
		rptr<int, int> ptr;
	} packed;

	typedef struct {
		int t;
		int *ptr;
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
	t.ptr = rptr<int, int>(&b, (void *)&t);
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

	p1.ptr = rptr<pen, long>(&p2, (void *)&p1);
	p2.ptr = rptr<pen, long>(&p1, (void *)&p2);
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
	intptr = rptr<int, int>((int *)&buf, (void *)&buf);
	intptr.assign(rand(), &buf);
	std::cout << "\tAssigned " << intptr * &buf << " to the char array"
			  << std::endl;
	std::cout << std::endl;
	std::cout << "Char buffer contents:" << std::endl;
	for (int i = 0; i < 8; i++) {
		std::cout << "\t" << i << ": " << buf[i] << std::endl;
	}
	std::cout << std::endl;
	rptr<int, int> nptr((int *)&buf, (void *)&buf);
	std::cout << "Pulled " << nptr * &buf << " out of the char buffer"
			  << std::endl;

	std::cout << std::endl;

	return 0;
}
