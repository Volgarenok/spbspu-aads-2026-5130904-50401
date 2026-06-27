#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

#include <vector.hpp>

#include "hash_table.hpp"
#include "graph.hpp"
#include "siphash.hpp"

namespace burukov
{
  using GraphDatabase = HashTable< std::string, Graph, SipHash< std::string >, std::equal_to< std::string > >;

  namespace helpers
  {
    void sortStrings(Vector< std::string > &strings);
    void sortWeights(Vector< size_t > &weights);
  }

  void commandGraphs(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandVertexes(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandOutbound(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandInbound(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandBind(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandCut(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandCreate(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandMerge(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
  void commandExtract(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database);
}

#endif
