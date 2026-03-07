#ifndef LITER_HPP
#define LITER_HPP

#include "list.hpp"

namespace chernov {
  template< class T >
  class LIter {
    friend class List< T >;
    Node< T > * ptr;
  public:
    LIter();
    LIter(Node< T > * node);
    bool hasNext() const noexcept;
    LIter< T > next() const;
    T & value() const;
    T & operator*() const;
    T * operator->() const;
    LIter< T > & operator++();
    LIter< T > operator++(int);
    bool operator==(const LIter< T > & other) const noexcept;
    bool operator!=(const LIter< T > & other) const noexcept;
  };
}

#endif
