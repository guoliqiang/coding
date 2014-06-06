CC = gcc
CXX = g++

CXXFLAGS = -g
CFLAGS = -g

INCPATH = -I. \
          -I./include
DEP_INCPATCH =

LIBFLAGS = -L../../.snake/dbg/base/internal/ -lbase \
           -lpthread \
					 -lm \
					 -lrt

BIN_DIR = ./bin
SOURCE_DIR = ./internal

foo_sources = foo.cc

all:
	$(CXX) -o $(BIN_DIR)/foo $(SOURCE_DIR)/$(foo_sources) $(CXXFLAGS) $(INCPATH) $(LIBFLAGS)

clean:
	rm -rf $(BIN_DIR)
	mkdir $(BIN_DIR)
