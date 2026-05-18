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
  if (!(input >> graph_name >> vertex_a >> vertex_b >> weight)) {
    return;
  }
  graphs.bindGraphVertexes(graph_name, vertex_a, vertex_b, weight, output);
}

void chernov::cmdCut(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name, vertex_a, vertex_b;
  size_t weight;
  if (!(input >> graph_name >> vertex_a >> vertex_b >> weight)) {
    return;
  }
  graphs.cutGraphEdge(graph_name, vertex_a, vertex_b, weight, output);
}

void chernov::cmdCreate(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string graph_name;
  input >> graph_name;

  if (graphs.hasGraph(graph_name)) {
    output << "<INVALID COMMAND>\n";
    return;
  }

  graphs.createGraph(graph_name, output);

  size_t count;
  if (!(input >> count)) {
    return;
  }

  for (size_t i = 0; i < count; ++i) {
    std::string vertex;
    input >> vertex;
    graphs.addVertex(graph_name, vertex, output);
  }
}

void chernov::cmdMerge(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string new_graph, old_graph1, old_graph2;
  if (!(input >> new_graph >> old_graph1 >> old_graph2)) {
    return;
  }
  graphs.mergeGraphs(new_graph, old_graph1, old_graph2, output);
}

void chernov::cmdExtract(std::istream & input, std::ostream & output, Graphs & graphs)
{
  std::string new_graph, old_graph;
  size_t count_k;

  if (!(input >> new_graph >> old_graph >> count_k)) {
    return;
  }

  Vector< std::string > vertexes;
  std::string s;
  for (size_t i = 0; i < count_k; ++i) {
    if (!(input >> s)) {
      return;
    }
    vertexes.pushBack(s);
  }

  graphs.extractGraphs(new_graph, old_graph, count_k, vertexes, output);
}
