#include <iostream>

#include "commands.hpp"

void chernov::cmdGraphs(std::istream &, std::ostream & output, Graphs & graphs)
{
  graphs.showGraphs(output);
}

void chernov::cmdVertexes(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name;
  input >> graph_name;
  graphs.showGraphVertexes(graph_name, output);
}

void chernov::cmdOutbound(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name, vertex;
  input >> graph_name >> vertex;
  graphs.showGraphOutbound(graph_name, vertex, output);
}

void chernov::cmdInbound(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name, vertex;
  input >> graph_name >> vertex;
  graphs.showGraphInbound(graph_name, vertex, output);
}

void chernov::cmdBind(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name, vertex_a, vertex_b;
  size_t weight;
  input >> graph_name >> vertex_a >> vertex_b >> weight;
  graphs.bindGraphVertexes(graph_name, vertex_a, vertex_b, weight, output);
}

void chernov::cmdCut(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name, vertex_a, vertex_b;
  size_t weight;
  input >> graph_name >> vertex_a >> vertex_b >> weight;
  graphs.cutGraphEdge(graph_name, vertex_a, vertex_b, weight, output);
}

void chernov::cmdCreate(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name;
  input >> graph_name;
  graphs.createGraph(graph_name, output);
}

void chernov::cmdMerge(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string new_graph, old_graph1, old_graph2;
  input >> new_graph >> old_graph1 >> old_graph2;
  graphs.mergeGraphs(new_graph, old_graph1, old_graph2, output);
}

void chernov::cmdExtract(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string new_graph, old_graph;
  size_t count_k;
  input >> new_graph >> old_graph >> count_k;

  Vector< std::string > vertexes(count_k, "");
  for (size_t i = 0; i < count_k; ++i) {
    input >> vertexes[i];
  }
  graphs.extractGraphs(new_graph, old_graph, count_k, vertexes, output);
}
