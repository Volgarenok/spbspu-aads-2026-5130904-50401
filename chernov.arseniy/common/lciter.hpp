#ifndef LCITER_HPP
#define LCITER_HPP

#include <memory>

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
    const detail::Node< T > * ptr_;
    const detail::Node< T > * fake_;
    LCIter(const detail::Node< T > * node, const detail::Node< T > * fake);
  };

  template< class T >
  chernov::LCIter< T >::LCIter(const detail::Node< T > * node, const detail::Node< T > * fake):
    ptr_(node),
    fake_(fake)
  {}

  template< class T >
  chernov::LCIter< T >::LCIter() noexcept:
    ptr_(nullptr),
    fake_(nullptr)
  {}

  template< class T >
  const T & chernov::LCIter< T >::operator*() const noexcept
  {
    return ptr_->data;
  }

  template< class T >
  const T * chernov::LCIter< T >::operator->() const noexcept
  {
    return std::addressof(ptr_->data);
  }

  template< class T >
  chernov::LCIter< T > & chernov::LCIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    if (ptr_ == fake_) {
      ptr_ = fake_->next;
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
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool chernov::LCIter< T >::operator!=(const LCIter< T > & other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
