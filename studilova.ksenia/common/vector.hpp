#ifndef TOPIT_VECTOR_HPP
#define TOPIT_VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <memory>
#include <algorithm>

namespace topit
{
  template< class T >
  struct Vector
  {
    public:
      Vector() noexcept;
      explicit Vector(size_t size);
      Vector(size_t size, const T& value);
      Vector(const Vector& other);
      Vector(Vector&& other) noexcept;
      ~Vector();

      Vector& operator=(const Vector& other);
      Vector& operator=(Vector&& other) noexcept;

      void swap(Vector& other) noexcept;

      bool isEmpty() const noexcept;
      size_t getSize() const noexcept;
      size_t getCapacity() const noexcept;

      T& operator[](size_t index) noexcept;
      const T& operator[](size_t index) const noexcept;

      T& at(size_t index);
      const T& at(size_t index) const;

    private:
      T* data_;
      size_t size_;
      size_t capacity_;

      void destroyAll() noexcept;
  };
}

template< class T >
topit::Vector< T >::Vector() noexcept :
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
topit::Vector< T >::Vector(size_t size) :
  data_(size ? static_cast< T* >(operator new(sizeof(T) * size)) : nullptr),
  size_(0),
  capacity_(size)
{
  try
  {
    for (; size_ < size; ++size_) 
    {
      new (data_ + size_) T();
    }
  }
  catch(...)
  {
    destroyAll();
    operator delete(data_);
    throw;
  }
}

template< class T >
topit::Vector< T >::Vector(size_t size, const T& value) :
  data_(size ? static_cast< T* >(operator new(sizeof(T) * size)) : nullptr),
  size_(0),
  capacity_(size)
{
  try
  {
    for (; size_ < size; ++size_) 
    {
      new (data_ + size_) T(value);
    }
  }
  catch(...)
  {
    destroyAll();
    operator delete(data_);
    throw;
  }
}

template< class T >
topit::Vector< T >::Vector(const Vector& other) :
  data_(other.size_ ? static_cast< T* >(operator new(sizeof(T) * other.size_)) : nullptr),
  size_(0),
  capacity_(other.size_)
{
  try
  {
    for (; size_ < other.size_; ++size_) 
    {
      new (data_ + size_) T(other.data_[size_]);
    }
  }
  catch(...)
  {
    destroyAll();
    operator delete(data_);
    throw;
  }
}

template< class T >
topit::Vector< T >::Vector(Vector&& other) noexcept :
  data_(other.data_),
  size_(other.size_),
  capacity_(other.capacity_)
{
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template< class T >
topit::Vector< T >::~Vector()
{
  destroyAll();
  operator delete(data_);
}

template< class T >
topit::Vector< T >& topit::Vector< T >::operator=(const Vector& other)
{
  if (this != std::addressof(other))
  {
    Vector tmp(other);
    swap(tmp);
  }
  return *this;
}

template< class T >
topit::Vector< T >& topit::Vector< T >::operator=(Vector&& other) noexcept
{
  if (this != std::addressof(other))
  {
    Vector tmp(std::move(other));
    swap(tmp);
  }
  return *this;
}

template< class T >
void topit::Vector< T >::swap(Vector& other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template< class T >
bool topit::Vector< T >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class T >
size_t topit::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t topit::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
T& topit::Vector< T >::operator[](size_t index) noexcept
{
  return data_[index];
}

template< class T >
const T& topit::Vector< T >::operator[](size_t index) const noexcept
{
  return data_[index];
}

template< class T >
T& topit::Vector< T >::at(size_t index)
{
  if (index >= size_)
  {
    throw std::out_of_range("Vector index is out of range");
  }
  return data_[index];
}

template< class T >
const T& topit::Vector< T >::at(size_t index) const
{
  if (index >= size_)
  {
    throw std::out_of_range("Vector index is out of range");
  }
  return data_[index];
}

template< class T >
void topit::Vector< T >::destroyAll() noexcept
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  size_ = 0;
}

#endif