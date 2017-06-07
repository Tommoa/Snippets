#include "buffer.hpp"

#include <iostream>

using namespace Snippets;

int main() {
    buffer buffer(sizeof(int)*2);

    int* my_int = (int*)buffer.alloc<int>();
    *my_int = 4;

    int* my_second_int = (int*)buffer.alloc(sizeof(int));
    *my_second_int = 3;

    buffer.save("test");

    class buffer new_buffer("test");

    my_int = (int*)new_buffer.offset(0);
    my_second_int = (int*)new_buffer.offset(4);
    std::cout << *my_int << " " << *my_second_int << std::endl;

    try {
        long* my_long = (long*)buffer.alloc<long>();
    }
    catch (AllocationError ex) {
        std::cout << "caught allocation error" << std::endl;
    }

    return 0;
}
