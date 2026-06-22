#ifndef TOP_IT_ITERATOR_HPP
#define TOP_IT_ITERATOR_HPP
#include <cassert>
#include <cstddef>
namespace malashenko
{
  template< class T >
  struct VecIter {
  public:
    VecIter();
    VecIter(T* ptr);
    VecIter< T >& operator++();
    VecIter< T > operator++(int);
    VecIter< T > operator+(size_t) const noexcept;


    VecIter< T >& operator--();
    VecIter< T > operator--(int);
    VecIter< T > operator-(size_t) const noexcept;

    std::ptrdiff_t operator-(const VecIter< T >& other) const;

    bool operator==(const VecIter& other) const noexcept;
    bool operator!=(const VecIter& other) const noexcept;
    bool operator<(const VecIter& other) const noexcept;
    bool operator>(const VecIter& other) const noexcept;
    bool operator<=(const VecIter& other) const noexcept;
    bool operator>=(const VecIter& other) const noexcept;

    T& operator*();
    T* operator->();
  private:
    T* ptr_;
  };
}

template< class T >
malashenko::VecIter< T >::VecIter():
  ptr_(nullptr)
{}


template< class T >
malashenko::VecIter< T >::VecIter(T* ptr):
  ptr_(ptr)
{}

template< class T >
malashenko::VecIter< T >& malashenko::VecIter< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
malashenko::VecIter< T > malashenko::VecIter< T >::operator++(int)
{
  VecIter< T >tmp{*this};
  ++(*this);
  return tmp;
}

template< class T >
malashenko::VecIter< T > malashenko::VecIter< T >::operator+(size_t n) const noexcept
{
  return VecIter< T >{ptr_ + n};
}


template< class T >
malashenko::VecIter< T >& malashenko::VecIter< T >::operator--()
{
  ptr_--;
  return *this;
}

template< class T >
malashenko::VecIter< T > malashenko::VecIter< T >::operator--(int)
{
  VecIter< T >tmp{*this};
  --(*this);
  return tmp;
}

template<class T>
malashenko::VecIter<T> malashenko::VecIter<T>::operator-(size_t n) const noexcept
{
  return VecIter<T>{ptr_ - n};
}

template< class T >
std::ptrdiff_t malashenko::VecIter< T >::operator-(const VecIter< T >& other) const
{
  return ptr_ - other.ptr_;
}


template< class T >
T& malashenko::VecIter< T >::operator*()
{
  assert(ptr_ != nullptr);
  return *ptr_;
}

template< class T >
T* malashenko::VecIter< T >::operator->()
{
  return std::addressof(ptr_);
}

template< class T >
bool malashenko::VecIter< T >::operator!=(const VecIter< T >& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
bool malashenko::VecIter< T >::operator==(const VecIter< T >& rhs) const noexcept
{
  return ptr_ == rhs.ptr_;
}

template< class T >
bool malashenko::VecIter< T >::operator<(const VecIter& other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool malashenko::VecIter< T >::operator>(const VecIter& other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool malashenko::VecIter< T >::operator<=(const VecIter& other) const noexcept
{
  return ptr_ <= other.ptr_;
}

template< class T >
bool malashenko::VecIter< T >::operator>=(const VecIter& other) const noexcept
{
  return ptr_ >= other.ptr_;
}

#endif
