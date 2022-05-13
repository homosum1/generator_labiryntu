#ifndef Maze_hpp
#define Maze_hpp

#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "GraphAsMatrix.hpp"

using namespace std;

struct block {
  stringstream s1, s2, s3;
};

class Maze {
private:
  string rb = "\u001b[31;1m";
  string re = "\u001b[32;1m";
  string gb = "\x1B[37m";
  string ge = "\033[0m";

  int x, y;
  // block blockArray[3][4];
  block** blockArray;
  stringstream result;

public:

  Maze(int xx = 3, int yy = 4): x(xx), y(yy) {
    blockArray = new block*[x];
    for(int i = 0; i < x; ++i)
      blockArray[i] = new block[y];
  }

  ~Maze() {
    for(int i = 0; i < x; ++i)
      delete [] blockArray[i];

    delete [] blockArray;
    
  }

  std::string getMaze() {
    return result.str();
  }

  void createMaze(GraphAsMatrix* graph) {
      int row = 0;
      int column = 0;

      for(int i=0; i<=x*3; i++)
        result << rb << "x" << re;
      result << "\n";

      for(int i=0; i<graph->VerticesCount(); i++) {

        // inicjalizacja kontenerów:
        blockArray[column][row].s1 << gb << "xx" << ge;
        blockArray[column][row].s2 << gb << "xx" << ge;

        // wypełnienie brzegów:
        if(column < x) {
          if( ((graph->SelectEdge(i, i+1)) == NULL) || (((column % (x-1)) == 0) && column != 0)) {
            blockArray[column][row].s1 << rb << "x" << re;
            blockArray[column][row].s2 << rb << "x" << re;
          }
          else
          {
            blockArray[column][row].s1 << gb << "x" << ge;
            blockArray[column][row].s2 << gb << "x" << ge;
          }
        }

        if(row != y-1) {
          if( (graph->SelectEdge(i, i+x)) == NULL)
            blockArray[column][row].s3 << rb << "xxx" << re;
          else
            blockArray[column][row].s3 << gb << "xx" << ge << rb << "x" << re;
        }

        column++;

        if( (column % x ) == 0) {
          // sum up data
          result << rb << "x" << re;
          for(int k=0; k<x; k++)
            result << blockArray[k][row].s1.str();
          result << "\n" << rb << "x" << re;
          for(int k=0; k<x; k++)
            result << blockArray[k][row].s2.str();
          if(row < y-1)
            result << "\n" << rb << "x" << re;
          for(int k=0; k<x; k++)
            result << blockArray[k][row].s3.str();
          result << "\n";

          column = 0;
          row++;
        }
      }

      for(int i=0; i<=x*3; i++)
        result << rb << "x" << re;
      result << "\n";
  }

};


#endif /* Maze_hpp */
