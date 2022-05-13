#include <iostream>
#include <stdlib.h>
#include <random>
#include <set>
#include <vector>
#include <algorithm>

#include "GraphAsMatrix.hpp"
#include "Iterator.hpp"
#include "Maze.hpp"
#include "PathFiller.hpp"

using namespace std;




int main() {
  int x,y;
  cout << "SELECT MAZE DIMENSIONS: \nx: ";
  cin >> x;
  cout << "y: ";
  cin >> y;
  cout << "selected dimensions:\nx: " << x << "\ny: " << y << "\n\n";


  GraphAsMatrix * graph=new GraphAsMatrix(x*y,false);

  PathFiller pf(x, y, graph);
  pf.getGraph();
  GraphAsMatrix* final_graph = pf.shareFinalGraph();

  Maze myMaze(x,y);
  myMaze.createMaze(final_graph);
  cout << myMaze.getMaze();

}
