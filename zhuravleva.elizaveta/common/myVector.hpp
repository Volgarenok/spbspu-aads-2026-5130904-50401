#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP
#include <cstddef>
#include <stdexcept>

namespace zhuravleva
{
  template< class T >
  class myVector
  {
    private:
      T* data_;
      size_t size_;
      size_t capacity_;

    public:
      typedef T* iterator;
      typedef const T* constIterator;

      myVector();
      ~myVector();
      myVector(size_t size);
      myVector(const myVector& other);
      myVector(myVector&& other) noexcept;
      myVector& operator=(const myVector& other);
      myVector& operator=(myVector&& other) noexcept;
      T& operator[](size_t index);
      const T& operator[](size_t index) const;
      size_t size() const noexcept;
      size_t capacity() const noexcept;
      bool empty() const noexcept;
      void pushBack(const T& value);
      void reserve(size_t newCapacity);
      void clear() noexcept;
      void swap(myVector& other) noexcept;
      iterator begin() noexcept;
      iterator end() noexcept;
      constIterator begin() const noexcept;
      constIterator end() const noexcept;
      constIterator cbegin() const noexcept;
      constIterator cend() const noexcept;

  };

  template< class T >
  myVector< T >::myVector():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  template< class T >
  myVector< T >::myVector(size_t size):
    data_(nullptr),
    size_(size),
    capacity_(size)
  {
    if (size_ != 0)
    {
      data_ = new T[size_];
    }
  }

  template< class T >
  myVector< T >::myVector(const myVector& other):
    data_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    if (capacity_ != 0)
    {
      data_ = new T[capacity_];
      for (size_t i = 0; i < size_; i++)
      {
        data_[i] = other.data_[i];
      }
    }
  }

  template< class T >
  myVector< T >::myVector(myVector&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< class T >
  myVector< T >::~myVector()
  {
    delete[] data_;
  }

  template< class T >
  myVector< T >& myVector< T >::operator=(const myVector& other)
  {
    if (this != &other)
    {
      myVector< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  myVector< T >& myVector< T >::operator=(myVector&& other) noexcept
  {
    if (this != &other)
    {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  template< class T >
  T& myVector< T >::operator[](size_t index)
  {
    if (index >= size_)
    {
      throw std::out_of_range("index out of range");
    }
    return data_[index];
  }

  template< class T >
  const T& myVector< T >::operator[](size_t index) const
  {
    if (index >= size_)
    {
      throw std::out_of_range("index out of range");
    }
    return data_[index];
  }

  template< class T >
  size_t myVector< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  size_t myVector< T >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  bool myVector< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  void myVector< T >::reserve(size_t newCapacity)
  {
    if (newCapacity <= capacity_)
    {
      return;
    }
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < size_; i++)
    {
      newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }

  template< class T >
  void myVector< T >::pushBack(const T& value)
  {
    if (size_ == capacity_)
    {
      size_t newCapacity = 0;
      if (capacity_ == 0)
      {
        newCapacity = 1;
      }
      else
      {
        newCapacity = capacity_ * 2;
      }
      reserve(newCapacity);
    }
    data_[size_] = value;
    size_++;
  }

  template< class T >
  void myVector< T >::clear() noexcept
  {
    size_ = 0;
  }

  template< class T >
  void myVector< T >::swap(myVector& other) noexcept
  {
    T* tempData = data_;
    data_ = other.data_;
    other.data_ = tempData;
    size_t tempSize = size_;
    size_ = other.size_;
    other.size_ = tempSize;
    size_t tempCapacity = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = tempCapacity;
  }

  template< class T >
  typename myVector< T >::iterator myVector< T >::begin() noexcept
  {
    return data_;
  }

  template< class T >
  typename myVector< T >::iterator myVector< T >::end() noexcept
  {
    return data_ + size_;
  }

  template< class T >
  typename myVector< T >::constIterator myVector< T >::begin() const noexcept
  {
    return data_;
  }

  template< class T >
  typename myVector< T >::constIterator myVector< T >::end() const noexcept
  {
    return data_ + size_;
  }

    template< class T >
  typename myVector< T >::constIterator myVector< T >::cbegin() const noexcept
  {
    return data_;
  }

  template< class T >
  typename myVector< T >::constIterator myVector< T >::cend() const noexcept
  {
    return data_ + size_;
  }
}
#endif
