CC=g++
CXXFLAGS=-std=c++11 -Wall

main: main.cxx
	$(CC) $(CXXFLAGS) $? -o $@

clean:
	rm main
