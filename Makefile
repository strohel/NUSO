HEAD := $(wildcard *.h)
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))

# implicit switches for gcc
CFLAGS = -O2 -march=native -pipe -Wall -Wextra
CXXFLAGS = $(CFLAGS)

# switches for linker
LDLIBS = -lcblas

# compiler
#CXX = g++-4.6.3
CC = $(CXX)

PROGRAMS = dgemm

# clean does not have a file on its own
.PHONY: clean

all: $(PROGRAMS)

# also a default target
dgemm: dgemm.o second.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

# cleans files form compilation
clean:
	rm -f $(OBJ) $(PROGRAMS)

# every object file depends on all header files
$(OBJ): $(HEAD)

# rest is done automagically by implicit rules
