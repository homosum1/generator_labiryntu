#ifndef PathFiller_hpp
#define PathFiller_hpp

#include <iostream>
#include <stdlib.h>
#include <random>
#include <set>
#include <vector>
#include <algorithm>

#include "GraphAsMatrix.hpp"

using namespace std;

class PathFiller {
private:
  int x, y;
  GraphAsMatrix* graph;
  GraphAsMatrix* final_graph;


  void randomizeWeights() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 10*graph->EdgesCount());

    // set<int> usedNumbers;

    for(int i=0; i<graph->VerticesCount(); i++) {
      Iterator<Edge> & incin_it = graph->EmanatingEdgesIter(i);

      while(!incin_it.IsDone()) {
        int potentail_number = distr(gen);

        // nie będzie problemem jeśli przydzielone wagi będą się powtarzać
        /*
        while(usedNumbers.contains(potentail_number))
          potentail_number = distr(gen);
          usedNumbers.insert(potentail_number);
        */

        (*incin_it).weight = potentail_number;
        //cout << "edge: " << (*incin_it).V0()->Number() << "->" << (*incin_it).V1()->Number() << "  potentail_number: " << potentail_number << "\n";
        ++incin_it;
      }
    }

  }

  void fillPath() {
    for(int i=0; i<graph->VerticesCount(); i++) {

      bool leftEdge, rightEdge;

      leftEdge = ((i % x) == 0) ? true : false;
      rightEdge = (( (i-(x-1)) % x) == 0) ? true : false;

      // górna krawędź
      if(i+x < graph->VerticesCount())
        graph->AddEdge(i, i+x);

      // dolna krawędź
      if(i-x >= 0)
        graph->AddEdge(i, i-x);


      if(leftEdge)
        graph->AddEdge(i, i+1);
      else if(rightEdge)
        graph->AddEdge(i, i-1);
      else {
        assert((!leftEdge) && (!rightEdge));
        graph->AddEdge(i, i+1);
        graph->AddEdge(i, i-1);
      }

    }
  }

  void createSecondGraph() {
    final_graph = new GraphAsMatrix(x*y,false);
    // cout << "edgescount: " << graph->EdgesCount() << "\n";
    std::vector<std::pair<int, Edge*>> table;

    for(int i=0; i<graph->EdgesCount(); i++) {
      std::pair<int, Edge*> temp_pair = std::make_pair(-1, new Edge(NULL, NULL));
      table.push_back(temp_pair);
    }

    // filling array for sorting
    int iter = 0;
    Iterator<Edge> & edge_iter = graph->EdgesIter();
    std::set<std::pair<int, int>> duplicates_check;

    while(!edge_iter.IsDone()) {
        int v0 = (*edge_iter).V0()->Number();
        int v1 = (*edge_iter).V1()->Number();

        if(!duplicates_check.contains(std::make_pair(v0,v1)) ) {
          duplicates_check.insert(std::make_pair(v0,v1));
          duplicates_check.insert(std::make_pair(v1,v0));

          table[iter] = std::make_pair( (*edge_iter).weight, &(*edge_iter) );
          //cout << "edge: " << (*edge_iter).V0()->Number() << "->" << (*edge_iter).V1()->Number() << " weight: " << (*edge_iter).weight <<  "\n";
          iter++;
        }

        ++edge_iter;
    }

    // TEST - printing table
    /*
    for(int i=0; i<table.size(); i++)
      cout << "i: " << i << "   weight: " << table[i].first << " " << table[i].second->V0()->Number() << "->" << table[i].second->V1()->Number() << "\n";
    */

    // sorting array
    for (int i = 1; i < table.size(); i++) {
      int key = table[i].first;
      Edge* temp = table[i].second;
      int j = i - 1;

      while (key < table[j].first && j >= 0) {
        table[j + 1] = table[j];
        --j;
      }
      table[j + 1].first = key;
      table[j + 1].second = temp;

    }

    // TEST - printing table
    /*
    cout << "\n\n";
    for(int i=0; i<table.size(); i++)
      cout << "i: " << i << "   weight: " << table[i].first << " " << table[i].second->V0()->Number() << "->" << table[i].second->V1()->Number() << "\n";
    */


    // filling second graphs edges
    std::vector<std::set<int>> used_vertices_sets;
    for(int i=0; i<graph->VerticesCount(); i++)
      used_vertices_sets.push_back(std::set<int>{i});

    for(int i=0; i<table.size(); i++) {
      //cout << "iteration: " << i << "\n";
      std::set<int> v0_set, v1_set;
      int v0_index, v1_index;

      int v0 = table[i].second->V0()->Number();
      int v1 = table[i].second->V1()->Number();

      // szukanie zbioru z v0
      for(int j=0; j<used_vertices_sets.size(); j++)
        if(used_vertices_sets[j].contains(v0)) {
          v0_set = used_vertices_sets[j];
          v0_index = j;
        }

      // szukanie zbioru z v1
      for(int j=0; j<used_vertices_sets.size(); j++)
        if(used_vertices_sets[j].contains(v1)) {
          v1_set = used_vertices_sets[j];
          v1_index = j;
        }


      // dodawanie krawędzie
      if(v0_set != v1_set) {
        final_graph->AddEdge(v0, v1);
        std::set<int> connected_set;

        std::set_union(
             used_vertices_sets[v0_index].begin(), used_vertices_sets[v0_index].end(),
             used_vertices_sets[v1_index].begin(), used_vertices_sets[v1_index].end(),
             std::inserter(connected_set, connected_set.begin())
            );

        // TEST PRINT
        /*
        cout << "\nv0 set: ";
        for (auto it=v0_set.begin(); it != v0_set.end(); ++it)
          cout << ' ' << *it;

        cout << "\nv1 set: ";
        for (auto it=v1_set.begin(); it != v1_set.end(); ++it)
          cout << ' ' << *it;

        cout << "\nconnected_set set: ";
        for (auto it=connected_set.begin(); it != connected_set.end(); ++it)
          cout << ' ' << *it;
        */

        // usunięcie połączonych zbiorów
        used_vertices_sets[v0_index].clear();
        used_vertices_sets[v1_index] = connected_set;

        /*
        for(int k=0; k<used_vertices_sets.size(); k++) {
          cout << "\niter: " << k << " ";
          for (auto it=used_vertices_sets[k].begin(); it != used_vertices_sets[k].end(); ++it)
            cout << ' ' << *it;
        }
        */

        used_vertices_sets.push_back(connected_set);

      }
    }
  }


public:
  PathFiller(int xx, int yy, GraphAsMatrix* gr): x(xx), y(yy), graph(gr) {}
  ~PathFiller() {}

  void getGraph() {
    fillPath();
    randomizeWeights();
    //displayEdges();
    createSecondGraph();
  }

  void displayEdges() {
    for(int i=0; i<graph->VerticesCount(); i++) {
      Iterator<Edge> & incin_it = graph->EmanatingEdgesIter(i);

      cout << "v" << i << ":\n";
      while(!incin_it.IsDone()) {
        cout << " " << (*incin_it).V0()->Number() << "->" << (*incin_it).V1()->Number() << "  weight: " << (*incin_it).weight <<  "\n";
        ++incin_it;
      }
      cout << "\n";
    }
  }

  GraphAsMatrix* shareFinalGraph() {
    return final_graph;
  }

};


#endif /* PathFiller_hpp */
