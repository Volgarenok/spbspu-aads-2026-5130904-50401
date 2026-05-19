#ifndef LITER_HPP
#define LITER_HPP

namespace chernov {
  template< class T >
  class Node;

  template< class T >
  class List;

  template< class T >
  class LIter {
  public:
    LIter();
    T & operator*() const;
    T * operator->() const;
    LIter< T > & operator++();
    LIter< T > operator++(int);
    bool operator==(const LIter< T > & other) const noexcept;
    bool operator!=(const LIter< T > & other) const noexcept;
  private:
    friend class List< T >;
    Node< T > * ptr;
    Node< T > * fake_;
    LIter(Node< T > * node, Node< T > * fake);
  };

  template< class T >
  LIter< T >::LIter(Node< T > * node, Node< T > * fake):
    ptr(node),
    fake_(fake)
  {}

  template< class T >
  LIter< T >::LIter():
    ptr(nullptr),
    fake_(nullptr)
  {}

  template< class T >
  T & LIter< T >::operator*() const
  {
    return ptr->data;
  }

  template< class T >
  T * LIter< T >::operator->() const
  {
    return &(ptr->data);
  }

  template< class T >
  LIter< T > & LIter< T >::operator++()
  {
    ptr = ptr->next;
    if (ptr == fake_) {
      ptr = fake_->next;
    }
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter old = *this;
    ++(*this);
    return old;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > & other) const noexcept
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > & other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
