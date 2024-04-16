
CXX := g++
CXXFLAGS := -Wall --std=c++11 -I .src/
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPS := $(patsubst %.cpp, %.d, $(SRCS))

.PHONY: all clean

all: main

main: $(OBJS)
	$(CXX) $^ $(CXXFLAGS) -o $@

-include $(DEPS)

%.o : %.cpp makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@


clean:
	rm -f $(OBJS) $(DEPS) main