#include "commands.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <top-it-vector.hpp>

namespace {
  void sortStrings(hvostov::Vector< std::string >& v)
  {
    for (size_t i = 0; i < v.size(); ++i) {
      size_t min = i;
      for (size_t j = i + 1; j < v.size(); ++j) {
        if (v[j] < v[min]) {
          min = j;
        }
      }
      if (min != i) {
        std::swap(v[i], v[min]);
      }
    }
  }

  void sortWeights(hvostov::Vector< size_t >& v)
  {
    for (size_t i = 0; i < v.size(); ++i) {
      size_t min = i;
      for (size_t j = i + 1; j < v.size(); ++j) {
        if (v[j] < v[min]) {
          min = j;
        }
      }
      if (min != i) {
        std::swap(v[i], v[min]);
      }
    }
  }

  struct PairComparator {
    bool operator()(const std::pair< std::string, size_t >& a, const std::pair< std::string, size_t >& b) const
    {
      if (a.first != b.first) {
        return a.first < b.first;
      }
      return a.second < b.second;
    }
  };

  void sortPairs(hvostov::Vector< std::pair< std::string, size_t > >& v)
  {
    PairComparator cmp;
    for (size_t i = 0; i < v.size(); ++i) {
      size_t min = i;
      for (size_t j = i + 1; j < v.size(); ++j) {
        if (cmp(v[j], v[min])) {
          min = j;
        }
      }
      if (min != i) {
        std::swap(v[i], v[min]);
      }
    }
  }

  void printEdges(std::ostream& out, hvostov::Vector< std::pair< std::string, size_t > >& res)
  {
    const char* sep = "";
    size_t i = 0;
    while (i < res.size()) {
      std::string cur_vertex = res[i].first;
      out << sep << cur_vertex;
      sep = "\n";

      hvostov::Vector< size_t > cur_weights;
      while (i < res.size() && res[i].first == cur_vertex) {
        cur_weights.pushBack(res[i].second);
        i++;
      }
      sortWeights(cur_weights);
      for (size_t j = 0; j < cur_weights.size(); ++j) {
        out << " " << cur_weights[j];
      }
    }
  }
}

void hvostov::graphs(std::istream&, std::ostream& out, const GraphTable& graphs)
{
  if (graphs.size() == 0) {
    return;
  }
  Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    names.pushBack((*it).first);
  }
  sortStrings(names);
  const char* sep = "";
  for (size_t i = 0; i < names.size(); ++i) {
    out << sep << names[i];
    sep = "\n";
  }
}

void hvostov::vertexes(std::istream& in, std::ostream& out, const GraphTable& graphs)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Invalid input\n");
  }
  if (!graphs.contains(name)) {
    throw std::logic_error("Graph doesnt exist\n");
  }

  const Graph& g = graphs.at(name);
  if (g.vertices_.size() == 0) {
    return;
  }
  Vector< std::string > vrts = g.vertices_;
  sortStrings(vrts);
  const char* sep = "";
  for (size_t i = 0; i < vrts.size(); ++i) {
    out << sep << vrts[i];
    sep = "\n";
  }
}

void hvostov::outbound(std::istream& in, std::ostream& out, const GraphTable& graphs)
{
  std::string graph_name;
  if (!(in >> graph_name)) {
    throw std::logic_error("Invalid input\n");
  }

  if (!graphs.contains(graph_name)) {
    throw std::logic_error("Such graph doesnt exist\n");
  }
  const Graph& g = graphs.at(graph_name);

  std::string v;
  if (!(in >> v)) {
    throw std::logic_error("Invalid input\n");
  }
  if (!g.hasVertex(v)) {
    throw std::logic_error("Such vertex doesnt exist\n");
  }

  Vector< std::pair< std::string, size_t > > res;
  for (auto it = g.edges_.begin(); it != g.edges_.end(); ++it) {
    if ((*it).first.first == v) {
      const Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.size(); ++i) {
        res.pushBack(std::make_pair((*it).first.second, weights[i]));
      }
    }
  }

  if (res.size() == 0) {
    return;
  }

  sortPairs(res);
  printEdges(out, res);
}

void hvostov::inbound(std::istream& in, std::ostream& out, const GraphTable& graphs)
{
  std::string graph_name;
  if (!(in >> graph_name)) {
    throw std::logic_error("Invalid input\n");
  }

  if (!graphs.contains(graph_name)) {
    throw std::logic_error("Such graph doesnt exist\n");
  }
  const Graph& g = graphs.at(graph_name);

  std::string v;
  if (!(in >> v)) {
    throw std::logic_error("Invalid input\n");
  }
  if (!g.hasVertex(v)) {
    throw std::logic_error("Such vertex doesnt exist\n");
  }

  Vector< std::pair< std::string, size_t > > res;
  for (auto it = g.edges_.begin(); it != g.edges_.end(); ++it) {
    if ((*it).first.second == v) {
      const Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.size(); ++i) {
        res.pushBack(std::make_pair((*it).first.first, weights[i]));
      }
    }
  }

  if (res.size() == 0) {
    return;
  }

  sortPairs(res);
  printEdges(out, res);
}

