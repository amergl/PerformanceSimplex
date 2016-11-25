CC=g++
CXXFLAGS=-std=c++11 -Wall
LFLAGS=-lcppunit

all: clean test_simplex test

test_simplex: test_simplex.cxx simplex.hxx
	$(CC) $@.cxx $(LFLAGS) $(CXXFLAGS) -o $@

test: test_simplex
	@./test_simplex

clean:
	@rm -f main test_simplex *~
