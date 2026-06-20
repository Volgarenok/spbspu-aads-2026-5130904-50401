#include "graph.hpp"

void hvostov::Graph::addVertex(const std::string& v)
{
  for (size_t i = 0; i < vertices_.size(); ++i) {
    if (vertices_[i] == v) {
      return;
    }
  }
  vertices_.pushBack(v);
}

void hvostov::Graph::removeVertex(const std::string& v)
{
  for (size_t i = 0; i < vertices_.size(); ++i) {
    if (vertices_[i] == v) {
      vertices_.erase(i);
      break;
    }
  }

  Vector< edge_key > to_remove;
  for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    if ((*it).first.first == v || (*it).first.second == v) {
      to_remove.pushBack((*it).first);
    }
  }
  for (size_t i = 0; i < to_remove.size(); ++i) {
    edges_.drop(to_remove[i]);
  }
}

bool hvostov::Graph::hasVertex(const std::string& v) const
{
  for (size_t i = 0; i < vertices_.size(); ++i) {
    if (vertices_[i] == v) {
      return true;
    }
  }
  return false;
}

void hvostov::Graph::addEdge(const std::string& from, const std::string& to, size_t w)
{
  addVertex(from);
  addVertex(to);
  edge_key key(from, to);
  if (!edges_.has(key)) {
    try {
      edges_.add(key, Vector< size_t >());
    } catch (const std::overflow_error&) {
      edges_.rehash();
      edges_.add(key, Vector< size_t >());
    }
  }
  edges_.at(key).pushBack(w);
}

void hvostov::Graph::removeEdge(const std::string& from, const std::string& to, size_t w)
{
  edge_key key(from, to);
  if (!edges_.has(key)) {
    throw std::out_of_range("Edge not found");
  }
  auto& weights = edges_.at(key);
  for (size_t i = 0; i < weights.size(); ++i) {
    if (weights[i] == w) {
      weights.erase(i);
      if (weights.size() == 0) {
        edges_.drop(key);
      }
      return;
    }
  }
  throw std::out_of_range("Weight not found");
}

bool hvostov::Graph::hasEdge(const std::string& from, const std::string& to) const
{
  return edges_.has(edge_key(from, to));
}
