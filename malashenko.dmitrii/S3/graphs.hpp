#ifndef GRAPHS
#define GRAPHS
#include <vector/top-it-vector.hpp>
#include "hash_table.hpp"
#include "hash_func.hpp"
#include <iostream>

namespace malashenko
{
  class Graph {
  public:
    using pair_t = std::pair< std::string, std::string >;
    using topPair_t = std::pair< std::string, size_t >;
    using vertexes_t = HashTable< pair_t, size_t, HmacHash< pair_t >, Equal< pair_t > >;
    using htIter_t = HashTableIter< std::string,  vertexes_t, HmacHash< std::string >, Equal< std::string > >;
    using VertIter_t = HashTableIter< pair_t, size_t, HmacHash< pair_t >, Equal< pair_t > >;

    void graphs(std::ostream& out);
    void vertexes(std::istream& in, std::ostream& out, std::string graphName);
    void outbound(std::istream& in, std::ostream& out, std::string graphName);
    void inbound (std::istream& in, std::ostream& out, std::string graphName);
    void bind(std::istream& in, std::ostream& out, std::string graphName);
    void cut(std::istream& in, std::ostream& out, std::string graphName);
    void create(std::istream& in, std::ostream& out, std::string graphName);
    void merge(std::istream& in, std::ostream& out, std::string graphName);
    void extract(std::istream& in, std::ostream& out, std::string graphName);
  private:
    void sortStr(topit::Vector< std::string >& vec);
    void sortPair(topit::Vector< topPair_t >& vec);
    topit::Vector< std::string > rmRepeatSortedStr(const topit::Vector< std::string >& vec);
    topit::Vector< topPair_t > rmRepeatSortedPair(const topit::Vector< topPair_t >& vec);
    HashTable< std::string, vertexes_t, HmacHash< std::string >, Equal< std::string > > graphs_;
  };
}
#endif