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

    LIter() noexcept:
      current_(nullptr)
    {}

    LIter(detail::Node< T >* ptr) noexcept:
      current_(ptr)
    {}

    bool hasNext() const noexcept
    {
      return current_ != nullptr;
    }

    LIter next() const
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      return LIter(current_->next);
    }

    LIter& operator++()
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      current_ = current_->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter result(*this);
      ++(*this);
      return result;
    }

    T& operator*() const
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      return current_->data;
    }

    bool operator==(const LIter& other) const noexcept
    {
      return current_ == other.current_;
    }

    bool operator!=(const LIter& other) const noexcept
    {
      return current_ != other.current_;
    }

    T* operator->() const
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      return &(current_->data);
    }

  private:
    detail::Node< T >* current_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;

  public:
    LCIter() noexcept:
      current_(nullptr)
    {}

    LCIter(const detail::Node< T >* ptr) noexcept:
      current_(ptr)
    {}

    bool hasNext() const noexcept
    {
      return current_ != nullptr;
    }

    LCIter next() const
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      return LCIter(current_->next);
    }

    LCIter& operator++()
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      current_ = current_->next;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter result(*this);
      ++(*this);
      return result;
    }

    const T& operator*() const
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      return current_->data;
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return current_ == other.current_;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return current_ != other.current_;
    }

    const T* operator->() const
    {
      if (!current_)
      {
        throw std::runtime_error("null iterator");
      }
      return &(current_->data);
    }

  private:
    const detail::Node< T >* current_;
  };
}
#endif
