#ifndef VECTOR_RANDOM_ITER_HPP
#define VECTOR_RANDOM_ITER_HPP

#include <cstddef>
#include <stdexcept>

namespace khasnulin
{

  template < class T > class Vector;

  template < class T > struct RAIter
  {
    RAIter();

    RAIter< T > &operator++() noexcept;
    RAIter< T > operator++(int n) noexcept;
    RAIter< T > operator+(int n) const noexcept;
    RAIter< T > &operator+=(int n) noexcept;

    RAIter< T > &operator--() noexcept;
    RAIter< T > operator--(int n) noexcept;
    RAIter< T > operator-(int n) const noexcept;
    RAIter< T > &operator-=(int n) noexcept;
    long long operator-(RAIter< T > it) const noexcept;

    T &operator[](long long n) const;
    T &operator*() const;
    T *operator->() const;

    bool operator==(const RAIter< T > &rhs) const noexcept;
    bool operator!=(const RAIter< T > &rhs) const noexcept;
    bool operator>(const RAIter< T > &rhs) const noexcept;
    bool operator<(const RAIter< T > &rhs) const noexcept;
    bool operator>=(const RAIter< T > &rhs) const noexcept;
    bool operator<=(const RAIter< T > &rhs) const noexcept;

    friend class Vector< T >;

  private:
    RAIter(Vector< T > *vec, size_t i) noexcept:
        curr_(vec),
        i_(i)
    {
    }
    Vector< T > *getPtr() noexcept
    {
      return curr_;
    }
    Vector< T > *curr_;
    size_t i_;
  };

  template < class T >
  RAIter< T >::RAIter():
      curr_(nullptr),
      i_(0){};

  template < class T > RAIter< T > &RAIter< T >::operator++() noexcept
  {
    i_++;
    return *this;
  }

  template < class T > RAIter< T > RAIter< T >::operator++(int) noexcept
  {
    RAIter< T > cpy(*this);
    ++i_;
    return cpy;
  }

  template < class T > RAIter< T > RAIter< T >::operator+(int n) const noexcept
  {
    RAIter< T > cpy(*this);
    cpy.i_ += n;
    return cpy;
  }

  template < class T > RAIter< T > &RAIter< T >::operator+=(int n) noexcept
  {
    i_ += n;
    return *this;
  }

  template < class T > RAIter< T > &RAIter< T >::operator--() noexcept
  {
    i_--;
    return *this;
  }

  template < class T > RAIter< T > RAIter< T >::operator--(int) noexcept
  {
    RAIter< T > cpy(*this);
    --i_;
    return cpy;
  }

  template < class T > RAIter< T > RAIter< T >::operator-(int n) const noexcept
  {
    RAIter< T > cpy(*this);
    cpy.i_ -= n;
    return cpy;
  }

  template < class T > RAIter< T > &RAIter< T >::operator-=(int n) noexcept
  {
    i_ -= n;
    return *this;
  }

  template < class T > long long RAIter< T >::operator-(RAIter< T > it) const noexcept
  {
    return static_cast< long long >(i_) - static_cast< long long >(it.i_);
  }

  template < class T > T &RAIter< T >::operator[](long long n) const
  {
    if (!curr_)
    {
      throw std::runtime_error("can't get access to empty vector elements");
    }
    long long new_i = i_ + n;
    return (*curr_).at(new_i);
  }

  template < class T > T &RAIter< T >::operator*() const
  {
    if (!curr_)
    {
      throw std::runtime_error("can't use operator * on empty vector element");
    }
    return (*curr_).at(i_);
  }

  template < class T > T *RAIter< T >::operator->() const
  {
    if (!curr_)
    {
      throw std::runtime_error("can't use operator -> on empty vector element");
    }
    return &(*curr_).at(i_);
  }

  template < class T > bool RAIter< T >::operator==(const RAIter< T > &rhs) const noexcept
  {
    return curr_ == rhs.curr_ && i_ == rhs.i_;
  }

  template < class T > bool RAIter< T >::operator!=(const RAIter< T > &rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template < class T > bool RAIter< T >::operator>(const RAIter< T > &rhs) const noexcept
  {
    return (curr_ == rhs.curr_) && (i_ > rhs.i_);
  }

  template < class T > bool RAIter< T >::operator<(const RAIter< T > &rhs) const noexcept
  {
    return (curr_ == rhs.curr_) && (i_ < rhs.i_);
  }

  template < class T > bool RAIter< T >::operator>=(const RAIter< T > &rhs) const noexcept
  {
    return (curr_ == rhs.curr_) && (i_ >= rhs.i_);
  }

  template < class T > bool RAIter< T >::operator<=(const RAIter< T > &rhs) const noexcept
  {
    return (curr_ == rhs.curr_) && (i_ <= rhs.i_);
  }
}

#endif
