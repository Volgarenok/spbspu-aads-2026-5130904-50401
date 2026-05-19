#ifndef LCITER_HPP
#define LCITER_HPP

namespace chernov {
  namespace detail {
    template< class T >
    class Node;
  }

  template< class T >
  class List;

  template< class T >
  class LCIter {
  public:
    LCIter() noexcept;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
    LCIter< T > & operator++() noexcept;
    LCIter< T > operator++(int) noexcept;
    bool operator==(const LCIter< T > & other) const noexcept;
    bool operator!=(const LCIter< T > & other) const noexcept;
  private:
    friend class List< T >;
    const detail::Node< T > * ptr;
    const detail::Node< T > * fake_;
    LCIter(const detail::Node< T > * node, const detail::Node< T > * fake);
  };

  template< class T >
  chernov::LCIter< T >::LCIter(const detail::Node< T > * node, const detail::Node< T > * fake):
    ptr(node),
    fake_(fake)
  {}

  template< class T >
  chernov::LCIter< T >::LCIter() noexcept:
    ptr(nullptr),
    fake_(nullptr)
  {}

  template< class T >
  const T & chernov::LCIter< T >::operator*() const noexcept
  {
    return ptr->data;
  }

  template< class T >
  const T * chernov::LCIter< T >::operator->() const noexcept
  {
    return &(ptr->data);
  }

  template< class T >
  chernov::LCIter< T > & chernov::LCIter< T >::operator++() noexcept
  {
    ptr = ptr->next;
    if (ptr == fake_) {
      ptr = fake_->next;
    }
    return *this;
  }

  template< class T >
  chernov::LCIter< T > chernov::LCIter< T >::operator++(int) noexcept
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
