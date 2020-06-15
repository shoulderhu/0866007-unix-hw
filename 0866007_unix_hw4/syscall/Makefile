CXX = g++
CFLAGS = -std=c++11 -g -Wall

all: psyscall

%.o: %.cpp
	$(CXX) -c $< $(CFLAGS)

psyscall: psyscall.o
	$(CXX) -o $@ $^

clean:
	rm -rf psyscall.o psyscall

.PHONY:	all clean