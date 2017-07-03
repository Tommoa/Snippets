#ifndef __SNIPPETS_ALLOCATORS_DEF_HPP__
#define __SNIPPETS_ALLOCATORS_DEF_HPP__

#include "global.hpp"
#include <exception>

namespace Snippets {
	struct Allocator {
		virtual void *allocate(size_t, void *v = nullptr) = 0;
		virtual void *reallocate(void *, size_t, void *v = nullptr) = 0;
		virtual void free(void *, void *v = nullptr) = 0;
		~Allocator() {}
	};
	class AllocationError : std::exception {};
}
#endif
