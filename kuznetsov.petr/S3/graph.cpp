#include "graph.hpp"

#include <top-it-iters.hpp>

kuznetsov::Graph::Graph():
  table_(),
  vertexes_()
{}

kuznetsov::Graph::Graph(size_t c):
  table_(c),
  vertexes_()
{}

void kuznetsov::Graph::addVertexes(const std::string& name)
{
  if (vertexes_.contain(name)) {
    return;
  }
  vertexes_.pushBack(name);
}

void kuznetsov::Graph::removeVertexes(const std::string& name)
{
  if (!vertexes_.contain(name)) {
    return;
  }
  Graph tmp(*this);

  for (auto it = tmp.vertexes_.cbegin(); it != tmp.vertexes_.cend(); ++it) {
    if (*it == name) {
      tmp.vertexes_.erase(it);
      break;
    }
  }

  Vector< key > toRemove;
  for (auto it = tmp.table_.begin(); it != tmp.table_.end(); ++it) {
    if (it->first.first == name || it->first.second == name) {
      toRemove.pushBack(it->first);
    }
  }
  for (size_t i = 0; i < toRemove.getSize(); ++i) {
    tmp.table_.remove(toRemove[i]);
  }

  table_.swap(tmp.table_);
  vertexes_.swap(tmp.vertexes_);
}

void kuznetsov::Graph::addEdge(const std::string& v1, const std::string& v2, size_t w)
{
  Graph tmp(*this);
  tmp.addVertexes(v1);
  tmp.addVertexes(v2);
  auto k = std::make_pair(v1, v2);
  if (tmp.table_.contains(k)) {
    tmp.table_.at(k).pushBack(w);
  } else {
    tmp.table_.add(k, Vector< size_t >());
    tmp.table_.at(k).pushBack(w);
  }
  table_.swap(tmp.table_);
  vertexes_.swap(tmp.vertexes_);
}

void kuznetsov::Graph::removeEdge(const std::string& v1, const std::string& v2, size_t w)
{
  auto k = std::make_pair(v1, v2);
  if (table_.contains(k)) {
    Vector< size_t >& slot = table_.at(k);
    if (slot.getSize() > 1) {
      for (auto it = slot.cbegin(); it != slot.cend(); ++it) {
        if ((*it) == w) {
          it = slot.erase(it);
          return;
        }
      }
    } else {
      table_.remove(k);
    }
  }
}
