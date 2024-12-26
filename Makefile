# Compiler
CC = g++

# Compilation flags
CFLAGS = --std=c++17 -Wall -Werror -pedantic -g

# Linker libraries (SFML and Boost)
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

# Header files
DEPS = Sokoban.hpp

# Object files
OBJECTS = Sokoban.o main.o

# Program name
PROGRAM = Sokoban

# Static library name
STATIC_LIB = Sokoban.a

.PHONY: all clean lint

# Build all`
all: $(STATIC_LIB) $(PROGRAM) test 

# Rule to generate .o files from corresponding .cpp files
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

# Rule to create the static library
$(STATIC_LIB): Sokoban.o
	ar rcs $@ $^

# Rule to create the final program executable
$(PROGRAM): $(OBJECTS) $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIB)

# Rule to create the test executable

test: test.o $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ test.o $(STATIC_LIB) $(LIB)

# Clean rule to remove all build artifacts
clean:
	rm -f *.o $(PROGRAM) $(STATIC_LIB) test

# Lint rule (adjust according to your linter)
lint:
	cpplint *.cpp *.hpp
