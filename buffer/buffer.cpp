#include <fstream>
#include <iostream>

#include "buffer.hpp"

Snippets::buffer::buffer(Allocator allocator, void* allocateObject) {
    this->allocator = &allocator;
    buf = (char*)this->allocator->allocate(0, allocateObject);
    current_size = 0;
    max_size = 0;
}
Snippets::buffer::buffer(size_t size, Allocator allocator, void* allocateObject) {
    this->allocator = &allocator;
    buf = (char*)((*(this->allocator)).allocate(size, allocateObject));
    current_size = 0;
    max_size = size;
}
Snippets::buffer::buffer(char* filename, Allocator allocator, void* allocateObject) {
    std::ifstream in(filename,std::ifstream::ate | std::ifstream::binary);
    long size = in.tellg();
    this->allocator = &allocator;
    buf = (char*)this->allocator->allocate(size, allocateObject);
    in.seekg(0, std::ifstream::beg);
    in.read(buf,size);
    max_size = size;
}

void* Snippets::buffer::alloc(size_t size) {
    if (size + current_size > max_size) {
        throw AllocationError();
    }
    current_size += size;
    return buf + (current_size-size);
}

void* Snippets::buffer::offset(size_t offset) {
    return &buf[offset];
}

void Snippets::buffer::clear() {
    for (unsigned int i = 0; i < max_size; i++) {
        buf[i] = 0; 
    }
    current_size = 0;
}

void Snippets::buffer::save(char* filename) {
    std::ofstream out(filename, std::ostream::binary);
    out.write(buf, max_size);
    out.close();
}
int Snippets::buffer::load(char* filename, size_t offset) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    unsigned long size = in.tellg();
    in.seekg(0, std::ifstream::beg);

    if (max_size-offset < size) {
        buf = (char*)this->allocator->reallocate(buf, size+offset);
        current_size = 0;
    }

    in.read(buf + offset,size);
    in.close();
    current_size = size+offset;
    if (size < max_size)
        return 1;
    return 0;
}

void* Snippets::buffer::malloc::allocate(size_t size, void* worker) {
    return std::malloc(size);
}
void* Snippets::buffer::malloc::reallocate(void* current, size_t size, void* worker) {
    return std::realloc(current, size);
}
void Snippets::buffer::malloc::free(void* current, void* worker) {
    std::free(current);
}

void* Snippets::buffer::recursive::allocate(size_t size, void *worker) {
    if (worker == nullptr)
        throw;
    ((Snippets::buffer*)worker)->alloc(size);
}
void* Snippets::buffer::recursive::reallocate(void *current, size_t size, void* worker) {
    throw; // No current way to do this
}
void Snippets::buffer::recursive::free(void *current, void* worker) {
    throw; // No current way to do this
}
Snippets::buffer::malloc Snippets::buffer::default_allocator = Snippets::buffer::malloc();
Snippets::buffer::recursive Snippets::buffer::recursive_allocator = Snippets::buffer::recursive();
