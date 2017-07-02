#ifndef __SNIPPETS_BUFFER_COMPLEX_INC_HPP__
#define __SNIPPETS_BUFFER_COMPLEX_INC_HPP__

#include <map>
#include <istream>
#include <ostream>
#include "../abstractions/global.hpp"
#include "../abstractions/allocators.hpp"

namespace Snippets {
	class buffer_complex {
		char* buf; 
		struct malloc : public Allocator {
            void* allocate(size_t size, void* v = nullptr) override;
            void* reallocate(void* current, size_t size, void* v = nullptr) override;
            void free(void* current, void* v = nullptr) override; 
        }; 
		struct recursive : public Allocator {
            void* allocate(size_t size, void* v = nullptr) override; 
            void* reallocate(void* current, size_t size, void* v = nullptr) override;
            void free(void* current, void* v = nullptr) override;
			private:
			buffer_complex* worker;
        }; 

		protected:
		Allocator* allocator; 
		size_t max_size;
		std::map<size_t, size_t> memory_locations; 

		public: 
		static malloc default_allocator;
		static recursive recursive_allocator;

		buffer_complex(Allocator *allocator = &default_allocator, void* AllocateObject = nullptr);
		buffer_complex(size_t initial_size, Allocator *allocator = &default_allocator, void* AllocateObject = nullptr);

		void* allocate(size_t size);
		void* reallocate(void* current, size_t size);
		void free(void* current);

		void load(std::istream& in, size_t offset = 0);
		void save(std::ostream& out);

		void* offset(size_t offset);
		void clear(); 

		void resize(size_t new_size);
	};
}

#endif
