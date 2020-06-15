CXX = g++
CFLAGS = -std=c++11 -g -Wall

all: pcountme

%.o: %.cpp
	$(CXX) -c $< $(CFLAGS)

pcountme: pcountme.o
	$(CXX) -o $@ $^

clean:
	rm -rf pcountme.o pcountme

.PHONY:	all clean