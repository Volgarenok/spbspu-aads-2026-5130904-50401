#ifndef GRAPH_STORAGE_HPP
#define GRAPH_STORAGE_HPP
#define XXH_INLINE_ALL

#include <string>
#include <stdexcept>
#include "hash_table.hpp"
#include "graph.hpp"
#include "xxhash.hpp"

namespace madieva {
  class GraphStorage {
    List<std::string> order_;
    HashTable< std::string, Graph, XXHash64, StringEqual > graphs_;
  public:
    GraphStorage() = default;
    void addGraph(const std::string & name, const Graph & graph);
    bool hasGraph(const std::string & name) const;
    Graph & getGraph(const std::string & name);
    const Graph & getGraph(const std::string & name) const;
    List<std::string> getAllNames() const;
    void removeGraph(const std::string & name);
    size_t size() const;
    bool empty() const;
  };

  void GraphStorage::addGraph(const std::string & name, const Graph & graph)
  {
    if (graphs_.has(name)) {
      throw std::runtime_error("Graph already exists");
    } else {
      graphs_.add(name, graph);
      order_.push_back(name);
    }
  }

  bool GraphStorage::hasGraph(const std::string& name) const
  {
    return graphs_.has(name);
  }

  Graph& GraphStorage::getGraph(const std::string& name) {
    return graphs_.get(name);
  }

  const Graph& GraphStorage::getGraph(const std::string& name) const {
    return graphs_.get(name);
  }

  List<std::string> GraphStorage::getAllNames() const {
    return order_;
  }

  void GraphStorage::removeGraph(const std::string& name) {
    graphs_.drop(name);
    LIter< std::string > it = order_.begin();
    for (size_t i = 0; i < order_.size(); ++i) {
      if (*it == name) {
        order_.erase(it);
        break;
      }
      ++it;
    }
  }

  size_t GraphStorage::size() const
  {
    return graphs_.size();
  }
  bool GraphStorage::empty() const
  {
    return graphs_.empty();
  }

}


#endif
