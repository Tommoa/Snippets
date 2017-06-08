#ifndef __SNIPPETS_BUFFER_INC_HPP__
#define __SNIPPETS_BUFFER_INC_HPP__
#include <exception>

namespace Snippets {

    typedef unsigned long size_t;

    struct Allocator {
        virtual void* allocate(size_t, void* v = nullptr) = 0;
        virtual void* reallocate(void*, size_t, void* v = nullptr) =0;
        virtual void free(void*, void* v = nullptr) = 0;
        ~Allocator() {
            
        }
    };

    class AllocationError : std::exception {};

    class buffer {
        char* buf;
        struct malloc : public Allocator {
            void* allocate(size_t size, void* v = nullptr); 
            void* reallocate(void* current, size_t size, void* v = nullptr); 
            void free(void* current, void* v = nullptr); 
        };
        struct recursive : public Allocator {
            void* allocate(size_t size, void* v);
            void* reallocate(void* current, size_t size, void* v);
            void free(void* current, void* v);
          private:
            void* worker;
        };
      protected: 
        Allocator* allocator;
        size_t current_size;
        size_t max_size;

      public:
        static malloc default_allocator;
        static recursive recursive_allocator;
        buffer(Allocator *allocator = &default_allocator, void* allocateObject = nullptr) ;
        buffer(size_t size, Allocator *allocator = &default_allocator, void* allocateObject = nullptr) ;
        buffer(char* filename, Allocator *allocator = &default_allocator, void* allocateObject = nullptr) ;

        void* alloc(size_t size) ;
        template<typename T> void* alloc() {
            if (sizeof(T) + current_size > max_size) {
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
