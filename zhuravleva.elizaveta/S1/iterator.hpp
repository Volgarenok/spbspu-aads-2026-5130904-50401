#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <stdexcept>
#include "node.hpp"

namespace zhuravleva
{
  template< class T >
  class List;

  template< class T >
  class LIter
  {
    friend class List< T >;

  public:
    LIter() noexcept;
    explicit LIter(detail::Node< T > * ptr) noexcept;
    bool hasNext() const noexcept;
    LIter next() const;
    LIter & operator++();
    LIter operator++(int);
    T & operator*();
    const T & operator*() const;
    T * operator->();
    const T * operator->() const;
    bool operator==(const LIter & other) const noexcept;
    bool operator!=(const LIter & other) const noexcept;

  private:
    detail::Node< T > * current_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;

  public:
    LCIter() noexcept;
    explicit LCIter(const detail::Node< T > * ptr) noexcept;
    bool hasNext() const noexcept;
    LCIter next() const;
    LCIter & operator++();
    LCIter operator++(int);
    const T & operator*() const;
    const T * operator->() const;
    bool operator==(const LCIter & other) const noexcept;
    bool operator!=(const LCIter & other) const noexcept;

  private:
    const detail::Node< T > * current_;
  };

  template< class T >
  LIter< T >::LIter() noexcept:
    current_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(detail::Node< T > * ptr) noexcept:
    current_(ptr)
  {}

  template< class T >
  bool LIter< T >::hasNext() const noexcept
  {
    return current_ != nullptr;
  }

  template< class T >
  LIter< T > LIter< T >::next() const
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    return LIter< T >(current_->next);
  }

  template< class T >
  LIter< T > & LIter< T >::operator++()
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    current_ = current_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > temp(*this);
    ++(*this);
    return temp;
  }

  template< class T >
  T & LIter< T >::operator*()
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    return current_->data;
  }

  template< class T >
  const T & LIter< T >::operator*() const
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    return current_->data;
  }

  template< class T >
  T * LIter< T >::operator->()
  {
    return &(**this);
  }

  template< class T >
  const T * LIter< T >::operator->() const
  {
    return &(**this);
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > & other) const noexcept
  {
    return current_ == other.current_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > & other) const noexcept
  {
    return !(*this == other);
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    current_(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T > * ptr) noexcept:
    current_(ptr)
  {}

  template< class T >
  bool LCIter< T >::hasNext() const noexcept
  {
    return current_ != nullptr;
  }

  template< class T >
  LCIter< T > LCIter< T >::next() const
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    return LCIter< T >(current_->next);
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator++()
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    current_ = current_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > temp(*this);
    ++(*this);
    return temp;
  }

  template< class T >
  const T & LCIter< T >::operator*() const
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    return current_->data;
  }

  template< class T >
  const T * LCIter< T >::operator->() const
  {
    return &(**this);
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > & other) const noexcept
  {
    return current_ == other.current_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > & other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
