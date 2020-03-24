CC = gcc
CXX = g++
CFLAGS = -g -Wall

all: hw1

%.o: %.cpp
	$(CXX) -c $< $(CFLAGS)

hw1: hw1.o
	$(CXX) -o $@ $^

clean:
	rm -rf hw1.o hw1

.PHONY: all clean