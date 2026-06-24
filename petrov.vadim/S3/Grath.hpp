#ifndef GRATH_HPP
#define GRATH_HPP

#include <cstddef>
#include <functional>
#include "HashTable.hpp"
#include "vector/top-it-vector.hpp"
#include "HashFunc.hpp"

namespace petrov
{
  using Weight = Vector< size_t >;
  using EdgeVec = std::pair< std::string, Weight >;
  namespace detail
  {
    template< class T, class Comp >
    void sort(petrov::Vector< T >& vec, Comp comp);

    struct CompareEdgeVec
    {
      bool operator()(const EdgeVec& a, const EdgeVec& b) const
      {
        return a.first < b.first;
      }
    };
  }


  struct Grath
  {
    Grath() = default;
    Grath(const Grath& other) = default;
    Grath(Grath&& other) noexcept = default;

    Grath& operator=(const Grath&) = default;

    void addVertex(const std::string& name);
    void addEdge(const std::string& from, const std::string& to, size_t weight);
    void removeVertex(const std::string& name);
    void removeEdge(const std::string& from, const std::string& to, size_t weight);

    bool hasVertex(const std::string& name) const;
    bool hasEdge(const std::string& from, const std::string& to) const;

    Vector< std::string > getVertices() const;
    Vector< EdgeVec > getInputEdges(const std::string& name) const;
    Vector< EdgeVec > getOutputEdges(const std::string& name) const;

    void swap(Grath& other) noexcept;

    private:
      HashTable< EdgeKey, Weight, edgeSha1, std::equal_to< EdgeKey > > edges_;
      HashTable< std::string, bool, sha1, std::equal_to< std::string > > vertices_;
  };
}

#endif
