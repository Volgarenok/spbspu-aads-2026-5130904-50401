#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "list.hpp"
#include "node.hpp"
#include <memory>

namespace donkeev
{
  template< class T > class List;

  template< class T >
  class LIter
  {
    friend class List< T >;
    Node< T >* n;

  public:
    LIter();
    LIter(const LIter&);

    LIter< T >& operator++() noexcept;
    LIter< T > operator++(int) noexcept;

    T& operator*() const noexcept;
    T* operator ->() const noexcept;

    bool operator==(const LIter< T >&) const noexcept;
    bool operator!=(const LIter< T >&) const noexcept;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    const Node< T >* n;

  public:
    LCIter();
    LCIter(const LCIter&);

    LCIter< T >& operator++() noexcept;
    LCIter< T > operator++(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator ->() const noexcept;

    bool operator==(const LCIter< T >&) const noexcept;
    bool operator!=(const LCIter< T >&) const noexcept;
  };

  template< class T >
  LIter< T >::LIter():
    n(nullptr)
  {}
  template< class T >
  LIter< T >::LIter(const LIter< T >& yaLIter):
    n(yaLIter.n)
  {}

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    n = n->next;
    return *this;
  }
  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T > tmp = *this;
    n = n->next;
    return tmp;
  }

  template< class T >
  T& LIter< T >::operator*() const noexcept
  {
    assert(n != nullptr);
    return n->val;
  }
  template< class T >
  T* LIter< T >::operator->() const noexcept
  {
    assert(n != nullptr);
    return std::addressof(n->val);
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& yaLIter) const noexcept
  {
    return n == yaLIter.n;
  }
  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& yaLIter) const noexcept
  {
    return !(*this == yaLIter);
  }
//-----------------------------------
  template< class T >
  LCIter< T >::LCIter():
    n(nullptr)
  {}
  template< class T >
  LCIter< T >::LCIter(const LCIter< T >& yaLCIter):
    n(yaLCIter.n)
  {}

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    n = n->next;
    return *this;
  }
  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T > tmp = *this;
    n = n->next;
    return tmp;
  }

  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    assert(n != nullptr);
    return n->val;
  }
  template< class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    assert(n != nullptr);
    return std::addressof(n->val);
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& yaLCIter) const noexcept
  {
    return n == yaLCIter.n;
  }
  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& yaLCIter) const noexcept
  {
    return !(*this == yaLCIter);
  }
}
#endif