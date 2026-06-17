#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cassert>
#include <cstddef>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

namespace lukashevich {
  template< class T >
  using VectorIt = T *;

  template< class T >
  using VectorConstIt = const T *;

  template< class T >
  class Vector {
  public:
    using iterator = VectorIt< T >;
    using const_it = VectorConstIt< T >;

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
    const_it begin() const noexcept;
    const_it end() const noexcept;
    const_it cbegin() const noexcept;
    const_it cend() const noexcept;

  private:
    static T * allocate(size_t capacity);
    static void deallocate(T * data) noexcept;
    static void destroyRange(T * data, size_t begin, size_t end) noexcept;
    static size_t nextCapacity(size_t needed);

    T * data_;
    size_t size_;
    size_t capacity_;
  };

  template< class T >
  Vector< T >::Vector():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  template< class T >
  Vector< T >::Vector(size_t count):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    reserve(count);

    try {
      for (size_t i = 0; i < count; ++i) {
        new (data_ + i) T();
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
  Vector< T >::Vector(size_t count, const T & value):
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
  Vector< T >::Vector(const Vector & rhs):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    reserve(rhs.size_);

    try {
      for (size_t i = 0; i < rhs.size_; ++i) {
        pushBack(rhs[i]);
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
  Vector< T >::Vector(Vector && rhs) noexcept:
    data_(rhs.data_),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {
    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
  }

  template< class T >
  Vector< T >::~Vector()
  {
    clear();
    deallocate(data_);
  }

  template< class T >
  Vector< T > & Vector< T >::operator=(const Vector & rhs)
  {
    if (this != &rhs) {
      Vector temp(rhs);
      swap(temp);
    }

    return *this;
  }

  template< class T >
  Vector< T > & Vector< T >::operator=(Vector && rhs) noexcept
  {
    if (this != &rhs) {
      clear();
      deallocate(data_);

      data_ = rhs.data_;
      size_ = rhs.size_;
      capacity_ = rhs.capacity_;

      rhs.data_ = nullptr;
      rhs.size_ = 0;
      rhs.capacity_ = 0;
    }

    return *this;
  }

  template< class T >
  void Vector< T >::pushBack(const T & value)
  {
    if (size_ == capacity_) {
      reserve(nextCapacity(size_ + 1));
    }

    new (data_ + size_) T(value);
    ++size_;
  }

  template< class T >
  void Vector< T >::popBack()
  {
    if (size_ == 0) {
      throw std::out_of_range("pop from empty vector");
    }

    --size_;
    data_[size_].~T();
  }

  template< class T >
  void Vector< T >::erase(size_t index)
  {
    if (index >= size_) {
      throw std::out_of_range("bad vector index");
    }

    for (size_t i = index; i + 1 < size_; ++i) {
      data_[i] = data_[i + 1];
    }

    popBack();
  }

  template< class T >
  void Vector< T >::clear() noexcept
  {
    destroyRange(data_, 0, size_);
    size_ = 0;
  }

  template< class T >
  void Vector< T >::reserve(size_t capacity)
  {
    if (capacity <= capacity_) {
      return;
    }

    T * newData = allocate(capacity);
    size_t index = 0;

    try {
      for (; index < size_; ++index) {
        new (newData + index) T(data_[index]);
      }
    } catch (...) {
      destroyRange(newData, 0, index);
      deallocate(newData);
      throw;
    }

    destroyRange(data_, 0, size_);
    deallocate(data_);

    data_ = newData;
    capacity_ = capacity;
  }

  template< class T >
  void Vector< T >::swap(Vector & rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
  }

  template< class T >
  T & Vector< T >::operator[](size_t index)
  {
    assert(index < size_);
    return data_[index];
  }

  template< class T >
  const T & Vector< T >::operator[](size_t index) const
  {
    assert(index < size_);
    return data_[index];
  }

  template< class T >
  size_t Vector< T >::getSize() const noexcept
  {
    return size_;
  }

  template< class T >
  size_t Vector< T >::getCapacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  bool Vector< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  VectorIt< T > Vector< T >::begin() noexcept
  {
    return data_;
  }

  template< class T >
  VectorIt< T > Vector< T >::end() noexcept
  {
    return data_ + size_;
  }

  template< class T >
  VectorConstIt< T > Vector< T >::begin() const noexcept
  {
    return data_;
  }

  template< class T >
  VectorConstIt< T > Vector< T >::end() const noexcept
  {
    return data_ + size_;
  }

  template< class T >
  VectorConstIt< T > Vector< T >::cbegin() const noexcept
  {
    return data_;
  }

  template< class T >
  VectorConstIt< T > Vector< T >::cend() const noexcept
  {
    return data_ + size_;
  }

  template< class T >
  T * Vector< T >::allocate(size_t capacity)
  {
    if (capacity == 0) {
      return nullptr;
    }

    if (capacity > std::numeric_limits< size_t >::max() / sizeof(T)) {
      throw std::length_error("vector capacity is too large");
    }

    return static_cast< T * >(operator new(sizeof(T) * capacity));
  }

  template< class T >
  void Vector< T >::deallocate(T * data) noexcept
  {
    operator delete(data);
  }

  template< class T >
  void Vector< T >::destroyRange(T * data, size_t begin, size_t end) noexcept
  {
    for (size_t i = begin; i < end; ++i) {
      data[i].~T();
    }
  }

  template< class T >
  size_t Vector< T >::nextCapacity(size_t needed)
  {
    size_t capacity = 1;

    while (capacity < needed) {
      if (capacity > std::numeric_limits< size_t >::max() / 2) {
        return needed;
      }

      capacity *= 2;
    }

    return capacity;
  }
}

#endif
