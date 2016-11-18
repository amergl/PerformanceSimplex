CC=g++
CXXFLAGS=-std=c++11

main: main.cxx
	$(CC) $(CXXFLAGS) $? -o $@

clean:
	rm main
