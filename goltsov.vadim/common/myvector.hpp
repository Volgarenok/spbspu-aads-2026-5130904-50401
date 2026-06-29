#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP
#include <cstddef>
#include <stdexcept>

namespace goltsov
{
  template< class T >
  class VIter
  {
    T* value_;
  public:
    VIter(T*);
    bool operator==(VIter< T > other) const noexcept;
    bool operator!=(VIter< T > other)const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    VIter< T >& operator++() noexcept;
    void operator+=(size_t n) noexcept;
    VIter< T > operator+(size_t n) const noexcept;
    size_t operator-(VIter< T >) const noexcept;
    VIter< T >& operator--() noexcept;
    void operator-=(size_t n) noexcept;
    VIter< T > operator-(size_t n) const noexcept;
  };

  template< class T >
  class Vector
  {
    T* data_;
    size_t size_, capacity_;
    void unsafePushBack(const T&) noexcept;
  public:
    Vector();
    ~Vector();
    Vector(const Vector< T >&);
    Vector(Vector< T >&&) noexcept;
    Vector(size_t);
    Vector(std::initializer_list< T > il);
    Vector< T >& operator=(const Vector< T >&);
    Vector< T >& operator=(Vector< T >&&) noexcept;
    Vector(size_t size, const T& init);

    T& operator[](size_t i) noexcept;
    const T& operator[](size_t) const noexcept;
    T& at(size_t i);
    const T& at(size_t i) const;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void reserve(size_t);
    void shrinkToFit();
    void normalize();
    void swap(Vector< T >&) noexcept;

    void pushBackCount(const T&, size_t);
    void pushBack(const T&);
    template< class IT >
    void pushBackRange(IT, size_t);
    void popBack();


    void insert(size_t, const T&);
    template< class IT >
    void insert(size_t, IT, size_t);
    void erase(size_t);
    void erase(size_t, size_t);

    VIter< T > begin() const;
    VIter< T > end() const;
    void insert(const VIter< T >, const T&);
    void insert(const VIter< T >, const VIter< T >, const VIter< T >);
    void insert(const VIter< T >, const VIter< T >, size_t);
    void erase(const VIter< T >);
    void erase(const VIter< T >, const VIter< T >);
    void erase(const VIter< T >, size_t);
  };

  template< class T >
  bool operator==(const Vector< T >& rhs, const Vector< T >& lhs);
  template< class T >
  bool operator!=(const Vector< T >& rhs, const Vector< T >& lhs);
}

namespace goltsov
{
  template< class T >
  bool operator==(const Vector< T >& rhs, const Vector< T >& lhs)
  {
    bool isEqual = lhs.getSize() == rhs.getSize();
    for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
    return isEqual;
  }

  template< class T >
  bool operator!=(const Vector< T >& rhs, const Vector< T >& lhs)
  {
    return !(rhs == lhs);
  }

