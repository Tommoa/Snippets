#include <fstream>

#include "buffer.hpp"

Snippets::buffer::buffer() {
    buf = (char*)malloc(0);
    maxsize = 0;
    current_size = 0;
}
Snippets::buffer::buffer(size_t size) {
    buf = (char*)malloc(size);
    maxsize = size;
    current_size = 0;
}
Snippets::buffer::buffer(char* filename) {
    std::ifstream in(filename,std::ifstream::ate | std::ifstream::binary);
    long size = in.tellg();
    buf = (char*)malloc(size);
    in.seekg(0, std::ifstream::beg);
    in.read(buf,size);
}

void* Snippets::buffer::alloc(size_t size) {
    if (size + current_size > maxsize) {
        throw AllocationError();
    }
    current_size += size;
    return buf + (current_size-size);
}

void* Snippets::buffer::offset(size_t offset) {
    return &buf[offset];
}

void Snippets::buffer::clear() {
    for (unsigned int i = 0; i < maxsize; i++) {
        buf[i] = 0;
    }
    current_size = 0;
}

void Snippets::buffer::save(char* filename) {
    std::ofstream out(filename, std::ostream::binary);
    out.write(buf, maxsize);
    out.close();
}
int Snippets::buffer::load(char* filename, size_t offset) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    unsigned long size = in.tellg();
    in.seekg(0, std::ifstream::beg);

    if (maxsize-offset < size) {
        buf = (char*)realloc(buf, size+offset);
        maxsize = size;
        current_size = 0;
    }

    in.read(buf + offset,size);
    in.close();
    current_size = size+offset;
    if (size < maxsize)
        return 1;
    return 0;
}
