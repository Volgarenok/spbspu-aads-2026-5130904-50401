#ifndef LITER_HPP
#define LITER_HPP

namespace chernov {
  namespace detail {
    template< class T >
    class Node;
  }

  template< class T >
  class List;

  template< class T >
  class LIter {
  public:
    LIter() noexcept;
    T & operator*() const noexcept;
    T * operator->() const noexcept;
    LIter< T > & operator++() noexcept;
    LIter< T > operator++(int) noexcept;
    bool operator==(const LIter< T > & other) const noexcept;
    bool operator!=(const LIter< T > & other) const noexcept;
  private:
    friend class List< T >;
    detail::Node< T > * ptr;
    detail::Node< T > * fake_;
    LIter(detail::Node< T > * node, detail::Node< T > * fake);
  };

  template< class T >
  LIter< T >::LIter(detail::Node< T > * node, detail::Node< T > * fake):
    ptr(node),
    fake_(fake)
  {}

  template< class T >
  LIter< T >::LIter() noexcept:
    ptr(nullptr),
    fake_(nullptr)
  {}

  template< class T >
  T & LIter< T >::operator*() const noexcept
  {
    return ptr->data;
  }

  template< class T >
  T * LIter< T >::operator->() const noexcept
  {
    return &(ptr->data);
  }

  template< class T >
  LIter< T > & LIter< T >::operator++() noexcept
  {
    ptr = ptr->next;
    if (ptr == fake_) {
      ptr = fake_->next;
    }
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
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
