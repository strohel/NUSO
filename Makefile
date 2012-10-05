program = comparison

HEAD := $(wildcard *.h)
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))

# implicit switches for gcc
CFLAGS = -O2 -march=native -pipe -Wall -Wextra

# switches for linker
LDFLAGS = -lcblas

# compiler
#CC = gcc
CC = g++

# clean does not have a file on its own
.PHONY: clean

# also a default target
$(program): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# cleans files form compilation
clean:
	rm -f ${OBJ} ${program}

# every object file depends on all header files
${OBJ}: ${HEAD}

# rest is done automagically by implicit rules
