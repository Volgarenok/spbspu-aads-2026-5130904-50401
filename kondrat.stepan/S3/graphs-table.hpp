#ifndef GRAPHS_TABLE_HPP
#define GRAPHS_TABLE_HPP

#include <iosfwd>
#include <string>
#include "graph.hpp"

namespace kondrat
{
  using GraphStorage = HashTable< std::string, Graph, blake2 >;

  struct GraphsTable
  {
    void readFile(std::istream & in);
    bool graphs(std::istream & in, std::ostream & out, const std::string & graphName);
    bool vertexes(std::istream & in, std::ostream & out, const std::string & graphName);
    bool outbound(std::istream & in, std::ostream & out, const std::string & graphName);
    bool inbound(std::istream & in, std::ostream & out, const std::string & graphName);
    bool bind(std::istream & in, std::ostream & out, const std::string & graphName);
    bool cut(std::istream & in, std::ostream & out, const std::string & graphName);
    bool create(std::istream & in, std::ostream & out, const std::string & graphName);
    bool merge(std::istream & in, std::ostream & out, const std::string & graphName);
    bool extract(std::istream & in, std::ostream & out, const std::string & graphName);

  private:
    GraphStorage graphs_;
  };
}

#endif
