HEAD := $(wildcard *.h)
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))

# implicit switches for gcc
CFLAGS = -O3 -march=native -pipe -Wall -Wextra
CXXFLAGS = $(CFLAGS)

# compiler
#CXX = g++-4.6.3
CC = $(CXX)

PROGRAMS = dgemm dgesv

# clean does not have a file on its own
.PHONY: clean

all: $(PROGRAMS)

# also a default target
dgemm: dgemm.o second.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -lcblas -o $@

dgesv: dgesv.o second.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -llapack -o $@

# cleans files form compilation
clean:
	rm -f $(OBJ) $(PROGRAMS)

# every object file depends on all header files
$(OBJ): $(HEAD)

# rest is done automagically by implicit rules
