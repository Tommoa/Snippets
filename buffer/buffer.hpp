#ifndef __SNIPPETS_BUFFER_INC_HPP__
#define __SNIPPETS_BUFFER_INC_HPP__
#include <exception>

namespace Snippets {

    typedef unsigned long size_t;

    class AllocationError : std::exception {};

    class buffer {
        char* buf;

      protected: 
        size_t maxsize;
        size_t current_size;

      public:
        buffer() ;
        buffer(size_t size) ;
        buffer(char* filename) ;

        void* alloc(size_t size) ;
        template<typename T> void* alloc() {
            if (sizeof(T) + current_size > maxsize) {
                throw AllocationError();
            }
            current_size += sizeof(T);
            return buf + (current_size-(sizeof(T)));
        }

        void* offset(size_t offset) ;

        void clear() ;

        void save(char* filename) ;
        int load(char* filename, size_t offset = 0) ;
    };
}
#endif
