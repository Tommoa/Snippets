#include "../buffer/buffer.hpp"
#include "../rptr/rptr.hpp"

#include <iostream>

using namespace Snippets;

int main() {

    char* rptrfilename = "rptr_buffer_test_rptrs";
    char* longfilename = "rptr_buffer_test_longs";
    
    buffer longbuffer(sizeof(long)*4);
    buffer rptrbuffer(sizeof(int)*4);

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
        testrptr->change((long*)longbuffer.offset(sizeof(long)*i), longbuffer.offset(0));
    }

    rptr<long, int>* looprptr;
    std::cout << "No realloc through memory" << std::endl;
    for (int i = 0; i < 4; i++) {
        looprptr = (rptr<long, int>*)rptrbuffer.offset(sizeof(int)*i);
        std::cout << i+1 << ": " << (*looprptr) * longbuffer.offset(0) << std::endl;
    } 
    std::cout << std::endl;

    try {
        longbuffer.save(longfilename);
        std::cout << "Saved longs to file successfully" << std::endl;
        
        rptrbuffer.save(rptrfilename);
        std::cout << "Saved rptrs to file successfully" << std::endl;

        std::cout << std::endl;
    }
    catch (std::exception ex) {
        std::cout << "Error saving files!" << std::endl << ex.what() << std::endl;
    }

    buffer longbuffer_file;
    buffer rptrbuffer_file;
    try {
        longbuffer_file.load(longfilename);
        std::cout << "Loaded longs from file successfully" << std::endl;

        rptrbuffer_file.load(rptrfilename);
        std::cout << "Loaded rptrs from file successfully" << std::endl;
    }
    catch (std::exception ex) {
        std::cout << "Error loading files!" << std::endl << ex.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Both from files" << std::endl;
    rptr<long, int>* ptrrptr;
    for (int i = 0; i < 4; i++) {
        ptrrptr = (rptr<long, int>*)rptrbuffer_file.offset(sizeof(int)*i);
        std::cout << i+1 << ": " << (*ptrrptr) * longbuffer_file.offset(0) << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
