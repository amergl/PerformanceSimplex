CC=g++
CXXFLAGS=-std=c++11 -Wall
LFLAGS=-lcppunit

all: main test_simplex test

main: main.cxx
	$(CC) $? $(CXXFLAGS) -o $@

test_simplex: test_simplex.cxx
	$(CC) $? $(LFLAGS) $(CXXFLAGS) -o $@

test: test_simplex
	@./test_simplex

clean:
	@rm -f main test_simplex *~
