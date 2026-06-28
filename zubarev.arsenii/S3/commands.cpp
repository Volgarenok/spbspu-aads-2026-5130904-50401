#include "commands.hpp"
#include "../common/my_vector/top-it-vector.hpp"
#include "graph.hpp"
#include <iostream>

namespace zubarev
{
  void cmd_graphs(std::istream&, std::ostream& out, GraphTable& graph)
  {
    graph.graphs(out);
  }
  void cmd_vertexes(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name;
    in >> name;
    graph.vertexes(name, out);
  }
  void cmd_outbound(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name, vertex;
    in >> name >> vertex;
    graph.outbound(name, vertex, out);
  }
  void cmd_inbound(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name, vertex;
    in >> name >> vertex;
    graph.inbound(name, vertex, out);
  }
  void cmd_bind(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name;
    std::pair< std::string, std::string > vertexes;
    size_t weight;
    in >> name >> vertexes.first >> vertexes.second >> weight;
    graph.bind(name, vertexes, weight, out);
  }
  void cmd_cut(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name;
    std::pair< std::string, std::string > vertexes;
    size_t weight;
    in >> name >> vertexes.first >> vertexes.second >> weight;
    graph.cut(name, vertexes, weight, out);
  }
  void cmd_create(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name;
    in >> name;

    size_t count = 0;

    if (!(in >> count)) {
      in.clear();
      graph.create(name, 0, {}, out);
      return;
    }

    Vector< std::string > vertexes;

    for (size_t i = 0; i < count; ++i) {
      std::string str;
      in >> str;
      vertexes.pushBack(str);
    }

    graph.create(name, count, vertexes, out);
  }
  void cmd_merge(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name1, name2, name3;
    in >> name1 >> name2 >> name3;
    graph.merge(name1, name2, name3, out);
  }
  void cmd_extract(std::istream& in, std::ostream& out, GraphTable& graph)
  {
    std::string name1, name2;
    size_t count;
    Vector< std::string > vertexes;
    in >> name1 >> name2 >> count;
    for (size_t i = 0; i < count; ++i) {
      std::string str;
      in >> str;
      vertexes.pushBack(str);
    }
    graph.extract(name1, name2, count, vertexes, out);
  }

}
