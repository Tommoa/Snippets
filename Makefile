COMPILER = g++
OBJS = all_tests.o buffer.o buffer_complex.o
BUILD_HEADERS = rptr/rptr.hpp
OUTPUT_NAME = test
COMPILER_FLAGS = -fpermissive -Wall -std=c++14 -c

build: $(OBJS) $(BUILD_HEADERS)
	$(COMPILER) $(OBJS) -o $(OUTPUT_NAME)

debug: $(OBJS) $(BUILD_HEADERS)
	$(COMPILER) $(OBJS) -o $(OUTPUT_NAME) -g

clean: build
	rm $(OBJS)

clear:
	\rm *.o *~ $(OUTPUT_NAME)

run: build
	./$(OUTPUT_NAME)
	rm $(OUTPUT_NAME)

buffer/buffer.hpp: abstractions/allocators.hpp abstractions/global.hpp

buffer.o: buffer/buffer.cpp buffer/buffer.hpp 
	$(COMPILER) $(COMPILER_FLAGS) buffer/buffer.cpp

buffer_complex.o: buffer/buffer_complex.cpp buffer/buffer_complex.hpp
	$(COMPILER) $(COMPILER_FLAGS) buffer/buffer_complex.cpp

all_tests.o: tests/all_tests.cpp buffer/buffer.hpp buffer/buffer_complex.hpp
	$(COMPILER) $(COMPILER_FLAGS) tests/all_tests.cpp
