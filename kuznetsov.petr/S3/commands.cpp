#include "commands.hpp"

#include <iostream>
#include <utility>

#include <top-it-vector.hpp>

namespace {
  struct PairComparator {
    using pair_t = std::pair< std::string, size_t >;
    bool operator()(const pair_t& p1, const pair_t& p2)
    {
      if (p1.first != p2.first) {
        return p1.first < p2.first;
      }
      return p1.second < p2.second;
    }
  };
}

template< class T, class Cmp >
void sort(kuznetsov::Vector< T >& v, Cmp cmp)
{
  for (size_t i = 0; i < v.getSize(); ++i) {
    size_t min = i;
    for (size_t j = i + 1; j < v.getSize(); ++j) {
      if (cmp(v[j], v[min])) {
        min = j;
      }
    }
    if (min != i) {
      std::swap(v[i], v[min]);
    }
  }
}

void kuznetsov::graphs(std::ostream& out, std::istream&, const table& t)
{
  if (t.size() == 0) {
    return;
  }
  Vector< std::string > names;
  for (auto it = t.cbegin(); it != t.cend(); ++it) {
    names.pushBack(it->first);
  }
  sort(names, std::less< std::string >{});
  auto it = names.cbegin();
  out << *it;
  ++it;
  for (; it != names.cend(); ++it) {
    out << '\n' << *it;
  }
}

void kuznetsov::vertexes(std::ostream& out, std::istream& in, const table& t)
{
  std::string name;
  in >> name;
  if (!t.contains(name)) {
    throw std::logic_error("Graph doesnt exist");
  }

  const Graph& g = t.at(name);
  if (g.vertexes_.isEmpty()) {
    return;
  }
  Vector< std::string > vrts(g.vertexes_);
  sort(vrts, std::less< std::string >{});
  auto it = vrts.cbegin();
  out << *it;
  ++it;
  for (; it != vrts.cend(); ++it) {
    out << '\n' << *it;
  }
}

void kuznetsov::outbound(std::ostream& out, std::istream& in, const table& t)
{
  std::string nameGraph;
  in >> nameGraph;

  if (!t.contains(nameGraph)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  const Graph& g = t.at(nameGraph);
  std::string v;
  in >> v;
  if (!g.vertexes_.contain(v)) {
    throw std::logic_error("Such vertex doesnt exist");
  }
  Vector< std::pair< std::string, size_t > > res;
  auto it = g.table_.begin();
  while (it != g.table_.end()) {
    if (it->first.first == v) {
      const Vector< size_t >& weights = it->second;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        res.pushBack(std::make_pair(it->first.second, weights[i]));
      }
    }
    ++it;
  }
  if (res.isEmpty()) {
    return;
  }
  sort(res, PairComparator{});
  auto itr = res.cbegin();
  out << itr->first << ' ' << itr->second;
  ++itr;
  for (; itr != res.cend(); ++itr) {
    if ((*(itr - 1)).first == itr->first) {
      out << ' ' << itr->second;
    } else {
      out << '\n' << itr->first << ' ' << itr->second;
    }
  }
}

void kuznetsov::inbound(std::ostream& out, std::istream& in, const table& t)
{
  std::string nameGraph;
  in >> nameGraph;

  if (!t.contains(nameGraph)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  const Graph& g = t.at(nameGraph);
  std::string v;
  in >> v;
  if (!g.vertexes_.contain(v)) {
    throw std::logic_error("Such vertex doesnt exist");
  }
  Vector< std::pair< std::string, size_t > > res;
  auto it = g.table_.begin();
  while (it != g.table_.end()) {
    if (it->first.second == v) {
      const Vector< size_t >& weights = it->second;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        res.pushBack(std::make_pair(it->first.first, weights[i]));
      }
    }
    ++it;
  }
  if (res.isEmpty()) {
    return;
  }
  sort(res, PairComparator{});
  auto itr = res.cbegin();
  out << itr->first << ' ' << itr->second;
  ++itr;
  for (; itr != res.cend(); ++itr) {
    if ((*(itr - 1)).first == itr->first) {
      out << ' ' << itr->second;
    } else {
      out << '\n' << itr->first << ' ' << itr->second;
    }
  }
}

