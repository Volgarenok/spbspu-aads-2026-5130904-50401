#include "commands.hpp"
#include <iostream>

void levkin::cmdGraphs(std::istream&, std::ostream& output, DB& graphs)
{
  graphs.showGraphs(output);
}

void levkin::cmdVertexes(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string graph_name;
  if (!(input >> graph_name)) {
    input.setstate(std::ios::failbit);
    return;
  }
  graphs.showGraphVertexes(graph_name, output);
}

void levkin::cmdOutbound(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string graph_name, vertex;
  if (!(input >> graph_name >> vertex)) {
    input.setstate(std::ios::failbit);
    return;
  }
  graphs.showGraphOutbound(graph_name, vertex, output);
}

void levkin::cmdInbound(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string graph_name, vertex;
  if (!(input >> graph_name >> vertex)) {
    input.setstate(std::ios::failbit);
    return;
  }
  graphs.showGraphInbound(graph_name, vertex, output);
}

void levkin::cmdBind(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string graph_name, vertex_a, vertex_b;
  size_t weight;
  if (!(input >> graph_name >> vertex_a >> vertex_b >> weight)) {
    input.setstate(std::ios::failbit);
    return;
  }
  graphs.bindGraphVertexes(graph_name, vertex_a, vertex_b, weight, output);
}

void levkin::cmdCut(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string graph_name, vertex_a, vertex_b;
  size_t weight;
  if (!(input >> graph_name >> vertex_a >> vertex_b >> weight)) {
    input.setstate(std::ios::failbit);
    return;
  }
  graphs.cutGraphEdge(graph_name, vertex_a, vertex_b, weight, output);
}

void levkin::cmdCreate(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string graph_name;
  if (!(input >> graph_name)) {
    input.setstate(std::ios::failbit);
    return;
  }

  if (graphs.hasGraph(graph_name)) {
    input.setstate(std::ios::failbit);
    return;
  }

  graphs.createGraphUnsafe(graph_name);

  size_t count;
  if (!(input >> count)) {
    input.setstate(std::ios::failbit);
    return;
  }

  std::string vertex;
  for (size_t i = 0; i < count; ++i) {
    if (!(input >> vertex)) {
      input.setstate(std::ios::failbit);
      return;
    }
    graphs.addVertex(graph_name, vertex, output);
  }
}

void levkin::cmdMerge(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string new_graph, old_graph1, old_graph2;
  if (!(input >> new_graph >> old_graph1 >> old_graph2)) {
    input.setstate(std::ios::failbit);
    return;
  }
  graphs.mergeGraphs(new_graph, old_graph1, old_graph2, output);
}

void levkin::cmdExtract(std::istream& input, std::ostream& output, DB& graphs)
{
  std::string new_graph, old_graph;
  size_t count_k;

  if (!(input >> new_graph >> old_graph >> count_k)) {
    input.setstate(std::ios::failbit);
    return;
  }

  stuff::Vector< std::string > vertexes;
  std::string s;
  for (size_t i = 0; i < count_k; ++i) {
    if (!(input >> s)) {
      input.setstate(std::ios::failbit);
      return;
    }
    vertexes.pushBack(s);
  }

  graphs.extractGraphs(new_graph, old_graph, count_k, vertexes, output);
}
