#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdlib.h>

class Vertex
{
  int number;
public:
  int weight;
  std::string label;

  Vertex(int n, int w = 0, std::string l = ""): number(n), weight(w), label(l) {}

  int Number() const { return number; }
  std::string Label() const { return label; }

};

#endif /* Vertex_hpp */
