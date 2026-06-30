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
  Graph temp(*this);
  for (size_t i = 0; i < temp.vertices_.size(); ++i) {
    if (temp.vertices_[i] == v) {
      temp.vertices_.erase(i);
      break;
    }
  }

  Vector< edge_key > to_remove;
  for (auto it = temp.edges_.begin(); it != temp.edges_.end(); ++it) {
    if ((*it).first.first == v || (*it).first.second == v) {
      to_remove.pushBack((*it).first);
    }
  }
  for (size_t i = 0; i < to_remove.size(); ++i) {
    temp.edges_.remove(to_remove[i]);
  }
  swap(temp);
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
  Graph temp(*this);
  temp.addVertex(from);
  temp.addVertex(to);
  edge_key key(from, to);
  if (!temp.edges_.contains(key)) {
    temp.edges_.add(key, Vector< size_t >());
  }
  temp.edges_.at(key).pushBack(w);
  swap(temp);
}

void hvostov::Graph::removeEdge(const std::string& from, const std::string& to, size_t w)
{
  Graph temp(*this);
  edge_key key(from, to);
  if (!temp.edges_.contains(key)) {
    throw std::out_of_range("Edge not found");
  }
  Vector< size_t >& weights = temp.edges_.at(key);
  for (size_t i = 0; i < weights.size(); ++i) {
    if (weights[i] == w) {
      weights.erase(i);
      if (weights.size() == 0) {
        temp.edges_.remove(key);
      }
      swap(temp);
      return;
    }
  }
  throw std::out_of_range("Weight not found");
}

bool hvostov::Graph::hasEdge(const std::string& from, const std::string& to) const
{
  return edges_.contains(edge_key(from, to));
}

void hvostov::Graph::swap(Graph& other) noexcept
{
  vertices_.swap(other.vertices_);
  edges_.swap(other.edges_);
}
