#ifndef LCITER_HPP
#define LCITER_HPP

namespace chernov {
  template< class T >
  class Node;

  template< class T >
  class List;
  
  template< class T >
  class LCIter {
    friend class List< T >;
    const Node< T > * ptr;
    const Node< T > * fake_;
    LCIter(const Node< T > * node, const Node< T > * fake);
  public:
    LCIter();
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

  template< class T >
  chernov::LCIter< T >::LCIter(const Node< T > * node, const Node< T > * fake):
    ptr(node),
    fake_(fake)
  {}

  template< class T >
  chernov::LCIter< T >::LCIter():
    ptr(nullptr),
    fake_(nullptr)
  {}

  template< class T >
  const T & chernov::LCIter< T >::operator*() const
  {
    return ptr->data;
  }

  template< class T >
  const T * chernov::LCIter< T >::operator->() const
  {
    return &(ptr->data);
  }

  template< class T >
  chernov::LCIter< T > & chernov::LCIter< T >::operator++()
  {
    ptr = ptr->next;
    if (ptr == fake_) {
      ptr = fake_->next;
    }
    return *this;
  }

  template< class T >
  chernov::LCIter< T > chernov::LCIter< T >::operator++(int)
  {
    LCIter old = *this;
    ++(*this);
    return old;
  }

  template< class T >
  bool chernov::LCIter< T >::operator==(const LCIter< T > & other) const noexcept
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool chernov::LCIter< T >::operator!=(const LCIter< T > & other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
