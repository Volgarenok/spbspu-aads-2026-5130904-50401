#include "commands.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <top-it-vector.hpp>

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

void hvostov::graphs(std::istream&, std::ostream& out, hvostov::GraphTable& graphs)
{
  if (graphs.size() == 0) {
    out << "\n";
    return;
  }
  hvostov::Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    names.pushBack((*it).first);
  }
  sortStrings(names);
  for (size_t i = 0; i < names.size(); ++i) {
    out << names[i] << "\n";
  }
}

void hvostov::vertexes(std::istream& in, std::ostream& out, hvostov::GraphTable& graphs)
{
  std::string name;
  in >> name;
  if (!graphs.has(name)) {
    throw std::logic_error("Graph doesnt exist");
  }

  const hvostov::Graph& g = graphs.at(name);
  if (g.vertices_.size() == 0) {
    out << "\n";
    return;
  }
  hvostov::Vector< std::string > vrts = g.vertices_;
  sortStrings(vrts);
  for (size_t i = 0; i < vrts.size(); ++i) {
    out << vrts[i] << "\n";
  }
}

void hvostov::outbound(std::istream& in, std::ostream& out, hvostov::GraphTable& graphs)
{
  std::string graph_name;
  in >> graph_name;

  if (!graphs.has(graph_name)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  const hvostov::Graph& g = graphs.at(graph_name);

  std::string v;
  in >> v;
  if (!g.hasVertex(v)) {
    throw std::logic_error("Such vertex doesnt exist");
  }

  hvostov::Vector< std::pair< std::string, size_t > > res;
  for (auto it = g.edges_.begin(); it != g.edges_.end(); ++it) {
    if ((*it).first.first == v) {
      const hvostov::Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.size(); ++i) {
        res.pushBack(std::make_pair((*it).first.second, weights[i]));
      }
    }
  }

  if (res.size() == 0) {
    out << "\n";
    return;
  }

  sortPairs(res);

  size_t i = 0;
  while (i < res.size()) {
    std::string cur_vertex = res[i].first;
    out << cur_vertex;

    hvostov::Vector< size_t > cur_weights;
    while (i < res.size() && res[i].first == cur_vertex) {
      cur_weights.pushBack(res[i].second);
      i++;
    }
    sortWeights(cur_weights);
    for (size_t j = 0; j < cur_weights.size(); ++j) {
      out << " " << cur_weights[j];
    }
    out << "\n";
  }
}

void hvostov::inbound(std::istream& in, std::ostream& out, hvostov::GraphTable& graphs)
{
  std::string graph_name;
  in >> graph_name;

  if (!graphs.has(graph_name)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  const hvostov::Graph& g = graphs.at(graph_name);

  std::string v;
  in >> v;
  if (!g.hasVertex(v)) {
    throw std::logic_error("Such vertex doesnt exist");
  }

  hvostov::Vector< std::pair< std::string, size_t > > res;
  for (auto it = g.edges_.begin(); it != g.edges_.end(); ++it) {
    if ((*it).first.second == v) {
      const hvostov::Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.size(); ++i) {
        res.pushBack(std::make_pair((*it).first.first, weights[i]));
      }
    }
  }

  if (res.size() == 0) {
    out << "\n";
    return;
  }

  sortPairs(res);

  size_t i = 0;
  while (i < res.size()) {
    std::string cur_vertex = res[i].first;
    out << cur_vertex;

    hvostov::Vector< size_t > cur_weights;
    while (i < res.size() && res[i].first == cur_vertex) {
      cur_weights.pushBack(res[i].second);
      i++;
    }
    sortWeights(cur_weights);
    for (size_t j = 0; j < cur_weights.size(); ++j) {
      out << " " << cur_weights[j];
    }
    out << "\n";
  }
}

