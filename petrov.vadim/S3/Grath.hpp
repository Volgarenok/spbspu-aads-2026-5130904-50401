#ifndef GRATH_HPP
#define GRATH_HPP

#include <cstddef>
#include "HashTable.hpp"
#include "../common/vector/top-it-vector.hpp"
#include "HashFunc.hpp"

namespace petrov
{
  using Weight = topit::Vector< size_t >;
  using EdgeVec = std::pair< std::string, Weight >;

  struct Grath
  {
    Grath();
    Grath(const Grath& other);
    Grath(Grath&& other) noexcept;

    void addVertex(const std::string& name);
    void addEdge(const std::string& from, const std::string& to, size_t weight);
    void removeVertex(const std::string& name);
    void removeEdge(const std::string& from, const std::string& to, size_t weight);

    bool hasVertex(const std::string& name) const;
    bool hasEdge(const std::string& from, const std::string& to) const;

    topit::Vector< std::string > getVertices() const;
    topit::Vector< EdgeVec > getInputEdges(const std::string& name) const;
    topit::Vector< EdgeVec > getOutputEdges(const std::string& name) const;

    void swap(Grath& other) noexcept;
    Grath& operator=(const Grath&) = default;

    private:
      HashTable< EdgeKey, Weight, edgeSha1, Equal< EdgeKey > > edges_;
      HashTable< std::string, bool, sha1, Equal< std::string > > vertices_;

      template< class T >
      struct Less
      {
        bool operator()(const T& lhs, const T& rhs) const noexcept
        {
          return lhs < rhs;
        }
      };
  };
}

#endif
