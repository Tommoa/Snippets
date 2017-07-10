#include "../defer/defer.hpp"
#include "../defer/defer_exceptions.hpp"

#include <iostream>

class tester {
  public:
	tester() { std::cout << "tester initialized" << std::endl; }
	~tester() { std::cout << "tester being destructed" << std::endl; }
};

void test_with_obj(bool throwing) {
	auto t = tester();
	defer { std::cout << "Exiting" << std::endl; };
	defer_fail {
		std::cout << "Threw" << std::endl;
		t.~tester();
	};
	if (throwing) {
		std::cout << "Throwing" << std::endl;
		throw std::exception();
	}
	std::cout << "Exiting normally" << std::endl;
}

int main() {
	int* i = new int[100];
	defer {
		std::cout << "Deleting i" << std::endl;
		delete[] i;
	};

	std::cout << "test with no throw" << std::endl;
	test_with_obj(false);

	try {
		std::cout << "test with throw" << std::endl;
		test_with_obj(true);
	} catch (std::exception) {
		std::cout << "Caught" << std::endl;
	}
}
