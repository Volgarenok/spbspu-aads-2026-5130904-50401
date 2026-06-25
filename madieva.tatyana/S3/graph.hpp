#ifndef GRAPH_HPP
#define GRAPH_HPP
#define XXH_INLINE_ALL

#include <string>
#include "hash_table.hpp"
#include "xxhash.hpp"

namespace madieva {
  class Graph {
    std::string name_graph;
    using key = std::pair<std::string, std::string>;
    using val = List< size_t >;
    HashTable< key, val, XXHash64, PairEqual > edge_;
    List< std::string > vertex_;
  public:
    Graph(const std::string & name);
    const std::string & getName() const;
    void addVertex(const std::string & v);
    bool hasVertex(const std::string & v) const;
    const List< std::string > & getVertices() const;
    HashTable< key, val, XXHash64, PairEqual > & getEdges();
    const HashTable< key, val, XXHash64, PairEqual > & getEdges() const;
    void addEdge(const std::string & a, const std::string & b, size_t e);
    void addEdge(const key & a, size_t weight);

  };

  Graph::Graph(const std::string & name) :
    name_graph(name)
  {}

  const std::string & Graph::getName() const
  {
    return name_graph;
  }

  void Graph::addVertex(const std::string & v)
  {
    if (!hasVertex(v)) {
      vertex_.push_back(v);
    }
  }

  bool Graph::hasVertex(const std::string & v) const
  {
    LCIter< std::string > it = vertex_.begin();
    for (size_t i = 0; i < vertex_.size(); ++i) {
      if ((*it) == v) {
        return true;
      }
      ++it;
    }
    return false;
  }

  const List< std::string > & Graph::getVertices() const
  {
    return vertex_;
  }

  HashTable< Graph::key, Graph::val, XXHash64, PairEqual > & Graph::getEdges()
  {
    return edge_;
  }

  const HashTable< Graph::key, Graph::val, XXHash64, PairEqual > & Graph::getEdges() const
  {
    return edge_;
  }

  void Graph::addEdge(const std::string & a, const std::string & b, size_t weight)
  {
    key k = {a, b};
    addEdge(k, weight);
  }

  void Graph::addEdge(const key & a, size_t weight)
  {
    if (edge_.has(a)) {
      edge_.get(a).push_back(weight);
    } else {
      List< size_t > list;
      list.push_back(weight);
      edge_.add(a, list);
    }
    addVertex(a.first);
    addVertex(a.second);
  }


}

#endif
