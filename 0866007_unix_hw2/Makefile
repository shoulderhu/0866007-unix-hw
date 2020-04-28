CC = gcc
CXX = g++
CFLAG = -g -Wall

all: sandbox sandbox.so

%.o: %.cpp
	$(CXX) -c $< $(CFLAG) -std=c++11

sandboxso.o: sandboxso.c
	$(CC) -c $< $(CFLAG) -fPIC

sandbox: sandbox.o
	$(CXX) -o $@ $^

sandbox.so: sandboxso.o
	$(CC) -o $@ $^ -shared -ldl

clean:
	rm -rf sandbox sandbox.o sandbox.so sandboxso.o

.PHONY: all clean
