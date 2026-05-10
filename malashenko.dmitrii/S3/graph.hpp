#ifndef GRAPH
#define GRAPH
#include "hash_func.hpp"
#include <utility>
#include <tuple>
#include <vector/vector.hpp>
#include "hash_func.hpp"
#include "hash_table.hpp"

namespace malashenko
{
  class GraphsTable;
  class Graph {
  public:
    using pair_t = std::pair< std::string, std::string >;
    using pairWeight_t = std::pair< std::string, malashenko::Vector< size_t > >;
    using htIter_t = HashTableIter< pair_t, malashenko::Vector< size_t >, malashenko::HmacHash< pair_t >, malashenko::Equal< pair_t > >;
    using htCIter_t = HashTableConstIter< pair_t, Vector< size_t >, HmacHash< pair_t >, Equal< pair_t > >;
    Vector< std::string > getTops();
    Vector< pairWeight_t > getWeightPairs(const std::string& topName, size_t pos);
    bool cutVertex(const std::string& from, const std::string& to, size_t size);

  private:
    friend class GraphsTable;
    void sort(Vector< size_t >& vec);
    HashTable< pair_t, Vector< size_t >, HmacHash< pair_t >, Equal< pair_t > > vertexes_;
  };
}

#endif
