program = comparison

HEAD := $(wildcard *.h)
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))

# implicit switches for gcc
CFLAGS = -O2 -pipe -Wall -Wextra

# switches for linker
LDFLAGS = -lcblas

# compiler
CC = gcc
#CC = g++

# clean does not have a file on its own
.PHONY: clean

# also a default target
${program}: ${OBJ}
	$(CC) $(LDFLAGS) dgemm-blas.o second.o -o dgemm-blas

# cleans files form compilation
clean:
	rm -f ${OBJ} dgemm-blas

# every object file depends on all header files
${OBJ}: ${HEAD}

# rest is done automagically by implicit rules
