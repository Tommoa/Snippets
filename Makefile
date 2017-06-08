COMPILER = clang++
OBJS = all_tests.o buffer.o
BUILD_HEADERS = rptr/rptr.hpp
OUTPUT_NAME = test

build: $(OBJS) $(BUILD_HEADERS)
	$(COMPILER) buffer.o all_tests.o -o $(OUTPUT_NAME)

clean: build
	rm $(OBJS)

clear:
	\rm *.o *~ $(OUTPUT_NAME)

run: build
	./$(OUTPUT_NAME)
	rm $(OUTPUT_NAME)

buffer/buffer.hpp: abstractions/allocators.hpp abstractions/global.hpp

buffer.o: buffer/buffer.cpp buffer/buffer.hpp 
	$(COMPILER) -Wall -std=c++14 -c buffer/buffer.cpp

all_tests.o: tests/all_tests.cpp buffer/buffer.hpp
	$(COMPILER) -Wall -std=c++14 -c tests/all_tests.cpp
