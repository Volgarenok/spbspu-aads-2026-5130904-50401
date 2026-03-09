#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <limits>

namespace vasyakin
{
  template< class T > class Node;
  template< class T > class List;
  const int MAX = std::numeric_limits< int >::max();

  template< class T >
  class LIter
  {
  private:
    friend class List< T >;
    Node< T >* ptr;
    explicit LIter(Node< T >* p);

  public:
    T& operator*() const;
    T* operator->() const;
    LIter& operator++();
    LIter operator++(int);
    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;
    Node< T >* get_ptr() const;
  };
}

#endif