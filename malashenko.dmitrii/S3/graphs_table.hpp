#ifndef GRAPHS_TABLE
#define GRAPHS_TABLE
#include <vector/vector.hpp>
#include "hash_table.hpp"
#include "hash_func.hpp"
#include <iostream>
#include "graph.hpp"
namespace malashenko
{
  class GraphsTable {
  public:
    using pair_t = std::pair< std::string, malashenko::Vector< size_t > >;
    using htIter_t = HashTableIter< std::string, malashenko::Graph, malashenko::HmacHash< std::string >, malashenko::Equal< std::string > >;
    void graphs(std::istream& in, std::ostream& out, std::string graphName);
    void vertexes(std::istream& in, std::ostream& out, std::string graphName);
    void outbound(std::istream& in, std::ostream& out, std::string graphName);
    void inbound (std::istream& in, std::ostream& out, std::string graphName);
    void bind(std::istream& in, std::ostream& out, std::string graphName);
    void cut(std::istream& in, std::ostream& out, std::string graphName);
    void create(std::istream& in, std::ostream& out, std::string graphName);
    void merge(std::istream& in, std::ostream& out, std::string graphName);
    void extract(std::istream& in, std::ostream& out, std::string graphName);
    void readFile(std::istream& in);
  private:
    void bounds(std::istream& in, std::ostream& out, std::string graphName, size_t pos);
    void sortStrs(Vector< std::string >& vec);
    Vector< std::string > rmRepeatStrs(const Vector< std::string >& vec);
    void sortPair(Vector< pair_t >& vec);
    HashTable< std::string, Graph, HmacHash< std::string >, Equal< std::string > > graphs_;
  };
}
#endif