void hvostov::bind(std::istream& in, std::ostream&, GraphTable& graphs)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Invalid input\n");
  }
  if (!graphs.contains(name)) {
    throw std::logic_error("Such graph doesnt exist\n");
  }
  std::string v1, v2;
  size_t w;
  if (!(in >> v1 >> v2 >> w)) {
    throw std::logic_error("Invalid input\n");
  }

  graphs.at(name).addEdge(v1, v2, w);
}

void hvostov::cut(std::istream& in, std::ostream&, GraphTable& graphs)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Invalid input\n");
  }
  if (!graphs.contains(name)) {
    throw std::logic_error("Such graph doesnt exist\n");
  }

  std::string v1, v2;
  size_t w;
  if (!(in >> v1 >> v2 >> w)) {
    throw std::logic_error("Invalid input\n");
  }

  Graph& g = graphs.at(name);

  if (!g.hasVertex(v1) || !g.hasVertex(v2)) {
    throw std::logic_error("Vertex doesnt exist\n");
  }

  g.removeEdge(v1, v2, w);
}

void hvostov::create(std::istream& in, std::ostream&, GraphTable& graphs)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Invalid input\n");
  }
  if (graphs.contains(name)) {
    throw std::logic_error("Such graph already exist\n");
  }

  size_t count = 0;
  if (!(in >> count)) {
    throw std::logic_error("Invalid input\n");
  }

  Graph g;
  for (size_t i = 0; i < count; ++i) {
    std::string v;
    if (!(in >> v)) {
      throw std::logic_error("Invalid input\n");
    }
    g.addVertex(v);
  }

  graphs.add(name, g);
}

void hvostov::merge(std::istream& in, std::ostream&, GraphTable& graphs)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Invalid input\n");
  }
  if (graphs.contains(name)) {
    throw std::logic_error("Such graph already exist\n");
  }

  std::string g1, g2;
  if (!(in >> g1 >> g2)) {
    throw std::logic_error("Invalid input\n");
  }
  if (!graphs.contains(g1) || !graphs.contains(g2)) {
    throw std::logic_error("One of graphs doesnt exist\n");
  }

  Graph& gr1 = graphs.at(g1);
  Graph& gr2 = graphs.at(g2);
  Graph gr3;

  for (auto it = gr1.edges_.begin(); it != gr1.edges_.end(); ++it) {
    const Vector< size_t >& w = (*it).second;
    for (size_t i = 0; i < w.size(); ++i) {
      gr3.addEdge((*it).first.first, (*it).first.second, w[i]);
    }
  }
  for (auto it = gr2.edges_.begin(); it != gr2.edges_.end(); ++it) {
    const Vector< size_t >& w = (*it).second;
    for (size_t i = 0; i < w.size(); ++i) {
      gr3.addEdge((*it).first.first, (*it).first.second, w[i]);
    }
  }

  for (size_t i = 0; i < gr1.vertices_.size(); ++i) {
    gr3.addVertex(gr1.vertices_[i]);
  }
  for (size_t i = 0; i < gr2.vertices_.size(); ++i) {
    gr3.addVertex(gr2.vertices_[i]);
  }

  graphs.add(name, gr3);
}

void hvostov::extract(std::istream& in, std::ostream&, GraphTable& graphs)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Invalid input\n");
  }
  if (graphs.contains(name)) {
    throw std::logic_error("Such graph already exist\n");
  }

  std::string old_name;
  if (!(in >> old_name)) {
    throw std::logic_error("Invalid input\n");
  }
  if (!graphs.contains(old_name)) {
    throw std::logic_error("Graph doesnt exist\n");
  }

  size_t count;
  if (!(in >> count)) {
    throw std::logic_error("Invalid input\n");
  }

  Graph gr;
  Vector< std::string > vertexes;

  const Graph& source = graphs.at(old_name);
  for (size_t i = 0; i < count; ++i) {
    std::string v;
    if (!(in >> v)) {
      throw std::logic_error("Invalid input\n");
    }
    if (!source.hasVertex(v)) {
      throw std::logic_error("Such vertex doesnt exist\n");
    }
    gr.addVertex(v);
    vertexes.pushBack(v);
  }

  for (auto it = source.edges_.begin(); it != source.edges_.end(); ++it) {
    const std::string& from = (*it).first.first;
    const std::string& to = (*it).first.second;

    bool fromExists = false;
    bool toExists = false;
    for (size_t i = 0; i < vertexes.size(); ++i) {
      if (vertexes[i] == from) {
        fromExists = true;
      }
      if (vertexes[i] == to) {
        toExists = true;
      }
    }

    if (fromExists && toExists) {
      const Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.size(); ++i) {
        gr.addEdge(from, to, weights[i]);
      }
    }
  }

  graphs.add(name, gr);
}
