#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <string>
#include <functional>

#include "my_hashtable.hpp"
#include "my_siphash.hpp"
#include "../common/top-it-vector.hpp"
namespace zubarev
{
  class GraphTable
  {

  private:
    using EdgeKey = std::pair< std::string, std::string >;
    using Weights = zubarev::Vector< size_t >;

    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, std::equal_to< EdgeKey > >;
    using GraphEdgeTable = HashTable< std::string, EdgeTable, SipHash, std::equal_to< std::string > >;

    using VertexList = zubarev::Vector< std::string >;
    using GraphVertexTable = HashTable< std::string, VertexList, SipHash, std::equal_to< std::string > >;

    GraphEdgeTable edge_data_;
    GraphVertexTable vertex_data_;

    static bool compare(const std::pair< std::string, zubarev::Vector< size_t > >& a,
                        const std::pair< std::string, zubarev::Vector< size_t > >& b);

  public:
    void graphs(std::ostream&) const;
    void vertexes(std::ostream&, const std::string&) const;
    void outbound(std::ostream&, const std::string&, const std::string&) const;
    void inbound(std::ostream&, const std::string&, const std::string&) const;

    void bind(std::ostream&, const std::string&, const std::pair< std::string, std::string >&, size_t);

    void cut(std::ostream&, const std::string&, const std::pair< std::string, std::string >&, size_t);

    bool create(std::ostream&, const std::string&, size_t, const zubarev::Vector< std::string >&);

    void merge(std::ostream&, const std::string&, const std::string&, const std::string&);

    void extract(std::ostream&, const std::string&, const std::string&, size_t, const zubarev::Vector< std::string >&);
  };

  template< class T, class Compare >
  void sort(zubarev::VectIter< T > beg, zubarev::VectIter< T > end, Compare comp)
  {
    if (beg == end) {
      return;
    }
    while (true) {
      zubarev::VectIter< T > last_swap = beg;
      for (auto it = beg; it != end; ++it) {
        auto next = it;
        ++next;
        if (next == end) {
          break;
        }

        if (comp(*next, *it)) {
          auto tmp = *it;
          *it = *next;
          *next = tmp;
          last_swap = next;
        }
      }
      if (last_swap == beg) {
        break;
      }

      end = last_swap;
    }
  }
  template< class T, class Equal >
  zubarev::VectIter< T > unique(zubarev::VectIter< T > begin, zubarev::VectIter< T > end, std::equal_to< T > eq)
  {
    if (begin == end) {
      return end;
    }

    zubarev::VectIter< T > result = begin;
    for (auto it = begin; it != end; ++it) {
      if (result == begin || !eq(*it, *(result - 1))) {
        if (result != it) {
          *result = *it;
        }
        ++result;
      }
    }
    return result;
  }
}

#endif
