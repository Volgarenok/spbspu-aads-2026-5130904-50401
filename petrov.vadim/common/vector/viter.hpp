#ifndef VITER_HPP
#define VITER_HPP

#include <cstddef>

namespace topit
{
  template< class T >
  struct Vector;

  template< class T >
  struct VCIter;

  template< class T >
  struct VIter
  {
    friend struct Vector< T >;
    friend struct VCIter< T >;

    VIter();
    VIter< T > & operator++();
    VIter< T > operator++(int);
    VIter< T > operator+(size_t n) const;
    VIter< T > & operator--();
    VIter< T > operator--(int);
    VIter< T > operator-(size_t n) const;
    std::ptrdiff_t operator-(const VIter< T > & rhs) const;
    bool operator==(const VIter< T > & rhs) const;
    bool operator!=(const VIter< T > & rhs) const;
    bool operator>(const VIter< T > & rhs) const;
    bool operator<(const VIter< T > & rhs) const;
    bool operator>=(const VIter< T > & rhs) const;
    bool operator<=(const VIter< T > & rhs) const;
    T & operator*() const;
    T * operator->() const;

    private:
      T * val_;
      VIter(T * val):
        val_(val)
      {}
  };
}

template< class T >
topit::VIter< T >::VIter():
  val_(nullptr)
{}

template< class T >
topit::VIter< T > & topit::VIter< T >::operator++()
{
  ++val_;
  return *this;
}

template< class T >
topit::VIter< T > topit::VIter< T >::operator+(size_t n) const
{
  return VIter< T >(val_ + n);
}

template< class T >
topit::VIter< T > topit::VIter< T >::operator++(int)
{
  VIter< T > temp(*this);
  ++(*this);
  return temp;
}

template< class T >
topit::VIter< T > & topit::VIter< T >::operator--()
{
  --val_;
  return *this;
}

template< class T >
topit::VIter< T > topit::VIter< T >::operator--(int)
{
  VIter< T > temp(*this);
  --(*this);
  return temp;
}

template< class T >
topit::VIter< T > topit::VIter< T >::operator-(size_t n) const
{
  return VIter< T >(val_ - n);
}

template< class T >
std::ptrdiff_t topit::VIter< T >::operator-(const VIter< T > & rhs) const
{
  return val_ - rhs.val_;
}

template< class T >
bool topit::VIter< T >::operator==(const VIter< T > & rhs) const
{
  return val_ == rhs.val_;
}

template< class T >
bool topit::VIter< T >::operator!=(const VIter< T > & rhs) const
{
  return val_ != rhs.val_;
}

template< class T >
bool topit::VIter< T >::operator>(const VIter< T > & rhs) const
{
  return val_ > rhs.val_;
}

template< class T >
bool topit::VIter< T >::operator<(const VIter< T > & rhs) const
{
  return val_ < rhs.val_;
}

template< class T >
bool topit::VIter< T >::operator>=(const VIter< T > & rhs) const
{
  return val_ >= rhs.val_;
}

template< class T >
bool topit::VIter< T >::operator<=(const VIter< T > & rhs) const
{
  return val_ <= rhs.val_;
}

template< class T >
T & topit::VIter< T >::operator*() const
{
  return *val_;
}

template< class T >
T * topit::VIter< T >::operator->() const
{
  return val_;
}

#endif
