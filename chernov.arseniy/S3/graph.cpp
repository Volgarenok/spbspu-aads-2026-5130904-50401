#include "graph.hpp"

#include <iostream>

template< class T, class Cmp >
void chernov::sort(Vector< T > & v, Cmp cmp)
{
  for (size_t i = 0; i < v.getSize(); ++i) {
    size_t min = i;
    for (size_t j = i + 1; j < v.getSize(); ++j) {
      if (cmp(v[j], v[min])) {
        min = j;
      }
    }
    if (min != i) {
      std::swap(v[min], v[i]);
    }
  }
}

chernov::Edges::Edges():
  edges_(64)
{}

void chernov::Edges::addEdge(std::string vertex, size_t weight)
{
  if (!edges_.has(vertex)) {
    try {
      edges_.add(vertex, Vector< size_t >());
    } catch (const std::length_error & e) {
      edges_.rehash(edges_.maxCapacity() ? edges_.maxCapacity() * 2 : 2);
      edges_.add(vertex, Vector< size_t >());
    }
  }
  edges_.at(vertex).pushBack(weight);
}

void chernov::Edges::cutEdge(std::string vertex, size_t weight)
{
  Vector< size_t > & weights = edges_.at(vertex);
  for (auto iter = weights.begin(); iter != weights.end(); ++iter) {
    if (*iter == weight) {
      weights.erase(iter);
      return;
    }
  }
  throw std::out_of_range("edge not found");
}

chernov::Vector< std::pair< std::string, size_t > > chernov::Edges::getEdges() const
{
  Vector< std::pair< std::string, size_t > > edges;
  for (auto ht_iter = edges_.cbegin(); ht_iter != edges_.cend(); ++ht_iter) {
    for (auto v_iter = ht_iter->second.cbegin(); v_iter != ht_iter->second.cend(); ++v_iter) {
      edges.pushBack({ht_iter->first, *v_iter});
    }
  }
  return edges;
}

chernov::Graph::Graph(std::string name):
  name_(name),
  incoming_(64),
  outgoing_(64)
{}

void chernov::Graph::addEdge(std::string start_vertex, std::string end_vertex, size_t weight)
{
  auto ensure_exists = [&](auto & table, const std::string & key) {
    if (!table.has(key)) {
      try {
        table.add(key, Edges());
      } catch (const std::length_error &) {
        table.rehash(table.maxCapacity() ? table.maxCapacity() * 2 : 2);
        table.add(key, Edges());
      }
    }
  };

  ensure_exists(incoming_, end_vertex);
  ensure_exists(outgoing_, start_vertex);

  incoming_.at(end_vertex).addEdge(start_vertex, weight);
  outgoing_.at(start_vertex).addEdge(end_vertex, weight);
}

void chernov::Graph::cutEdge(std::string start_vertex, std::string end_vertex, size_t weight)
{
  incoming_.at(end_vertex).cutEdge(start_vertex, weight);
  outgoing_.at(start_vertex).cutEdge(end_vertex, weight);
}

chernov::Vector< std::string > chernov::Graph::getVertexes() const
{
  Vector< std::string > vertexes;
  for (auto iter = incoming_.cbegin(); iter != incoming_.cend(); ++iter) {
    vertexes.pushBack(iter->first);
  }
  for (auto iter = outgoing_.cbegin(); iter != outgoing_.cend(); ++iter) {
    bool flag = true;
    for (auto v_iter = vertexes.cbegin(); v_iter != vertexes.cend(); ++v_iter) {
      if (iter->first == *v_iter) {
        flag = false;
        break;
      }
    }
    if (flag) {
      vertexes.pushBack(iter->first);
    }
  }
  return vertexes;
}

chernov::Vector< std::pair< std::string, size_t > > chernov::Graph::getOutbound(std::string vertex) const
{
  if (!outgoing_.has(vertex) && !incoming_.has(vertex)) {
    throw std::out_of_range("vertex not found");
  }
  if (!outgoing_.has(vertex)) {
    return {};
  }
  return outgoing_.at(vertex).getEdges();
}

chernov::Vector< std::pair< std::string, size_t > > chernov::Graph::getInbound(std::string vertex) const
{
   if (!incoming_.has(vertex) && !outgoing_.has(vertex)) {
    throw std::out_of_range("vertex not found");
  }
  if (!incoming_.has(vertex)) {
    return {};
  }
  return incoming_.at(vertex).getEdges();
}

chernov::Graphs::Graphs():
  graphs_(64)
{}

void chernov::Graphs::createGraphWithoutCheckingExisting(std::string graph_name)
{
  try {
    graphs_.add(graph_name, Graph(graph_name));
  } catch (const std::length_error & e) {
    graphs_.rehash(graphs_.maxCapacity() ? graphs_.maxCapacity() * 2 : 2);
    graphs_.add(graph_name, Graph(graph_name));
  }
}

void chernov::Graphs::createGraph(std::string graph_name, std::ostream & output)
{
  if (!graphs_.has(graph_name)) {
    createGraphWithoutCheckingExisting(graph_name);
  } else {
    output << "<INVALID COMMAND>\n";
  }
}


