override CXXFLAGS += -Wall -g -O0 -pipe -std=c++11
INC               += -I../ -I. -Igtest-1.7.0/include
CC                := cc
CXX               := c++

VALGRIND          := valgrind

BINARY := test.out
DEPS := $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp -type f))

all: $(BINARY)

%.o: %.cpp libgtest.a
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -c $< -o $@

$(BINARY): $(DEPS) $(MODS) main.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -o $(BINARY) $(DEPS) libgtest.a -pthread ../src/libreactcpp.so -lev

libgtest.a:
	wget -q http://googletest.googlecode.com/files/gtest-1.7.0.zip
	unzip -qq gtest-1.7.0.zip
	$(CXX) $(CXXFLAGS) -I gtest-1.7.0/include -I gtest-1.7.0 -c gtest-1.7.0/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

.PHONY: test

test: $(BINARY)
	./$(BINARY)

.PHONY: valgrind

valgrind: $(BINARY)
	$(VALGRIND) $(VALGRIND_OPTS) ./$(BINARY)

.PHONY: clean

clean:
	rm -rf $(BINARY) $(DEPS) $(MODS)