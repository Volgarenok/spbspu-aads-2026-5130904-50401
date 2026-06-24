#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "hash_table.hpp"
#include "graph.hpp"
#include "siphash.hpp"
#include <iostream>
#include <string>

namespace burukov
{
  using GraphDatabase = HashTable<std::string, Graph, SipHash<std::string>, std::equal_to<std::string>>;

  void commandGraphs(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandVertexes(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandOutbound(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandInbound(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandBind(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandCut(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandCreate(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandMerge(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
  void commandExtract(std::istream &input_stream, std::ostream &output_stream, GraphDatabase &database);
}

#endif
