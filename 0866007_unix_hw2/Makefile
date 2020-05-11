CC = gcc
CXX = g++
CFLAG = -g -Wall

all: sandbox sandbox.so sample1 sample2

%.o: %.cpp
	$(CXX) -c $< $(CFLAG) -std=c++11

sandboxso.o: sandboxso.c
	$(CC) -c $< $(CFLAG) -fPIC #-D DEBUG

sandbox: sandbox.o
	$(CXX) -o $@ $^

sandbox.so: sandboxso.o
	$(CC) -o $@ $^ -shared -ldl

test: test.o
	$(CXX) -o $@ $^

sample1: sample.o
	$(CXX) -o $@ $^

sample2: sample.o
	$(CXX) -o $@ $^ -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

clean:
	rm -rf sandbox sandbox.o sandbox.so sandboxso.o sample.o sample1 sample2

.PHONY: all clean
