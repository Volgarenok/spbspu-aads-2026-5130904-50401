#ifndef GRAPH_STORAGE_HPP
#define GRAPH_STORAGE_HPP

#include <string>

#include "graph.hpp"
#include "hash_table.hpp"
#include "hash_functions.hpp"

namespace ulanova
{
  class GraphStorage
  {
  public:
    GraphStorage();

    bool hasGraph(const std::string& name) const;
    Graph* findGraph(const std::string& name);
    const Graph* findGraph(const std::string& name) const;

    void addGraph(const Graph& graph);
    Vector<std::string> getGraphNames() const;

  private:
    HashTable<std::string, Graph, StringHash, StringEqual> graphs_;
  };

  void loadGraphsFromFile(const std::string& filename, GraphStorage& storage);
}

#endif
