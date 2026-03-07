#ifndef LCITER_HPP
#define LCITER_HPP

#include "list.hpp"

namespace chernov {
  template< class T >
  class LCIter {
    friend class List< T >;
    const Node< T > * ptr;
  public:
    LCIter();
    LCIter(const Node< T > * node);
    bool hasNext() const noexcept;
    LCIter< T > next() const;
    const T & value() const;
    const T & operator*() const;
    const T * operator->() const;
    LCIter< T > & operator++();
    LCIter< T > operator++(int);
    bool operator==(const LCIter< T > & other) const noexcept;
    bool operator!=(const LCIter< T > & other) const noexcept;
  };
}

#endif
