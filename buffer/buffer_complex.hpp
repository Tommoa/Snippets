#ifndef __SNIPPETS_BUFFER_COMPLEX_INC_HPP__
#define __SNIPPETS_BUFFER_COMPLEX_INC_HPP__

#include "../abstractions/allocators.hpp"
#include "../abstractions/global.hpp"
#include <istream>
#include <map>
#include <ostream>

namespace Snippets {
	class buffer_complex {
		char* buf;

		// NOTE:
		// 	I have attempted to write an allocator with new/delete to allow
		// 	std::allocator to be used. I haven't been able to figure out
		// 	how to do this yet though.

		/* *
		 * Allocate using std::malloc/std::realloc/std::free
		 * */
		struct malloc : public Allocator {
			void* allocate(size_t size, void* v = nullptr) override;
			void* reallocate(void* current, size_t size,
							 void* v = nullptr) override;
			void free(void* current, void* v = nullptr) override;
		};

		/* *
		 * Allocate using another buffer (defined by worker)
		 * */
		struct recursive : public Allocator {
			void* allocate(size_t size, void* worker = nullptr) override;
			void* reallocate(void* current, size_t size,
							 void* worker = nullptr) override;
			void free(void* current, void* worker = nullptr) override;

		  private:
			buffer_complex* worker;
		};

	  protected:
		Allocator* allocator;
		size_t max_size;
		std::map<size_t, size_t> memory_locations;

	  public:
		// An allocator that uses malloc to allocate space for the buffer
		static malloc default_allocator;
		// An allocator that uses another buffer to allocate space for this
		// buffer
		static recursive recursive_allocator;

		// Default constructor.
		// 	Optional:
		// 		Allocator *allocator: A pointer to a defined derivative of the
		// 								Allocator class
		// 		void *AllocateObject: An object that the allocator may use for
		// 								allocation
		buffer_complex(Allocator* allocator = &default_allocator,
					   void* AllocateObject = nullptr);

		// 	Required:
		// 		size_t initial_size: The initial size (in bytes) of the buffer
		// 	Optional:
		// 		Allocator *allocator: A pointer to a defined derivative of the
		// 								Allocator class
		// 		void *AllocateObject: An object that the allocator may use for
		// 								allocation
		buffer_complex(size_t initial_size,
					   Allocator* allocator = &default_allocator,
					   void* AllocateObject = nullptr);

		// Destructor for the buffer_complex object
		~buffer_complex();

		// Allocates space on the buffer.
		// 	Required:
		// 		size_t size: The size to be allocated
		// 	Returns:
		// 		A void* pointer to the location of the allocation
		void* allocate(size_t size);

		// Reallocates an object on the buffer
		// 	Required:
		// 		void* current: A pointer to the current object
		// 		size_t size: The new size of the object
		// 	Returns:
		// 		A void* pointer to the location of the allocation
		void* reallocate(void* current, size_t size);

		// Frees an object from use on the buffer and allows that space
		// 		to be reused
		// 	Required:
		// 		void* current: A pointer to the object to be freed
		void free(void* current);

		// Adds data to a buffer from a stream.
		// 	Required:
		// 		std::istream& in: The stream to be used as input
		// 		size_t offset: The offset of the inputted data from the start
		// 						of the buffer
		void load(std::istream& in, size_t offset = 0);

		// Saves the current buffer to a stream.
		// 	Required:
		// 		std::ostream& out: The stream to be used as output
		void save(std::ostream& out);

		// Returns a pointer at a specific offset. The offset must be the
		// 		start of an object
		// 	Required:
		// 		size_t offset: The offset in bytes to be returned
		void* offset(size_t offset);

		// Clears the buffer
		void clear();

		// Resizes the buffer to a new size.
		// 	Required:
		// 		size_t new_size: The new size of the buffer in bytes
		void resize(size_t new_size);
	};
}

#endif