void hvostov::bind(std::istream& in, std::ostream&, hvostov::GraphTable& graphs)
{
  std::string name;
  in >> name;
  if (!graphs.has(name)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;

  try {
    graphs.at(name).addEdge(v1, v2, w);
  } catch (...) {
    graphs.at(name).edges_.rehash();
    graphs.at(name).addEdge(v1, v2, w);
  }
}

void hvostov::cut(std::istream& in, std::ostream&, hvostov::GraphTable& graphs)
{
  std::string name;
  in >> name;
  if (!graphs.has(name)) {
    throw std::logic_error("Such graph doesnt exist");
  }

  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;

  hvostov::Graph& g = graphs.at(name);

  if (!g.hasVertex(v1) || !g.hasVertex(v2)) {
    throw std::logic_error("Vertex doesnt exist");
  }

  hvostov::edge_key key(v1, v2);
  if (!g.edges_.has(key)) {
    throw std::logic_error("Edge doesnt exist");
  }

  auto& weights = g.edges_.at(key);
  bool found = false;
  for (size_t i = 0; i < weights.size(); ++i) {
    if (weights[i] == w) {
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::logic_error("Edge with this weight doesnt exist");
  }

  g.removeEdge(v1, v2, w);
}

void hvostov::create(std::istream& in, std::ostream&, hvostov::GraphTable& graphs)
{
  std::string name;
  in >> name;
  if (graphs.has(name)) {
    throw std::logic_error("Such graph already exist");
  }

  size_t count = 0;
  in >> count;

  hvostov::Graph g;
  for (size_t i = 0; i < count; ++i) {
    std::string v;
    in >> v;
    g.addVertex(v);
  }

  try {
    graphs.add(name, g);
  } catch (...) {
    graphs.rehash();
    graphs.add(name, g);
  }
}

void hvostov::merge(std::istream& in, std::ostream&, hvostov::GraphTable& graphs)
{
  std::string name;
  in >> name;
  if (graphs.has(name)) {
    throw std::logic_error("Such graph already exist");
  }

  std::string g1, g2;
  in >> g1 >> g2;
  if (!graphs.has(g1) || !graphs.has(g2)) {
    throw std::logic_error("One of graphs doesnt exist");
  }

  hvostov::Graph& gr1 = graphs.at(g1);
  hvostov::Graph& gr2 = graphs.at(g2);
  hvostov::Graph gr3;

  for (auto it = gr1.edges_.begin(); it != gr1.edges_.end(); ++it) {
    const hvostov::Vector< size_t >& w = (*it).second;
    for (size_t i = 0; i < w.size(); ++i) {
      gr3.addEdge((*it).first.first, (*it).first.second, w[i]);
    }
  }
  for (auto it = gr2.edges_.begin(); it != gr2.edges_.end(); ++it) {
    const hvostov::Vector< size_t >& w = (*it).second;
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

  try {
    graphs.add(name, gr3);
  } catch (...) {
    graphs.rehash();
    graphs.add(name, gr3);
  }
}

void hvostov::extract(std::istream& in, std::ostream&, hvostov::GraphTable& graphs)
{
  std::string name;
  in >> name;
  if (graphs.has(name)) {
    throw std::logic_error("Such graph already exist");
  }

  std::string old_name;
  in >> old_name;
  if (!graphs.has(old_name)) {
    throw std::logic_error("Graph doesnt exist");
  }

  size_t count;
  in >> count;

  hvostov::Graph gr;
  hvostov::Vector< std::string > vertexes;

  const hvostov::Graph& source = graphs.at(old_name);
  for (size_t i = 0; i < count; ++i) {
    std::string v;
    in >> v;
    if (!source.hasVertex(v)) {
      throw std::logic_error("Such vertex doesnt exist");
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
      const hvostov::Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.size(); ++i) {
        gr.addEdge(from, to, weights[i]);
      }
    }
  }

  try {
    graphs.add(name, gr);
  } catch (...) {
    graphs.rehash();
    graphs.add(name, gr);
  }
}
