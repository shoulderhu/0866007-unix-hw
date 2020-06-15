CXX = g++
CFLAGS = -std=c++11 -g -Wall

all: pno_more_traps

%.o: %.cpp
	$(CXX) -c $< $(CFLAGS)

pno_more_traps: pno_more_traps.o
	$(CXX) -o $@ $^

clean:
	rm -rf pno_more_traps.o pno_more_traps

.PHONY:	all clean