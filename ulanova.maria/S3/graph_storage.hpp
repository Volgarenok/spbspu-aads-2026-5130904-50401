#ifndef GRAPH_STORAGE_HPP
#define GRAPH_STORAGE_HPP

#include <string>
#include <iosfwd>

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

    Vector< std::string > getGraphNames() const;

  private:
    HashTable< std::string, Graph, StringHash, StringEqual > graphs_;
  };

  std::istream& loadGraphs(std::istream& input, GraphStorage& storage);
}

#endif
