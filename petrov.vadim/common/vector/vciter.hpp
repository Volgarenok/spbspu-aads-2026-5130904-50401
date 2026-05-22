#ifndef VCITER_HPP
#define VCITER_HPP

#include <cstddef>

namespace topit
{
  template< class T >
  struct Vector;

  template< class T >
  struct VIter;

  template< class T >
  struct VCIter
  {
    friend struct Vector< T >;

    VCIter();
    VCIter(const VIter< T > & rhs);
    VCIter< T > & operator++();
    VCIter< T > operator++(int);
    VCIter< T > operator+(size_t n) const;
    VCIter< T > & operator--();
    VCIter< T > operator--(int);
    VCIter< T > operator-(size_t n) const;
    std::ptrdiff_t operator-(const VCIter< T > & rhs) const;
    bool operator==(const VCIter< T > & rhs) const;
    bool operator!=(const VCIter< T > & rhs) const;
    bool operator>(const VCIter< T > & rhs) const;
    bool operator<(const VCIter< T > & rhs) const;
    bool operator>=(const VCIter< T > & rhs) const;
    bool operator<=(const VCIter< T > & rhs) const;
    const T & operator*() const;
    const T * operator->() const;

    private:
      const T * val_;
      VCIter(const T * val):
        val_(val)
      {}
  };
}

template< class T >
topit::VCIter< T >::VCIter():
  val_(nullptr)
{}

template< class T >
topit::VCIter< T >::VCIter(const VIter< T > & rhs):
  val_(rhs.val_)
{}

template< class T >
topit::VCIter< T > & topit::VCIter< T >::operator++()
{
  ++val_;
  return *this;
}

template< class T >
topit::VCIter< T > topit::VCIter< T >::operator++(int)
{
  VCIter< T > temp(*this);
  ++(*this);
  return temp;
}

template< class T >
topit::VCIter< T > topit::VCIter< T >::operator+(size_t n) const
{
  return VCIter< T >(val_ + n);
}

template< class T >
topit::VCIter< T > & topit::VCIter< T >::operator--()
{
  --val_;
  return *this;
}

template< class T >
topit::VCIter< T > topit::VCIter< T >::operator--(int)
{
  VCIter< T > temp(*this);
  --(*this);
  return temp;
}

template< class T >
topit::VCIter< T > topit::VCIter< T >::operator-(size_t n) const
{
  return VCIter< T >(val_ - n);
}

template< class T >
std::ptrdiff_t topit::VCIter< T >::operator-(const VCIter< T > & rhs) const
{
  return val_ - rhs.val_;
}

template< class T >
bool topit::VCIter< T >::operator==(const VCIter< T > & rhs) const
{
  return val_ == rhs.val_;
}

template< class T >
bool topit::VCIter< T >::operator!=(const VCIter< T > & rhs) const
{
  return val_ != rhs.val_;
}

template< class T >
bool topit::VCIter< T >::operator>(const VCIter< T > & rhs) const
{
  return val_ > rhs.val_;
}

template< class T >
bool topit::VCIter< T >::operator<(const VCIter< T > & rhs) const
{
  return val_ < rhs.val_;
}

template< class T >
bool topit::VCIter< T >::operator>=(const VCIter< T > & rhs) const
{
  return val_ >= rhs.val_;
}

template< class T >
bool topit::VCIter< T >::operator<=(const VCIter< T > & rhs) const
{
  return val_ <= rhs.val_;
}

template< class T >
const T & topit::VCIter< T >::operator*() const
{
  return *val_;
}

template< class T >
const T * topit::VCIter< T >::operator->() const
{
  return val_;
}

#endif
