
# kompilator
CXX = g++

#flagi
CXXFLAGS = -Wall -O0 -std=c++20 -pedantic

all: GraphAsMatrix.x

GraphAsMatrix.x : GraphAsMatrix.cpp GraphAsMatrix.hpp Iterator.hpp Maze.hpp PathFiller.hpp
	$(CXX) $(CXXFLAGS)  -o $@ $<
