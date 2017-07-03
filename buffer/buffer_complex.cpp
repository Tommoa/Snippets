#include "buffer_complex.hpp"
#include <cstring>
#include <iostream>

Snippets::buffer_complex::buffer_complex(Allocator* allocator,
										 void* AllocateObject) {
	max_size = 0;
	this->allocator = allocator;
	buf = static_cast<char*>(this->allocator->allocate(0, AllocateObject));
}

Snippets::buffer_complex::buffer_complex(size_t initial_size,
										 Allocator* allocator,
										 void* AllocateObject) {
	max_size = initial_size;
	this->allocator = allocator;
	buf = static_cast<char*>(
		this->allocator->allocate(initial_size, AllocateObject));
}

Snippets::buffer_complex::~buffer_complex() {
	this->allocator->free(buf); // Ensure we clear the buffer
}

void* Snippets::buffer_complex::allocate(size_t size) {
	if (memory_locations.begin() == memory_locations.end() &&
		max_size >= size) { // Nothing has been placed yet
		memory_locations.emplace(std::make_pair(0, size - 1));
		return buf;
	}
	if (!memory_locations.empty() &&
		max_size - ((--memory_locations.end())->second) >
			size) { // There is space at the end
		auto loc = (--memory_locations.end())->second;
		memory_locations.emplace(std::make_pair(loc + 1, loc + size + 1));
		return buf + loc + 1;
	}

	size_t last = 0;
	for (auto itr = memory_locations.begin(); itr != memory_locations.end();
		 itr++) { // There is space between two other allocations (possibly due
		// to a free)
		if (itr->first - last > size + 1) {
			memory_locations.emplace(std::make_pair(last + 1, last + size + 1));
			return buf + last + 1;
		}
	}

	buf = static_cast<char*>(this->allocator->reallocate(
		buf,
		max_size + size)); // We need to allocate some more space on the end
	auto pos =
		memory_locations.empty() ? 0 : (--memory_locations.end())->second;
	memory_locations.emplace(std::make_pair(pos, pos + size));
	max_size += size;
	return buf + pos;
}

void* Snippets::buffer_complex::reallocate(void* original, size_t size) {
	size_t diff = static_cast<char*>(original) - buf;
	auto mem = memory_locations.find(diff);
	size_t end = (++mem) == memory_locations.end()
					 ? max_size
					 : (mem)->first; // Either the start of the memory location
									 // after or the end of our current memory
									 // location
	size_t start =
		(--(--mem)) == memory_locations.end()
			? 0
			: (mem)->second; // Either the very start of the end of the
							 // previous memory location
	++mem;					 // Return mem to the original iterator

	if (end - start > size) {
		memory_locations.emplace(std::make_pair(start, start + size - 1));
		if (buf + start != original) {
			std::memmove(buf + start, original, mem->second - mem->first);
		}
		memory_locations.erase(mem);
		return buf + start;
	}

	size_t last = 0;
	for (auto itr = memory_locations.begin(); itr != memory_locations.end();
		 itr++) { // There is space between two other allocations (possibly due
		// to a free)
		if (itr == mem)
			continue;
		if (itr->first - last > size + 1) {
			memory_locations.emplace(std::make_pair(last + 1, last + size + 1));
			memory_locations.erase(mem);
			return buf + last + 1;
		}
	}

	buf = static_cast<char*>(this->allocator->reallocate(buf, max_size + size));
	auto pos = (--memory_locations.end())->second;
	memory_locations.emplace(std::make_pair(pos, pos + size));
	memory_locations.erase(mem);
	return buf + pos;
}

void Snippets::buffer_complex::free(void* what) {
	memory_locations.erase(
		memory_locations.find(static_cast<char*>(what) - buf));
}

void Snippets::buffer_complex::save(std::ostream& out) {
	size_t s = memory_locations.size();
	out.write(reinterpret_cast<char*>(&s), sizeof(size_t));
	for (auto itr = memory_locations.begin(); itr != memory_locations.end();
		 itr++) {
		size_t first = itr->first, second = itr->second;
		out.write(reinterpret_cast<char*>(&first), sizeof(size_t));
		out.write(reinterpret_cast<char*>(&second), sizeof(size_t));
	}
	out.write(buf, max_size);
}

void Snippets::buffer_complex::load(std::istream& in, size_t offset) {
	size_t size_of_input;
	in.read(reinterpret_cast<char*>(&size_of_input), sizeof(size_t));
	size_t first, second;
	for (size_t i = 0; i < size_of_input; ++i) {
		in.read(reinterpret_cast<char*>(&first), sizeof(size_t));
		in.read(reinterpret_cast<char*>(&second), sizeof(size_t));
		memory_locations.emplace(
			std::make_pair(offset + first, offset + second));
	}
	size_t size = (--memory_locations.end())->second;
	if (max_size < size) {
		buf = static_cast<char*>(this->allocator->reallocate(buf, size));
		max_size = size;
	}
	in.read(buf + offset, size - offset);
}

void* Snippets::buffer_complex::offset(size_t offset) {
	auto pos = memory_locations.find(offset);
	if (pos == memory_locations.end())
		throw AllocationError();
	return buf + pos->first;
}

void Snippets::buffer_complex::clear() {
	this->allocator->free(buf);
	max_size = 0;
}

void Snippets::buffer_complex::resize(size_t new_size) {
	buf = static_cast<char*>(this->allocator->reallocate(buf, new_size));
}

void* Snippets::buffer_complex::malloc::allocate(size_t size, void* worker) {
	return std::malloc(size);
}

void* Snippets::buffer_complex::malloc::reallocate(void* what, size_t size,
												   void* worker) {
	return std::realloc(what, size);
}

void Snippets::buffer_complex::malloc::free(void* what, void* worker) {
	std::free(what);
}

void* Snippets::buffer_complex::recursive::allocate(size_t size, void* worker) {
	if (worker != nullptr)
		this->worker = static_cast<buffer_complex*>(worker);
	if (this->worker == nullptr)
		throw Snippets::AllocationError();
	return this->worker->allocate(size);
}

void* Snippets::buffer_complex::recursive::reallocate(void* what, size_t size,
													  void* worker) {
	if (worker != nullptr)
		this->worker = static_cast<buffer_complex*>(worker);
	if (this->worker == nullptr)
		throw Snippets::AllocationError();
	return this->worker->reallocate(what, size);
}

void Snippets::buffer_complex::recursive::free(void* what, void* worker) {
	if (worker != nullptr)
		this->worker = static_cast<buffer_complex*>(worker);
	if (this->worker == nullptr)
		throw Snippets::AllocationError();
	this->worker->free(what);
}

Snippets::buffer_complex::malloc Snippets::buffer_complex::default_allocator =
	Snippets::buffer_complex::malloc();

Snippets::buffer_complex::recursive
	Snippets::buffer_complex::recursive_allocator =
		Snippets::buffer_complex::recursive();