void kuznetsov::bind(std::ostream&, std::istream& in, table& t)
{
  std::string name;
  in >> name;
  if (!t.contains(name)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  t.at(name).addEdge(v1, v2, w);
}

void kuznetsov::cut(std::ostream&, std::istream& in, table& t)
{
  std::string name;
  in >> name;
  if (!t.contains(name)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  Graph& g = t.at(name);
  bool f = !g.vertexes_.contain(v1);
  f = f || !g.vertexes_.contain(v2);
  f = f || !g.table_.contains(std::make_pair(v1, v2));
  f = f || !g.table_.at(std::make_pair(v1, v2)).contain(w);
  if (f) {
    throw std::logic_error("Vertex or edge with this weight doesnt exist");
  }
  g.removeEdge(v1, v2, w);
}

void kuznetsov::create(std::ostream&, std::istream& in, table& t)
{
  std::string name;
  size_t count = 0;
  std::string v;
  in >> name;
  if (t.contains(name)) {
    throw std::logic_error("Such graph already exist");
  }
  in >> count;
  Graph g(count);
  for (size_t i = 0; i < count; ++i) {
    in >> v;
    g.addVertexes(v);
  }
  t.add(name, g);
  if (std::cin.fail()) {
    std::cin.clear();
  }
}

void kuznetsov::merge(std::ostream&, std::istream& in, table& t)
{
  std::string name;
  in >> name;
  if (t.contains(name)) {
    throw std::logic_error("Such graph already exist");
  }
  std::string g1, g2;
  in >> g1 >> g2;
  if (!t.contains(g1) || !t.contains(g2)) {
    throw std::logic_error("One of graphs doesnt exist");
  }
  Graph& gr1 = t.at(g1);
  Graph& gr2 = t.at(g2);
  Graph gr3;

  for (auto it1 = gr1.table_.begin(); it1 != gr1.table_.end(); ++it1) {
    const Vector< size_t >& w = it1->second;
    for (size_t i = 0; i < w.getSize(); ++i) {
      gr3.addEdge(it1->first.first, it1->first.second, w[i]);
    }
  }
  for (auto it1 = gr2.table_.begin(); it1 != gr2.table_.end(); ++it1) {
    const Vector< size_t >& w = it1->second;
    for (size_t i = 0; i < w.getSize(); ++i) {
      gr3.addEdge(it1->first.first, it1->first.second, w[i]);
    }
  }

  for (size_t i = 0; i < gr1.vertexes_.getSize(); ++i) {
    gr3.addVertexes(gr1.vertexes_[i]);
  }

  for (size_t i = 0; i < gr2.vertexes_.getSize(); ++i) {
    gr3.addVertexes(gr2.vertexes_[i]);
  }
  t.add(name, gr3);
}

void kuznetsov::extract(std::ostream&, std::istream& in, table& t)
{
  std::string name;
  in >> name;
  if (t.contains(name)) {
    throw std::logic_error("Such graph already exist");
  }
  std::string g1;
  in >> g1;
  if (!t.contains(g1)) {
    throw std::logic_error("graph doesnt exist");
  }

  size_t count;
  in >> count;

  Graph gr(count);
  Vector< std::string > vertexes;

  const Graph& source = t.at(g1);
  for (size_t i = 0; i < count; ++i) {
    std::string v;
    in >> v;
    if (!source.vertexes_.contain(v)) {
      throw std::logic_error("Such vertex doesnt exist");
    }
    gr.addVertexes(v);
    vertexes.pushBack(v);
  }

  for (auto it = source.table_.begin(); it != source.table_.end(); ++it) {
    const std::string& from = it->first.first;
    const std::string& to = it->first.second;

    bool fromExists = false, toExists = false;
    for (size_t i = 0; i < vertexes.getSize(); ++i) {
      if (vertexes[i] == from) {
        fromExists = true;
      }
      if (vertexes[i] == to) {
        toExists = true;
      }
    }

    if (fromExists && toExists) {
      const Vector< size_t >& weights = it->second;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        gr.addEdge(from, to, weights[i]);
      }
    }
  }
  t.add(name, gr);
}
