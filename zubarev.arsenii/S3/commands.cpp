#include "commands.hpp"

#include <iostream>

#include "../common/top-it-vector.hpp"
#include "graph.hpp"

void zubarev::cmd_graphs(std::istream&, std::ostream& out, GraphTable& graph)
{
  graph.graphs(out);
}
void zubarev::cmd_vertexes(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name;
  in >> name;
  graph.vertexes(out, name);
  out << '\n';
}
void zubarev::cmd_outbound(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name, vertex;
  in >> name >> vertex;
  graph.outbound(out, name, vertex);
}
void zubarev::cmd_inbound(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name, vertex;
  in >> name >> vertex;
  graph.inbound(out, name, vertex);
}
void zubarev::cmd_bind(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name;
  std::pair< std::string, std::string > vertexes;
  size_t weight;
  in >> name >> vertexes.first >> vertexes.second >> weight;
  graph.bind(out, name, vertexes, weight);
}
void zubarev::cmd_cut(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name;
  std::pair< std::string, std::string > vertexes;
  size_t weight;
  in >> name >> vertexes.first >> vertexes.second >> weight;
  graph.cut(out, name, vertexes, weight);
}
void zubarev::cmd_create(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name;
  in >> name;

  size_t count = 0;

  if (!(in >> count)) {
    in.clear();
    graph.create(out, name, 0, {});
    return;
  }

  zubarev::Vector< std::string > vertexes;

  for (size_t i = 0; i < count; ++i) {
    std::string str;
    in >> str;
    vertexes.pushBack(str);
  }

  graph.create(out, name, count, vertexes);
}
void zubarev::cmd_merge(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name1, name2, name3;
  in >> name1 >> name2 >> name3;
  graph.merge(out, name1, name2, name3);
}
void zubarev::cmd_extract(std::istream& in, std::ostream& out, GraphTable& graph)
{
  std::string name1, name2;
  size_t count;
  zubarev::Vector< std::string > vertexes;
  in >> name1 >> name2 >> count;
  for (size_t i = 0; i < count; ++i) {
    std::string str;
    in >> str;
    vertexes.pushBack(str);
  }
  graph.extract(out, name1, name2, count, vertexes);
}
