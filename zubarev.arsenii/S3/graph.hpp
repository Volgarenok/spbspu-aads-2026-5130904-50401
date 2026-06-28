#ifndef GRAPH_TABLE_HPP
#define GRAPH_TABLE_HPP

#include <iostream>
#include <string>

#include "../common/my_hashfunc/my_equal.hpp"
#include "my_hashtable.hpp"
#include "../common/my_hashfunc/my_siphash.hpp"


#include "../common/my_vector/top-it-vector.hpp"
namespace zubarev
{
  class GraphTable
  {

  private:
    using EdgeKey = std::pair< std::string, std::string >;
    using Weights = Vector< size_t >;

    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, Equaler< EdgeKey > >;
    using GraphEdgeTable = HashTable< std::string, EdgeTable, SipHash, Equaler< std::string > >;

    GraphEdgeTable edge_data_;

    using VertexList = Vector< std::string >;
    using GraphVertexTable = HashTable< std::string, VertexList, SipHash, Equaler< std::string > >;

    GraphVertexTable vertex_data_;

    static bool compare(const std::pair< std::string, Vector< size_t > >& a,
                        const std::pair< std::string, Vector< size_t > >& b)
    {
      return a.first < b.first;
    }

  public:
    void graphs(std::ostream&) const;
    void vertexes(const std::string&, std::ostream&) const;
    void outbound(const std::string&, const std::string&, std::ostream&) const;
    void inbound(const std::string&, const std::string&, std::ostream&) const;
    void bind(const std::string&, const std::pair< std::string, std::string >&, size_t, std::ostream&);
    void cut(const std::string&, const std::pair< std::string, std::string >&, size_t, std::ostream&);
    bool create(const std::string&, size_t, const Vector< std::string >&, std::ostream&);
    void merge(const std::string&, const std::string&, const std::string&, std::ostream&);
    void extract(const std::string&, const std::string&, size_t, const Vector< std::string >&, std::ostream&);
  };

  template < class T, class Compare >
  void sort(VectIter< T > beg, VectIter< T > end, Compare comp)
  {
    if (beg == end) {
      return;
    }
    while (true) {
      VectIter< T > last_swap = beg;
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
  template < class T, class Equal >
  zubarev::VectIter< T > unique(zubarev::VectIter< T > begin, zubarev::VectIter< T > end, Equal eq)
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
