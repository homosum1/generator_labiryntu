#ifndef GraphAsMatrix_hpp
#define GraphAsMatrix_hpp

#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <sstream>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Iterator.hpp"

class GraphAsMatrix {

  /* POWTÓRZ TAKIE SAMY TESTY DLA GRAFU NIESKIEROWANEGO !!! */

private:
  std::vector<Vertex *> vertices;
  std::vector< std::vector<Edge*> > adjacencyMatrix;

  bool isDirected;
  bool isConnected;
  int numberOfVertices;
  int numberOfEdges=0;


  ///////////////// KLASY ITERATORÓW /////////////////////


  // przechodzimy po liście wierzchołków
  class AllVerticesIter: public Iterator<Vertex> {
    GraphAsMatrix & owner;
    int current;
  public:
    AllVerticesIter(GraphAsMatrix &o): owner(o), current(0) {}

    bool IsDone () const {
      return current >= owner.NumberOfVertices() ;
    }

    Vertex & operator*() const {
      return *owner.SelectVertex(current);
    }

    void  operator++() {
      if(IsDone() == false)
        current++;
    }
  };

  // przechodzimy po dwuwymiarowej tablicy krawędzi
  class AllEdgesIter: public Iterator<Edge> {
    GraphAsMatrix & owner;
    int row;
    int col;
  public:
    AllEdgesIter(GraphAsMatrix &o): owner(o), row(0), col(0) {
      next();
    }

    void next() {
      Edge* ptr = owner.adjacencyMatrix[row][col];

      if( (row+col) != 0) {
        if(!IsDone() ) {
          if(col < owner.NumberOfVertices()-1){
            col++;
          } else {
            if(row == owner.NumberOfVertices()-1)
             return;
            col = 0;
            row++;
          }
        }
        ptr = owner.adjacencyMatrix[row][col];
      }

      while((ptr == NULL) && (!IsDone()) ) {
        if(col < owner.NumberOfVertices()-1){
          col++;
        } else {
          if(row == owner.NumberOfVertices()-1)
           return;
          col = 0;
          row++;
        }

        ptr = owner.adjacencyMatrix[row][col];
      }
    }

    bool IsDone() const {
      return ((row >= (owner.NumberOfVertices()-1)) && (col >= (owner.NumberOfVertices()-1)) );
    }

    Edge & operator*() const {
      return *owner.SelectEdge(row, col);
    }

    Edge* operator-() const {
      return owner.SelectEdge(row,col);
    }

    void  operator++(){
      if(!IsDone()) {
        next();
      }
    }

   };

   // przeszukujemy wszystkie krawędzie w danym wierszu różne od nulla co odpowiada przechodzeniu
   // po wszystkich krawędziach stykających się z danym wierzchołkiem (iterator po krawędziach wychodzących
   // z danego wierzchołka)
   class EmanEdgesIter: public Iterator<Edge> {
   private:
     GraphAsMatrix & owner;
     int row;
     int col;

     bool first_loop = true;
     bool isDone = false;

     void check_overloop(int p_row, int p_col) {
       if(col == (owner.NumberOfVertices()) )  {
         row = p_row;
         col = p_col;
         isDone = true;
       }
     }

   public:

     void next() {
       Edge* ptr = owner.adjacencyMatrix[row][col];
       int p_row = row;
       int p_col = col;

       if(!IsDone() && !first_loop){
         col++;
         check_overloop(p_row, p_col);
         ptr = owner.adjacencyMatrix[row][col];
       }

       while((ptr == NULL) && (!IsDone()) ) {
         col++;
         ptr = owner.adjacencyMatrix[row][col];
         check_overloop(p_row, p_col);
       }

       first_loop = false;
     }

     EmanEdgesIter(GraphAsMatrix &o,int v): owner(o) {
       assert(v < owner.NumberOfVertices());
       row = v;
       col = 0;

       next();
     }

     bool IsDone() const {
       return ((col > (owner.NumberOfVertices()-1)) || isDone);
     }

     Edge & operator*() const {
       return *owner.SelectEdge(row, col);
     }

     void  operator++(){
       if(!IsDone()) {
         next();
       }
     }
   };

   //  to samo co poprzednio ale przechodzimy wszystkie krawędzie w danej kolumnie
   // ( iterator po krawędziach dochodzących do danego wierzchołka )
   class InciEdgesIter: public Iterator<Edge> {
   private:
     GraphAsMatrix & owner;
     int row;
     int col;

     bool first_loop = true;
     bool isDone = false;

     void check_overloop(int p_row, int p_col) {
       if(row == (owner.NumberOfVertices()) )  {
         row = p_row;
         col = p_col;
         isDone = true;
       }
     }

   public:

     void next() {
       Edge* ptr = owner.adjacencyMatrix[row][col];
       int p_row = row;
       int p_col = col;

       if(!IsDone() && !first_loop){
         row++;
         check_overloop(p_row, p_col);
         ptr = owner.adjacencyMatrix[row][col];
       }

       while((ptr == NULL) && (!IsDone()) ) {
         row++;
         check_overloop(p_row, p_col);
         ptr = owner.adjacencyMatrix[row][col];
       }

       first_loop = false;
     }

