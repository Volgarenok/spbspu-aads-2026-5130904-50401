#ifndef GRAPHS_TABLE
#define GRAPHS_TABLE
#include <iostream>
#include <vector/vector.hpp>
#include "hash_table.hpp"
#include "hash_func.hpp"
#include "graph.hpp"
namespace malashenko
{
  class GraphsTable {
  public:
    using pair_t = std::pair< std::string, Vector< size_t > >;
    using htIter_t = HashTableIter< std::string, Graph, HmacHash< std::string >, Equal< std::string > >;
    void graphs(std::istream& in, std::ostream& out, std::string graphName);
    void vertexes(std::istream& in, std::ostream& out, std::string graphName);
    void outbound(std::istream& in, std::ostream& out, std::string graphName);
    void inbound (std::istream& in, std::ostream& out, std::string graphName);
    std::istream& bind(std::istream& in, std::ostream& out, std::string graphName);
    std::istream& cut(std::istream& in, std::ostream& out, std::string graphName);
    std::istream& create(std::istream& in, std::ostream& out, std::string graphName);
    std::istream& merge(std::istream& in, std::ostream& out, std::string graphName);
    std::istream& extract(std::istream& in, std::ostream& out, std::string graphName);
    std::istream& readFile(std::istream& in);
  private:
    HashTable< std::string, Graph, HmacHash< std::string >, Equal< std::string > > graphs_;

    Vector< std::string > rmRepeatStrs(const Vector< std::string >& vec);
    std::istream& bounds(std::istream& in, std::ostream& out, std::string graphName, size_t pos);
    void sortStrs(Vector< std::string >& vec);
    void sortPair(Vector< pair_t >& vec);
    void copyGraph(Graph& newGraph, Graph& graph);
  };
}
#endif
