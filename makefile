
CXX = g++
CXXFLAGS = -Wall --std=c++11 -I .src/
SRCS := $(wildcard src/*.cpp) 

all: main

main: $(SRCS)
	$(CXX) $(SRCS) $(CXXFLAGS) -o main

clean:
	rm -f main