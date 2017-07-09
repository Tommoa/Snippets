#include "buffer.hpp"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

Snippets::buffer::buffer(Allocator* allocator, void* allocateObject) {
	this->allocator = allocator;
	buf = static_cast<char*>(this->allocator->allocate(0, allocateObject));
	current_size = 0;
	max_size = 0;
}
Snippets::buffer::buffer(size_t size, Allocator* allocator,
						 void* allocateObject) {
	this->allocator = allocator;
	buf = static_cast<char*>(this->allocator->allocate(size, allocateObject));
	current_size = 0;
	max_size = size;
}
Snippets::buffer::buffer(std::istream& in, Allocator* allocator,
						 void* allocateObject) {
	long size = in.tellg();
	in.seekg(0, std::istream::end);
	size = in.tellg() - size;
	this->allocator = allocator;
	buf = static_cast<char*>(this->allocator->allocate(size, allocateObject));
	in.seekg(0, std::istream::beg);
	in.read(buf, size);
	max_size = size;
}
Snippets::buffer::~buffer() {
	this->allocator->free(buf); // Something to note is that this will not
								// change anything for recursive_allocator
								// as buffer::free() does nothing
}

void* Snippets::buffer::alloc(size_t size) {
	if (size + current_size > max_size) {
		throw AllocationError();
	}
	current_size += size;
	return buf + (current_size - size);
}

void* Snippets::buffer::offset(size_t offset) { return &buf[offset]; }

void Snippets::buffer::clear() {
	for (unsigned int i = 0; i < max_size; i++) {
		buf[i] = 0;
	}
	current_size = 0;
}

void Snippets::buffer::save(std::ostream& out) { out.write(buf, max_size); }
int Snippets::buffer::load(std::istream& in, size_t offset) {
	std::ios::streampos start = in.tellg();
	in.seekg(0, std::ifstream::end);
	std::size_t size = in.tellg() - start;
	in.seekg(0, std::ifstream::beg);

	if (max_size - offset < size) {
		buf =
			static_cast<char*>(this->allocator->reallocate(buf, size + offset));
		current_size = 0;
	}

	in.read(buf + offset, size);
	current_size = size + offset;
	if (size < max_size)
		return 1;
	return 0;
}

void Snippets::buffer::resize(size_t new_size, bool allow_try) {
	if (allow_try) {
		try {
			buf =
				static_cast<char*>(this->allocator->reallocate(buf, new_size));
		} catch (...) { // We want to make sure that we catch all possible
						// reallocator errors
			char* temp = buf;
			buf = static_cast<char*>(this->allocator->allocate(new_size));
			std::memcpy(buf, temp, max_size);
			this->allocator->free(buf);
		}
	} else {
		buf = static_cast<char*>(this->allocator->reallocate(buf, new_size));
	}
	max_size = new_size;
}

void* Snippets::buffer::malloc::allocate(size_t size, void* worker) {
	return std::malloc(size);
}
void* Snippets::buffer::malloc::reallocate(void* current, size_t size,
										   void* worker) {
	return std::realloc(current, size);
}
void Snippets::buffer::malloc::free(void* current, void* worker) {
	std::free(current);
}

void* Snippets::buffer::recursive::allocate(size_t size, void* worker) {
	if (worker == nullptr)
		throw;
	return static_cast<Snippets::buffer*>(worker)->alloc(size);
}
void* Snippets::buffer::recursive::reallocate(void* current, size_t size,
											  void* worker) {
	throw; // No current way to do this
}
void Snippets::buffer::recursive::free(void* current, void* worker) {
	throw; // No current way to do this
}
Snippets::buffer::malloc Snippets::buffer::default_allocator =
	Snippets::buffer::malloc();
Snippets::buffer::recursive Snippets::buffer::recursive_allocator =
	Snippets::buffer::recursive();
