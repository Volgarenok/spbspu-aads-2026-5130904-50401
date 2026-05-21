#ifndef GRAPH_TABLE_HPP
#define GRAPH_TABLE_HPP

#include "my_equal.hpp"
#include "my_hashtable.hpp"
#include "my_siphash.hpp"
#include <iostream>
#include <string>

#include "../common/top-it-vector.hpp"
namespace zubarev
{
  class GraphTable
  {

  private:
    using EdgeKey = std::pair< std::string, std::string >;
    using Weights = topit::Vector< size_t >;

    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, Equaler< EdgeKey > >;
    using GraphEdgeTable = HashTable< std::string, EdgeTable, SipHash, Equaler< std::string > >;

    GraphEdgeTable edge_data_;

    using VertexList = topit::Vector< std::string >;
    using GraphVertexTable = HashTable< std::string, VertexList, SipHash, Equaler< std::string > >;

    GraphVertexTable vertex_data_;

    static bool compare(const std::pair< std::string, topit::Vector< size_t > >& a,
                        const std::pair< std::string, topit::Vector< size_t > >& b)
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
    bool create(const std::string&, size_t, const topit::Vector< std::string >&, std::ostream&);
    void merge(const std::string&, const std::string&, const std::string&, std::ostream&);
    void extract(const std::string&, const std::string&, size_t, const topit::Vector< std::string >&, std::ostream&);
  };

  template < class T, class Compare >
  void sort(topit::VectIter< T > beg, topit::VectIter< T > end, Compare comp)
  {
    if (beg == end) {
      return;
    }
    while (true) {
      topit::VectIter< T > last_swap = beg;
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
  topit::VectIter< T > unique(topit::VectIter< T > begin, topit::VectIter< T > end, Equal eq)
  {
    if (begin == end) {
      return end;
    }

    topit::VectIter< T > result = begin;
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