void chernov::Graphs::addEdge(std::string graph_name, std::string start_vertex, std::string end_vertex, size_t weight)
{
  if (!graphs_.has(graph_name)) {
    createGraphWithoutCheckingExisting(graph_name);
  }
  graphs_.at(graph_name).addEdge(start_vertex, end_vertex, weight);
}

void chernov::Graphs::showGraphs(std::ostream & output)
{
  Vector< std::string > graphs;
  for (auto iter = graphs_.begin(); iter != graphs_.end(); ++iter) {
    graphs.pushBack(iter->first);
  }
  sort(graphs, Comparator< std::string >{});
  for (auto iter = graphs.cbegin(); iter != graphs.cend(); ++iter) {
    output << *iter << "\n";
  }
}

void chernov::Graphs::showGraphVertexes(std::string graph_name, std::ostream & output)
{
  try {
    Vector< std::string > vertexes = graphs_.at(graph_name).getVertexes();
    sort(vertexes, Comparator< std::string >{});
    for (auto iter = vertexes.cbegin(); iter != vertexes.cend(); ++iter) {
      output << *iter << "\n";
    }
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}

void chernov::Graphs::showGraphOutbound(std::string graph_name, std::string vertex, std::ostream & output)
{
  try {
    Vector< std::pair< std::string, size_t > > vertexes = graphs_.at(graph_name).getOutbound(vertex);
    sort(vertexes, PairComparator< std::string, size_t >{});
    for (auto iter = vertexes.cbegin(); iter != vertexes.cend(); ++iter) {
      output << iter->first << " " << iter->second << "\n";
    }
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}

void chernov::Graphs::showGraphInbound(std::string graph_name, std::string vertex, std::ostream & output)
{
  try {
    Vector< std::pair< std::string, size_t > > edges = graphs_.at(graph_name).getInbound(vertex);
    if (edges.getSize() == 0) {
      return;
    }

    sort(edges, PairComparator< std::string, size_t >{});

    auto iter = edges.cbegin();
    while (iter != edges.cend()) {
      std::string current_name = iter->first;
      output << current_name;

      while (iter != edges.cend() && iter->first == current_name) {
        output << " " << iter->second;
        ++iter;
      }
      output << "\n";
    }
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}

void chernov::Graphs::bindGraphVertexes(
  std::string graph_name, std::string vertex_a, std::string vertex_b, size_t weight, std::ostream & output)
{
  try {
    graphs_.at(graph_name).addEdge(vertex_a, vertex_b, weight);
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}

void chernov::Graphs::cutGraphEdge(std::string graph_name,
  std::string vertex_a, std::string vertex_b, size_t weight, std::ostream & output)
{
  try {
    graphs_.at(graph_name).cutEdge(vertex_a, vertex_b, weight);
  } catch (const std::out_of_range & e) {
    output << "<INVALID COMMAND>\n";
  }
}

void chernov::Graphs::mergeGraphs(std::string new_graph,
  std::string old_graph1, std::string old_graph2, std::ostream & output)
{
  if (graphs_.has(new_graph) || !graphs_.has(old_graph1) || !graphs_.has(old_graph2)) {
    output << "<INVALID COMMAND>\n";
    return;
  }

  Graph graph(new_graph);

  auto add_edges = [&graph](Graph gr) {
    for (auto iter = gr.outgoing_.cbegin(); iter != gr.outgoing_.cend(); ++iter) {
      Vector< std::pair< std::string, size_t > > edges = iter->second.getEdges();
      for (auto v_iter = edges.cbegin(); v_iter != edges.cend(); ++v_iter) {
        graph.addEdge(iter->first, v_iter->first, v_iter->second);
      }
    }
  };

  add_edges(graphs_.at(old_graph1));
  add_edges(graphs_.at(old_graph2));

  try {
    graphs_.add(new_graph, graph);
  } catch (const std::length_error & e) {
    graphs_.rehash(graphs_.maxCapacity() ? graphs_.maxCapacity() * 2 : 2);
    graphs_.add(new_graph, graph);
  }
}

void chernov::Graphs::extractGraphs(std::string new_graph,
  std::string old_graph, size_t count_k, Vector< std::string > & vertexes, std::ostream & output)
{
  if (graphs_.has(new_graph) || !graphs_.has(old_graph)) {
    output << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Graph new_gr(new_graph);
    const Graph & old_gr = graphs_.at(old_graph);

    for (size_t i = 0; i < count_k && i < vertexes.getSize(); ++i) {
      const std::string & src = vertexes[i];
      Vector< std::pair< std::string, size_t > > edges = old_gr.getOutbound(src);

      for (size_t j = 0; j < edges.getSize(); ++j) {
        const std::string & dst = edges[j].first;
        size_t weight = edges[j].second;

        bool dst_allowed = false;
        for (size_t k = 0; k < count_k && k < vertexes.getSize(); ++k) {
          if (dst == vertexes[k]) {
            dst_allowed = true;
            break;
          }
        }

        if (dst_allowed) {
          new_gr.addEdge(src, dst, weight);
        }
      }
    }

    try {
      graphs_.add(new_graph, new_gr);
    } catch (const std::length_error&) {
      graphs_.rehash(graphs_.maxCapacity() ? graphs_.maxCapacity() * 2 : 2);
      graphs_.add(new_graph, new_gr);
    }
  } catch (const std::out_of_range&) {
    output << "<INVALID COMMAND>\n";
  }
}
