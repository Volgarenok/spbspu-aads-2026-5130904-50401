#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cassert>
#include <cstddef>
#include <new>
#include <stdexcept>
#include <utility>

namespace lukashevich {
  template< class T >
  class Vector {
  public:
    using iterator = T *;
    using const_iterator = const T *;

    Vector();
    explicit Vector(size_t count);
    Vector(size_t count, const T & value);
    Vector(const Vector & rhs);
    Vector(Vector && rhs) noexcept;
    ~Vector();

    Vector & operator=(const Vector & rhs);
    Vector & operator=(Vector && rhs) noexcept;

    void pushBack(const T & value);
    void popBack();
    void erase(size_t index);
    void clear() noexcept;
    void reserve(size_t capacity);
    void swap(Vector & rhs) noexcept;

    T & operator[](size_t index);
    const T & operator[](size_t index) const;

    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    bool isEmpty() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

  private:
    static T * allocate(size_t capacity);
    static void deallocate(T * data) noexcept;
    static void destroyRange(T * data, size_t begin, size_t end) noexcept;
    static size_t nextCapacity(size_t needed);

    T * data_;
    size_t size_;
    size_t capacity_;
  };
}

template< class T >
lukashevich::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
lukashevich::Vector< T >::Vector(size_t count):
  data_(nullptr),
  size_(0),
  capacity_(0)
{
  reserve(count);

  try {
    for (size_t i = 0; i < count; ++i) {
      new (data_ + i) T();
    }
    size_ = count;
  } catch (...) {
    destroyRange(data_, 0, size_);
    deallocate(data_);
    data_ = nullptr;
    capacity_ = 0;
    throw;
  }
}

template< class T >
lukashevich::Vector< T >::Vector(size_t count, const T & value):
  data_(nullptr),
  size_(0),
  capacity_(0)
{
  reserve(count);

  try {
    for (size_t i = 0; i < count; ++i) {
      new (data_ + i) T(value);
      ++size_;
    }
  } catch (...) {
    clear();
    deallocate(data_);
    data_ = nullptr;
    capacity_ = 0;
    throw;
  }
}

template< class T >
lukashevich::Vector< T >::~Vector()
{
  clear();
  deallocate(data_);
}

template< class T >
T * lukashevich::Vector< T >::allocate(size_t capacity)
{
  if (capacity == 0) {
    return nullptr;
  }

  return static_cast< T * >(operator new(sizeof(T) * capacity));
}

template< class T >
void lukashevich::Vector< T >::deallocate(T * data) noexcept
{
  operator delete(data);
}

template< class T >
void lukashevich::Vector< T >::destroyRange(T * data, size_t begin, size_t end) noexcept
{
  for (size_t i = begin; i < end; ++i) {
    data[i].~T();
  }
}

template< class T >
size_t lukashevich::Vector< T >::nextCapacity(size_t needed)
{
  size_t capacity = 1;

  while (capacity < needed) {
    capacity *= 2;
  }

  return capacity;
}

#endif
