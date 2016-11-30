CC=g++
CXXFLAGS=-std=c++11 -Wall -ggdb
LFLAGS=-lcppunit

all: clean test_simplex test

test_simplex: test_simplex.cxx simplex.hxx
	$(CC) $@.cxx $(LFLAGS) $(CXXFLAGS) -o $@

test: test_simplex
	@./test_simplex

clean:
	@rm -f test_simplex *~