  template< class T >
  Vector< T >::Vector():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  template< class T >
  Vector< T >::Vector(const Vector< T >& other):
    size_(other.size_),
    capacity_(size_ ? size_ * 2 : 1)
  {
    data_ = static_cast< T* >(::operator new (sizeof(T) * capacity_));
    size_t i = 0;
    try
    {
      for (; i < size_; ++i)
      {
        new (data_ + i) T(other.data_[i]);
      }
      for (; i < capacity_; ++i)
      {
        new (data_ + i) T();
      }
    }
    catch(...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        data_[j].~T();
      }
      ::operator delete (data_);
      throw;
    }
  }

  template< class T >
  Vector< T >::Vector(Vector< T >&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
  }

  template< class T >
  Vector< T >::Vector(size_t size):
    size_(size),
    capacity_(size_ ? size_ * 2 : 1)
  {
    data_ = static_cast< T* > (::operator new (sizeof(T) * capacity_));

    size_t i = 0;
    try
    {
      for (; i < capacity_; ++i)
      {
        new (data_ + i) T();
      }
    }
    catch(...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        data_[j].~T();
      }
      ::operator delete (data_);
      throw;
    }
  }

  template< class T >
  Vector< T >::Vector(size_t size, const T& init):
    size_(size),
    capacity_(size ? size * 2 : 1)
  {
    data_ = static_cast< T* > (::operator new (sizeof(T) * capacity_));

    size_t i = 0;
    try
    {
      for (; i < size; ++i)
      {
        new (data_ + i) T(init);
      }
      for (; i < capacity_; ++i)
      {
        new (data_ + i) T();
      }
    }
    catch(...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        data_[i].~T();
      }
      ::operator delete (data_);
      throw;
    }
  }

  template< class T >
  Vector< T >::Vector(std::initializer_list< T > il):
    size_(il.size()),
    capacity_(size_ ? size_ * 2 : 1)
  {
    data_ = static_cast< T* >(::operator new (sizeof(T) * capacity_));

    size_t i = 0;
    try
    {
      for (auto it = il.begin(); it != il.end(); ++it)
      {
        data_[i] = * it;
        ++i;
      }
    }
    catch(const std::exception& e)
    {
      for (size_t j = 0; j < i; ++j)
      {
        data_[i].~T();
      }
      ::operator delete (data_);
      throw;
    }
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(const Vector< T >& other)
  {
    Vector< T > cop(other);
    swap(cop);
    return * this;
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(Vector< T >&& other) noexcept
  {
    Vector< T > cop(std::move(other));
    swap(cop);
    return * this;
  }

  template< class T >
  Vector< T >::~Vector()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
  }


  template< class T >
  void Vector< T >::reserve(size_t new_capacity)
  {
    if (new_capacity > capacity_)
    {
      T* d = static_cast< T* >(::operator new (sizeof(T) * new_capacity));
      size_t i = 0;
      try
      {
        for (; i < size_; ++i)
        {
          new (d + i) T(data_[i]);
        }
      }
      catch (...)
      {
        for (size_t j = 0; j < i; ++j)
        {
          d[j].~T();
        }
        ::operator delete (d);
        throw;
      }
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i].~T();
      }
      ::operator delete(data_);
      data_ = d;
      capacity_ = new_capacity;
    }
  }

  template< class T >
  void Vector< T >::shrinkToFit()
  {
    size_t new_capacity = 0;
    if (capacity_ != size_)
    {
      new_capacity = size_;
      T* d = static_cast< T* >(::operator new (sizeof(T) * new_capacity));
      size_t i = 0;
      try
      {
        for (; i < size_; ++i)
        {
          new (d + i) T(data_[i]);
        }
      }
      catch (...)
      {
        for (size_t j = 0; j < i; ++j)
        {
          d[j].~T();
        }
        ::operator delete (d);
        throw;
      }
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i].~T();
      }
      ::operator delete(data_);
      data_ = d;
      capacity_ = new_capacity;
    }
  }

  template< class T >
  void Vector< T >::normalize()
  {
    size_t new_capacity = 0;
    if (size_ == 0)
    {
      new_capacity = 2;
    }
    else if (size_ * 2 < capacity_)
    {
      new_capacity = size_ * 2;
    }
    else
    {
      return;
    }
    T* d = static_cast< T* >(::operator new (sizeof(T) * new_capacity));
    size_t i = 0;
    try
    {
      for (; i < size_; ++i)
      {
        new (d + i) T(data_[i]);
      }
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        d[j].~T();
      }
      ::operator delete (d);
      throw;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    data_ = d;
    capacity_ = new_capacity;
  }

  template< class T >
  void Vector< T >::swap(Vector< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }



  template< class T >
  bool Vector< T >::isEmpty() const noexcept
  {
    return size_ == 0;
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
  T& Vector< T >::operator[](size_t i) noexcept
  {
    return data_[i];
  }

  template< class T >
  const T& Vector< T >::operator[](size_t i) const noexcept
  {
    return data_[i];
  }

  template< class T >
  T& Vector< T >::at(size_t i)
  {
    if (i < getSize())
    {
      return data_[i];
    }
    else
    {
      throw std::out_of_range("Bad i");
    }
  }

  template< class T >
  const T& Vector< T >::at(size_t i) const
  {
    if (i < getSize())
    {
      return data_[i];
    }
    else
    {
      throw std::out_of_range("Bad i");
    }
  }



  template< class T >
  void Vector< T >::unsafePushBack(const T& v) noexcept
  {
    new (data_ + size_) T(v);
    ++size_;
  }

  template< class T >
  void Vector< T >::pushBack(const T& v)
  {
    if (capacity_ <= size_)
    {
      reserve(size_ ? size_ * 2 : 2);
    }
    unsafePushBack(v);
  }

  template< class T >
  void Vector< T >::pushBackCount(const T& v, size_t count)
  {
    size_t new_size = size_ + count;
    if (capacity_ < new_size)
    {
      reserve(new_size * 2);
    }
    for (size_t i = 0; i < count; ++i)
    {
      unsafePushBack(v);
    }
  }

  template< class T >
  template< class IT >
  void Vector< T >::pushBackRange(IT from, size_t count)
  {
    size_t new_size = size_ + count;
    if (capacity_ < new_size)
    {
      reserve(new_size * 2);
    }
    for (size_t i = 0; i < count; ++i)
    {
      unsafePushBack(* from);
      ++from;
    }
  }

  template< class T >
  void Vector< T >::popBack()
  {
    if (size_ > 0)
    {
      data_[size_ - 1].~T();
      size_--;
      normalize();
    }
  }



  template< class T >
  void Vector< T >::insert(size_t i, const T& v)
  {
    if (i != 0 && i > size_)
    {
      throw std::out_of_range("Index out of range");
    }
    if (i == size_)
    {
      pushBack(v);
      return;
    }
    T* d = static_cast< T* >(::operator new (sizeof(T) * (capacity_ + 2)));
    size_t j = 0;
    try
    {
      for (; j < i; ++j)
      {
        new (d + j) T(data_[j]);
      }
      new (d + j) T(v);
      ++j;
      for (; j < size_ + 1; ++j)
      {
        new (d + j) T(data_[j - 1]);
      }
      for (; j < capacity_ + 2; ++j)
      {
        new (d + j) T();
      }
    }
    catch (...)
    {
      for (size_t k = 0; k < j; ++k)
      {
        d[k].~T();
      }
      ::operator delete (d);
      throw;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    ++size_;
    capacity_ += 2;
    data_ = d;
  }

  template< class T >
  template< class IT >
  void Vector< T >::insert(size_t i, IT from, size_t count)
  {
    if (i != 0 && i > size_)
    {
      throw std::out_of_range("Index out of range");
    }
    if (i == size_)
    {
      pushBackRange(from, count);
      return;
    }
    T* d = static_cast< T* >(::operator new (sizeof(T) * (capacity_ + 2 * count)));
    size_t j = 0;
    try
    {
      for (; j < i; ++j)
      {
        new (d + j) T(data_[j]);
      }
      for (; j < i + count; ++j)
      {
        new (d + j) T(* from);
        ++from;
      }
      for (; j < size_ + count; ++j)
      {
        new (d + j) T(data_[j - count]);
      }
      for (; j < capacity_ + 2 * count; ++j)
      {
        new (d + j) T();
      }
    }
    catch(...)
    {
      for (size_t k = 0; k < j; ++k)
      {
        d[k].~T();
      }
      ::operator delete (d);
      throw;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    size_ += count;
    capacity_ += 2 * count;
    data_ = d;
  }

  template< class T >
  void Vector< T >::erase(size_t i)
  {
    if (i >= size_)
    {
      throw std::out_of_range("Index out of range");
    }
    T* d = static_cast< T* >(::operator new (sizeof(T) * ((size_- 1) * 2)));
    size_t j = 0;
    try
    {
      for (; j < i; ++j)
      {
        new (d + j) T(data_[j]);
      }
      for (; j < size_ - 1; ++j)
      {
        new (d + j) T(data_[j + 1]);
      }
      for (; j < ((size_- 1) * 2); ++j)
      {
        new (d + j) T();
      }
    }
    catch (...)
    {
      for (size_t k = 0; k < j; ++k)
      {
        d[k].~T();
      }
      ::operator delete (d);
      throw;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    --size_;
    capacity_ = size_ * 2;
    data_ = d;
  }

  template< class T >
  void Vector< T >::erase(size_t i, size_t n)
  {
    if (i + n > size_)
    {
      throw std::out_of_range("Index out of range");
    }
    T* d = static_cast< T* >(::operator new (sizeof(T) * ((size_ - n) * 2)));
    size_t j = 0;
    try
    {
      for (; j < i; ++j)
      {
        new (d + j) T(data_[j]);
      }
      for (; j < size_ - n; ++j)
      {
        new (d + j) T(data_[j + n]);
      }
      for (; j < ((size_ - n) * 2); ++j)
      {
        new (d + j) T();
      }
    }
    catch (...)
    {
      for (size_t k = 0; k < j; ++k)
      {
        d[k].~T();
      }
      ::operator delete (d);
      throw;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    size_ -= n;
    capacity_ = size_ * 2;
    data_ = d;
  }




  template< class T >
  VIter< T >::VIter(T* other):
    value_(other)
  {}

  template< class T >
  bool VIter< T >::operator==(VIter< T > other) const noexcept
  {
    return other.value_ == value_;
  }

  template< class T >
  bool VIter< T >::operator!=(VIter< T > other) const noexcept
  {
    return other.value_ != value_;
  }

  template< class T >
  T& VIter< T >::operator*() const noexcept
  {
    return * value_;
  }
  template< class T >
  T* VIter< T >::operator->() const noexcept
  {
    return value_;
  }
  template< class T >
  VIter< T >& VIter< T >::operator++() noexcept
  {
    value_ = value_ + 1;
    return *this;
  }
  template< class T >
  void VIter< T >::operator+=(size_t n) noexcept
  {
    value_ += n;
  }
  template< class T >
  VIter< T > VIter< T >::operator+(size_t n) const noexcept
  {
    return {value_ + n};
  }
  template< class T >
  VIter< T >& VIter< T >::operator--() noexcept
  {
    value_--;
    return (* this);
  }
  template< class T >
  void VIter< T >::operator-=(size_t n) noexcept
  {
    value_ -= n;
  }
  template< class T >
  VIter< T > VIter< T >::operator-(size_t n) const noexcept
  {
    return VIter< T >(value_ - n);
  }

  template< class T >
  VIter< T > Vector< T >::begin() const
  {
    return VIter< T >(data_);
  }
  template< class T >
  VIter< T > Vector< T >::end() const
  {
    return VIter< T >(data_ + size_);
  }



  template< class T >
  void Vector< T >::insert(const VIter< T > i, const T& val)
  {
    insert(i - begin(), val);
  }
  template< class T >
  void Vector< T >::insert(const VIter< T > i, const VIter< T > from, const VIter< T > to)
  {
    size_t count = 0;
    VIter< T > temp = from;
    while (temp != to)
    {
      ++count;
      ++temp;
    }
    insert(i - begin(), from, count);
  }
  template< class T >
  void Vector< T >::insert(const VIter< T > i, const VIter< T > from, size_t count)
  {
    insert(i - begin(), from, count);
  }
  template< class T >
  void Vector< T >::erase(const VIter< T > i)
  {
    erase(i - begin());
  }
  template< class T >
  void Vector< T >::erase(const VIter< T > i, const VIter< T > to)
  {
    size_t count = 0;
    VIter< T > now_temp = i;
    while(now_temp != to)
    {
      ++count;
      ++now_temp;
    }
    erase(i - begin(), count);
  }
  template< class T >
  void Vector< T >::erase(VIter< T > i, size_t count)
  {
    erase(i - begin(), count);
  }

  template< class T >
  size_t VIter< T >::operator-(VIter< T > i) const noexcept
  {
    return value_ - i.value_;
  }
}

#endif