     InciEdgesIter(GraphAsMatrix &o,int v): owner(o) {
       assert(v < owner.NumberOfVertices());
       row = 0;
       col = v;
       next();
     }

     bool IsDone() const {
       return ((row > (owner.NumberOfVertices()-1)) || isDone);
     }

     Edge & operator*() const {
       return *owner.SelectEdge(row, col);
     }

     void  operator++(){
       if(!IsDone()) {
         next();
       }
     }
   };

   ////////////////////////////////////////////////////////

public:

  GraphAsMatrix (int n, bool b): isDirected(b), numberOfVertices(n) {

    std::vector<Edge* > inner_matrix;
    for(int i=0; i<numberOfVertices; i++)
      inner_matrix.push_back(NULL);

    for(int i=0; i<numberOfVertices; i++) {
      vertices.push_back(new Vertex(i));
      adjacencyMatrix.push_back(inner_matrix);
    }
  }

  int NumberOfVertices() {
    return numberOfVertices;
  }

  bool IsDirected() {
    return isDirected;
  }

  int NumberOfEdges() {
    return numberOfEdges;
  }

  bool IsEdge(int u, int v) {
    Edge* edge = adjacencyMatrix[u][v];

    if(edge != NULL)
      return true;
    else
      return false;
  }

  void MakeNull() {
    for(int i = 0; i < numberOfVertices; i++) {
      for(int j=0; j < numberOfVertices; j++) {
        delete adjacencyMatrix[i][j];
        adjacencyMatrix[i][j] = NULL;
      }
    }
  }

     void AddEdge (int u, int v) {
       assert( (u >= 0) && (v >= 0) );
       //std::cout <<  "u: " << u << " v: " << v << "\n";
       assert( (u < numberOfVertices) && (v < numberOfVertices) );
       assert( (vertices[u] != NULL) && (vertices[v] != NULL) );

       // check for existing edge
       Edge* e = adjacencyMatrix[u][v];
       if(e != NULL)
         return;

       adjacencyMatrix[u][v] = new Edge(vertices[u], vertices[v]);

       if(!isDirected)
        adjacencyMatrix[v][u] = new Edge(vertices[v], vertices[u]);

       numberOfEdges++;
     }

     void AddEdge (Edge* edge) {
       Vertex v1 = *(edge->V0());
       Vertex v2 = *(edge->V1());

       adjacencyMatrix[v1.Number()][v2.Number()] = edge;

       if(!isDirected)
        adjacencyMatrix[v2.Number()][v1.Number()] = new Edge(edge->V1(), edge->V0());

       numberOfEdges++;
     }

     Edge * SelectEdge (int u, int v) {
       return adjacencyMatrix[u][v];
     }

     Vertex * SelectVertex(int v) {
       return vertices[v];
     }


     ///////////////// ITERATORY ////////////////////////////

     AllVerticesIter & VerticesIter() {
       return *new AllVerticesIter(*this);
     }

     AllEdgesIter & EdgesIter() {
       return *new AllEdgesIter(*this);
     }

     EmanEdgesIter & EmanatingEdgesIter(int v) {
      return *new EmanEdgesIter(*this, v);
     };

     InciEdgesIter & IncidentEdgesIter(int v) {
       return *new InciEdgesIter(*this, v);
     }

     ////////////////////////////////////////////////////////


     ///////////////// METODY DFS ///////////////////////////

     void DFS(Vertex* v, bool printData = false) {
        std::vector<bool> visited_array(vertices.size(), false);
        std::stringstream ss;

        DFS1(v, visited_array, ss);
        for(int i=0; i<visited_array.size(); i++) {
          if(!visited_array[i]){
            ss << "odwiedzenie części grafu niespójnego dla wierzchołka: " << i << "\n";
            isConnected = false;
            DFS1(vertices[i], visited_array, ss);
          }
        }

        if(printData) {
          std::cout << ss.str();
        }
     }

     void DFS1(Vertex *v, std::vector<bool> & visited, std::stringstream& ss) {
       int v_number = v->Number();
       visited[v_number] = true;

       ss << "odwiedzono wierzchołek: " << v_number << " \n";
       Iterator<Edge> & incin_it = this->EmanatingEdgesIter(v_number);

       while(!incin_it.IsDone()) {
         int inner_number = (*incin_it).V1()->Number();
         ss << "wierzchołek: " << inner_number << "\n";
         if(!visited[inner_number])
          DFS1((*incin_it).V1(), visited, ss);

         ++incin_it;
       }

     }

     bool IsConnected() {
       isConnected = true;

       if(!isDirected){
         DFS(this->SelectVertex(0));
       }
       else {
         for(int i=0; i<vertices.size(); i++)
          DFS(this->SelectVertex(i));
       }

       return isConnected;
     }

     ////////////////////////////////////////////////////////

     int VerticesCount() {
       return numberOfVertices;
     }

     int EdgesCount() {
       return numberOfEdges;
     }
};

#endif /* GraphAsMatrix_hpp */
