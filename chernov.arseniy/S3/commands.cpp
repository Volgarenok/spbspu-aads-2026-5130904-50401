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

