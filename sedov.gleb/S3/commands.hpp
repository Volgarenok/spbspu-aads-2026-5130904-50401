#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "hash_table.hpp"
#include "graph.hpp"
#include "siphash.hpp"
#include <iostream>
#include <string>

namespace sedov
{
  using graphSet = HashTable< std::string, sedov::Graph, sedov::Hasher< std::string >, std::equal_to< std::string > >;

  void graphs(std::istream & in, std::ostream & out, graphSet & graphs);
  void vertexes(std::istream & in, std::ostream & out, graphSet & graphs);
  void outbound(std::istream & in, std::ostream & out, graphSet & graphs);
  void inbound(std::istream & in, std::ostream & out, graphSet & graphs);
  void bind(std::istream & in, std::ostream & out, graphSet & graphs);
  void cut(std::istream & in, std::ostream & out, graphSet & graphs);
  void create(std::istream & in, std::ostream & out, graphSet & graphs);
  void merge(std::istream & in, std::ostream & out, graphSet & graphs);
  void extract(std::istream & in, std::ostream & out, graphSet & graphs);
}

#endif
