#ifndef VECTOR_ITERATORS_HPP
#define VECTOR_ITERATORS_HPP
#include <cstddef> 
namespace topit
{
  template< class T > struct Vector;

  template< class T >
  struct VIter
  {
    VIter() = default;
    explicit VIter(T*);
    
    VIter< T >& operator++() noexcept;
    VIter< T > operator++(int) noexcept;
    VIter< T >& operator+=(int) noexcept;
    VIter< T > operator+(int) noexcept;

    VIter< T >& operator--() noexcept;
    VIter< T > operator--(int) noexcept;
    VIter< T >& operator-=(int) noexcept;
    VIter< T > operator-(int) noexcept;

    std::ptrdiff_t operator-(const VIter< T >&) const noexcept;

    T& operator*() const noexcept;
    T* operator->() const noexcept;

    bool operator==(const VIter< T >&) const noexcept;
    bool operator!=(const VIter< T >&) const noexcept;
    bool operator>(const VIter< T >&) const noexcept;
    bool operator>=(const VIter< T >&) const noexcept;
    bool operator<(const VIter< T >&) const noexcept;
    bool operator<=(const VIter< T >&) const noexcept;
  private:
    T* node;
  };

  template< class T >
  struct VCIter
  {
    VCIter() = default;
    explicit VCIter(const T*);
    
    VCIter< T >& operator++() noexcept;
    VCIter< T > operator++(int) noexcept;
    VCIter< T >& operator+=(int) noexcept;
    VCIter< T > operator+(int) noexcept;

    VCIter< T >& operator--() noexcept;
    VCIter< T > operator--(int) noexcept;
    VCIter< T >& operator-=(int) noexcept;
    VCIter< T > operator-(int) noexcept;

    std::ptrdiff_t operator-(const VCIter< T >&) const noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const VCIter< T >&) const noexcept;
    bool operator!=(const VCIter< T >&) const noexcept;
    bool operator>(const VCIter< T >&) const noexcept;
    bool operator>=(const VCIter< T >&) const noexcept;
    bool operator<(const VCIter< T >&) const noexcept;
    bool operator<=(const VCIter< T >&) const noexcept;
  private:
    const T* node;
  };
}

template< class T >
topit::VIter< T >::VIter(T* n):
  node(n)
{}

template< class T >
topit::VIter< T >& topit::VIter< T >::operator++() noexcept
{
  ++node;
  return *this;
}
template< class T >
topit::VIter< T > topit::VIter< T >::operator++(int) noexcept
{
  VIter< T > tmp = *this;
  ++node;
  return tmp;
}
template< class T >
topit::VIter< T >& topit::VIter< T >::operator+=(int count) noexcept
{
  node += count;
  return *this;
}
template< class T >
topit::VIter< T > topit::VIter< T >::operator+(int count) noexcept
{
  VIter< T > tmp = *this;
  tmp.node += count;
  return tmp;
}

template< class T >
topit::VIter< T >& topit::VIter< T >::operator--() noexcept
{
  --node;
  return *this;
}
template< class T >
topit::VIter< T > topit::VIter< T >::operator--(int) noexcept
{
  VIter< T > tmp = *this;
  --node;
  return tmp;
}
template< class T >
topit::VIter< T >& topit::VIter< T >::operator-=(int count) noexcept
{
  node -= count;
  return *this;
}
template< class T >
topit::VIter< T > topit::VIter< T >::operator-(int count) noexcept
{
  VIter< T > tmp = *this;
  tmp.node -= count;
  return tmp;
}

template< class T >
std::ptrdiff_t topit::VIter< T >::operator-(const VIter< T >& yaIt) const noexcept
{
  return node - yaIt.node;
}

template< class T >
T& topit::VIter< T >::operator*() const noexcept
{
  return *node;
}
template< class T >
T* topit::VIter< T >::operator->() const noexcept
{
  return node;
}

template< class T >
bool topit::VIter< T >::operator==(const VIter< T >& yaIt) const noexcept
{
  return node == yaIt.node;
}
template< class T >
bool topit::VIter< T >::operator!=(const VIter< T >& yaIt) const noexcept
{
  return node != yaIt.node;
}
template< class T >
bool topit::VIter< T >::operator>(const VIter< T >& yaIt) const noexcept
{
  return node > yaIt.node;
}
template< class T >
bool topit::VIter< T >::operator>=(const VIter< T >& yaIt) const noexcept
{
  return node >= yaIt.node;
}
template< class T >
bool topit::VIter< T >::operator<(const VIter< T >& yaIt) const noexcept
{
  return node < yaIt.node;
}
template< class T >
bool topit::VIter< T >::operator<=(const VIter< T >& yaIt) const noexcept
{
  return node <= yaIt.node;
}

template< class T >
topit::VCIter< T >::VCIter(const T* n):
  node(n)
{}

template< class T >
topit::VCIter< T >& topit::VCIter< T >::operator++() noexcept
{
  ++node;
  return *this;
}
template< class T >
topit::VCIter< T > topit::VCIter< T >::operator++(int) noexcept
{
  VCIter< T > tmp = *this;
  ++node;
  return tmp;
}
template< class T >
topit::VCIter< T >& topit::VCIter< T >::operator+=(int count) noexcept
{
  node += count;
  return *this;
}
template< class T >
topit::VCIter< T > topit::VCIter< T >::operator+(int count) noexcept
{
  VCIter< T > tmp = *this;
  tmp.node += count;
  return tmp;
}

template< class T >
topit::VCIter< T >& topit::VCIter< T >::operator--() noexcept
{
  --node;
  return *this;
}
template< class T >
topit::VCIter< T > topit::VCIter< T >::operator--(int) noexcept
{
  VCIter< T > tmp = *this;
  --node;
  return tmp;
}
template< class T >
topit::VCIter< T >& topit::VCIter< T >::operator-=(int count) noexcept
{
  node -= count;
  return *this;
}
template< class T >
topit::VCIter< T > topit::VCIter< T >::operator-(int count) noexcept
{
  VCIter< T > tmp = *this;
  tmp.node -= count;
  return tmp;
}

template< class T >
std::ptrdiff_t topit::VCIter< T >::operator-(const VCIter< T >& yaIt) const noexcept
{
  return node - yaIt.node;
}

template< class T >
const T& topit::VCIter< T >::operator*() const noexcept
{
  return *node;
}
template< class T >
const T* topit::VCIter< T >::operator->() const noexcept
{
  return node;
}

template< class T >
bool topit::VCIter< T >::operator==(const VCIter< T >& yaIt) const noexcept
{
  return node == yaIt.node;
}
template< class T >
bool topit::VCIter< T >::operator!=(const VCIter< T >& yaIt) const noexcept
{
  return node != yaIt.node;
}
template< class T >
bool topit::VCIter< T >::operator>(const VCIter< T >& yaIt) const noexcept
{
  return node > yaIt.node;
}
template< class T >
bool topit::VCIter< T >::operator>=(const VCIter< T >& yaIt) const noexcept
{
  return node >= yaIt.node;
}
template< class T >
bool topit::VCIter< T >::operator<(const VCIter< T >& yaIt) const noexcept
{
  return node < yaIt.node;
}
template< class T >
bool topit::VCIter< T >::operator<=(const VCIter< T >& yaIt) const noexcept
{
  return node <= yaIt.node;
}

#endif