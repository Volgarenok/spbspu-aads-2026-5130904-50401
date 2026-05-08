#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include "viter.hpp"

namespace sedov
{
  template< class T >
  struct Vector
  {
    ~Vector();
    Vector();
    Vector(const Vector< T > &);
    Vector(Vector< T > &&) noexcept;
    Vector(size_t size, const T & init);
    explicit Vector(std::initializer_list< T > il);
    Vector< T > & operator=(const Vector< T > &);
    Vector< T > & operator=(Vector< T > &&) noexcept;

    void swap(Vector< T > & rhs) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    T & operator[](size_t id) noexcept;
    const T & operator[](size_t id) const noexcept;
    T & at(size_t id);
    const T & at(size_t id) const;

    void pushBack(const T & v);
    void popBack();

    void insert(size_t i, const T & v);
    void insert(size_t i, const Vector< T > & rhs, size_t start, size_t end);
    void erase(size_t i);
    void erase(size_t start, size_t count);

    CIter< T > cbegin() const noexcept;
    CIter< T > cend() const noexcept;
    Iter< T > begin() noexcept;
    Iter< T > end() noexcept;
    CIter< T > begin() const noexcept;
    CIter< T > end() const noexcept;

    void insert(CIter< T > it, const T & v);
    void insert(CIter< T > it, T && v);
    void insert(CIter< T > i, CIter< T > start, CIter< T > end);

    CIter< T > erase(CIter< T > i);
    CIter< T > erase(CIter< T > start, CIter< T > end);
    template< class C >
    CIter< T > erase(CIter< T > start, CIter< T > end, C c);

    void reserve(size_t k);
    void shrinkToFit();
    void pushBackCount(size_t k, const T & v);
    template< class IT >
    void pushBackRange(IT b, size_t c);
    void unsafePushBack(const T & v);

  private:
    T * data_;
    size_t size_, capacity_;
    explicit Vector(size_t size);
    void grow(size_t newCap);
    template< class P >
    void uniInsert(size_t pos, P && value);
  };
  template< class T >
  bool operator==(const Vector< T > & lhs, const Vector< T > & rhs);
}

template< class T >
sedov::Vector< T >::~Vector()
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  size_ = 0;
  operator delete(data_);
}

template< class T >
sedov::Vector< T >::Vector():
  data_(nullptr), size_(0), capacity_(0)
{}

template< class T >
sedov::Vector< T >::Vector(const Vector< T > & rhs):
  Vector(rhs.getCapacity())
{
  size_t i = 0;
  for (; i < rhs.getSize(); ++i)
  {
    new(&data_[i]) T(rhs.data_[i]);
  }
  size_ = rhs.size_;
}

template< class T >
sedov::Vector< T >::Vector(Vector< T > && rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template< class T >
sedov::Vector< T >::Vector(size_t size):
  data_(size ? static_cast< T * >(operator new(sizeof(T) * size)) : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
sedov::Vector< T >::Vector(size_t size, const T & init):
  Vector(size)
{
  for (size_t i = 0; i < size; ++i)
  {
    new(&data_[i]) T(init);
  }
  size_ = size;
}

template< class T >
sedov::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it)
  {
    new(&data_[i++]) T(*it);
  }
  size_ = il.size();
}

template< class T >
sedov::Vector< T > & sedov::Vector< T >::operator=(const Vector< T > & rhs)
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }
  Vector< T > cpy = rhs;
  swap(cpy);
  return *this;
}

template< class T >
sedov::Vector< T > & sedov::Vector< T >::operator=(Vector< T > && rhs) noexcept
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
void sedov::Vector< T >::swap(Vector< T > & rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
bool sedov::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t sedov::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t sedov::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
T & sedov::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T > * cthis = this;
  return const_cast< T & >((*cthis)[id]);
}

template< class T >
const T & sedov::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T & sedov::Vector< T >::at(size_t id)
{
  const Vector< T >* cthis = this;
  return const_cast< T & >(cthis->at(id));
}

template< class T >
const T & sedov::Vector< T >::at(size_t id) const
{
  if (id < getSize())
  {
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
}

template < class T >
void sedov::Vector< T >::grow(size_t newCap)
{
  T * newData = static_cast< T * >(operator new(sizeof(T) * newCap));
  size_t c = 0;
  try
  {
    for (size_t i = 0; i < size_; ++i)
    {
      new (&newData[i]) T(std::move(data_[i]));
      ++c;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    operator delete(data_);
    data_ = newData;
    capacity_ = newCap;
  }
  catch(...)
  {
    for (size_t i = 0; i < c; ++i)
    {
      newData[i].~T();
    }
    operator delete(newData);
    throw;
  }
}

template< class T >
void sedov::Vector< T >::pushBack(const T & v)
{
  Vector< T > t = *this;
  if (t.size_ == t.capacity_)
  {
    size_t newCap = (t.capacity_ == 0) ? 1 : t.capacity_ * 2;
    t.grow(newCap);
  }
  new (&t.data_[t.size_]) T(v);
  ++t.size_;
  swap(t);
}

template< class T >
void sedov::Vector< T >::popBack()
{
  if (size_)
  {
    --size_;
    data_[size_].~T();
  }
}

template< class T >
void sedov::Vector< T >::insert(size_t i, const T & v)
{
  if (i > size_)
  {
    throw std::out_of_range("Index out of range");
  }
  uniInsert(i, v);
}

template< class T >
void sedov::Vector< T >::insert(size_t i, const Vector< T > & rhs, size_t start, size_t end)
{
  if (i > size_)
  {
    throw std::out_of_range("Index out of range");
  }
  if (end > rhs.getSize())
  {
    throw std::out_of_range("End of vector out of range");
  }
  size_t count = (end - start);
  size_t newCap = capacity_ + count;
  if (size_ + count < capacity_)
  {
    newCap = capacity_;
  }
  T* newData = static_cast< T * >(operator new(sizeof(T) * newCap));
  size_t j = 0;
  size_t k = 0;
  try
  {
    for (; j < i; ++j)
    {
      new(&newData[j]) T(data_[j]);
    }
    for (; k < count; ++k)
    {
      new(&newData[j + k]) T(rhs[start + k]);
    }
    for (; j < size_; ++j)
    {
      new(&newData[j + k]) T(data_[j]);
    }
  }
  catch (...)
  {
    for (size_t l = 0; l < j + k; ++l)
    {
      (&newData[l])->~T();
    }
    operator delete(newData);
    throw;
  }
  for (j = 0; j < size_; ++j)
  {
    (&data_[j])->~T();
  }
  operator delete(data_);
  data_ = newData;
  size_ += count;
  capacity_ = newCap;
}

template< class T >
void sedov::Vector< T >::erase(size_t i)
{
  if (i >= size_)
  {
    throw std::out_of_range("Index out of range");
  }
  if (i == size_ - 1)
  {
    popBack();
    return;
  }
  T * v = static_cast< T * >(operator new(sizeof(T) * (size_ - 1)));
  size_t j = 0;
  try
  {
    for (; j < i; ++j)
    {
      new(&v[j]) T(data_[j]);
    }
    for (; j < size_ - 1; ++j)
    {
      new(&v[j]) T(data_[j + 1]);
    }
  }
  catch (...)
  {
    for (size_t k = 0; k < j; ++k) {
      (&v[k])->~T();
    }
    operator delete(v);
    throw;
  }
  for (j = 0; j < size_; ++j)
  {
    (&data_[j])->~T();
  }
  operator delete(data_);
  data_ = v;
  size_--;
}

template< class T >
void sedov::Vector< T >::erase(size_t start, size_t count)
{
  if (start >= size_)
  {
    throw std::out_of_range("Start out of range");
  }
  if (start == size_ - 1)
  {
    popBack();
    return;
  }
  if (start + count >= size_)
  {
    count = size_ - start;
  }
  T * v = static_cast< T * >(operator new(sizeof(T) * (size_ - count)));
  size_t i = 0;
  try
  {
    for (; i < start; ++i)
    {
      new(&v[i]) T(data_[i]);
    }
    for (; i < size_ - count; ++i)
    {
      new(&v[i]) T(data_[i + count]);
    }
  }
  catch (...)
  {
    for (; i > 0; i--)
    {
      (&v[i - 1])->~T();
    }
    operator delete(v);
    throw;
  }
  for (i = 0; i < size_; ++i)
  {
    (&data_[i])->~T();
  }
  operator delete(data_);
  data_ = v;
  size_ -= count;
}

template< class T >
sedov::CIter< T > sedov::Vector< T >::cbegin() const noexcept
{
  return CIter< T >(data_);
}

template< class T >
sedov::CIter< T > sedov::Vector< T >::cend() const noexcept
{
  return CIter< T >(data_ + size_);
}

template< class T >
sedov::Iter< T > sedov::Vector< T >::begin() noexcept
{
  return Iter< T >(data_);
}

template< class T >
sedov::Iter< T > sedov::Vector< T >::end() noexcept
{
  return Iter< T >(data_ + size_);
}

template< class T >
sedov::CIter< T > sedov::Vector< T >::begin() const noexcept
{
  return CIter< T >(data_);
}

template< class T >
sedov::CIter< T > sedov::Vector< T >::end() const noexcept
{
  return CIter< T >(data_ + size_);
}

template< class T >
void sedov::Vector< T >::insert(CIter< T > it, const T& v)
{
  if (it < CIter< T >(data_) || it > CIter< T >(data_ + size_))
  {
    throw std::out_of_range("position out of range");
  }
  size_t pos = it - CIter< T >(data_);
  uniInsert(pos, v);
}

template< class T >
void sedov::Vector< T >::insert(CIter< T > it, T&& v)
{
  if (it < CIter< T >(data_) || it > CIter< T >(data_ + size_))
  {
    throw std::out_of_range("position out of range");
  }
  size_t pos = it - CIter< T >(data_);
  uniInsert(pos, std::move(v));
}

template< class T >
void sedov::Vector< T >::insert(CIter< T > i, CIter< T > start, CIter< T > end)
{
  if (i < CIter< T >(data_) || i > CIter< T >(data_ + size_))
  {
    throw std::out_of_range("iterator index is out of range");
  }

  size_t index = i - CIter< T >(data_);
  size_t count = end - start;
  if (!count)
  {
    return;
  }
  size_t newCap = capacity_;
  if (size_ + count > capacity_)
  {
    newCap = capacity_ * 2 + count;
  }
  T * newData = static_cast< T * >(operator new(sizeof(T) * newCap));
  size_t j = 0;
  try
  {
    for (; j < index; ++j)
    {
      new(&newData[j]) T(data_[j]);
    }
    for (auto it = start; it != end; ++it)
    {
      new(&newData[j++]) T(*it);
    }
    for (size_t k = index; k < size_; ++k)
    {
      new(&newData[j++]) T(data_[k]);
    }
  }
  catch (...)
  {
    for (size_t k = 0; k < j; ++k)
    {
      (&newData[k])->~T();
    }
    operator delete(newData);
    throw;
  }
  for (size_t j = 0; j < size_; ++j)
  {
    (&data_[j])->~T();
  }
  operator delete(data_);
  data_ = newData;
  size_ += count;
  capacity_ = newCap;
}

template< class T >
sedov::CIter< T > sedov::Vector< T >::erase(CIter< T > i)
{
  if (i < CIter< T >(data_) || i >= CIter< T >(data_ + size_))
  {
    throw std::out_of_range("index is out of range in erase");
  }
  size_t c = i - cbegin();
  erase(c);
  return CIter< T >{data_ + c};
}

template< class T >
sedov::CIter< T > sedov::Vector< T >::erase(CIter< T > start, CIter< T > end)
{
  if (start < CIter< T >(data_) || end > CIter< T >(data_ + size_) || start > end)
  {
    throw std::out_of_range("range out of range in erase");
  }
  size_t count = end - start;
  size_t pos = start - cbegin();
  erase(pos, count);
  return CIter< T >{data_ + pos};
}

template< class T >
template< class C >
sedov::CIter< T > sedov::Vector< T >::erase(CIter< T > start, CIter< T > end, C c)
{
  if (start < CIter< T >(data_) || end > CIter< T >(data_ + size_) || start > end)
  {
    throw std::out_of_range("out of range in erase with predicate");
  }
  size_t startIndex = start - CIter< T >(data_);
  size_t endIndex = end - CIter< T >(data_);
  size_t count = endIndex - startIndex;
  Vector< T > temp;
  for (size_t i = 0; i < startIndex; ++i)
  {
    temp.pushBack(data_[i]);
  }
  for (size_t i = startIndex; i < endIndex; ++i)
  {
    if (!c(data_[i]))
    {
      temp.pushBack(data_[i]);
    }
  }
  for (size_t i = endIndex; i < size_; ++i)
  {
    temp.pushBack(data_[i]);
  }
  swap(temp);
  return CIter< T > {data_ + startIndex};
}

template< class T >
void sedov::Vector< T >::reserve(size_t k)
{
  if (k <= capacity_)
  {
    return;
  }
  Vector< T > t = *this;
  t.grow(k);
  swap(t);
}

template< class T >
void sedov::Vector< T >::shrinkToFit()
{
  if (size_ < capacity_)
  {
    T * newData = static_cast< T * >(operator new(sizeof(T) * size_));
    size_t c = 0;
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        new (&newData[i]) T(std::move(data_[i]));
        ++c;
      }
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i].~T();
      }
      operator delete(data_);
      data_ = newData;
      capacity_ = size_;
    }
    catch(...)
    {
      for (size_t i = 0; i < c; ++i)
      {
        newData[i].~T();
      }
      operator delete(newData);
      throw;
    }
  }
}

template< class T >
void sedov::Vector< T >::pushBackCount(size_t k, const T& v)
{
  if (!k)
  {
    return;
  }
  Vector< T > t = *this;
  if (t.size_ + k > t.capacity_)
  {
    size_t newCap = (!t.capacity_) ? k : std::max(t.size_ + k, t.capacity_ * 2);
    t.grow(newCap);
  }
  for (size_t i = 0; i < k; ++i)
  {
    new (&t.data_[t.size_ + i]) T(v);
  }
  t.size_ += k;
  swap(t);
}

template< class T >
template< class IT >
void sedov::Vector< T >::pushBackRange(IT b, size_t c)
{
  if (!c)
  {
    return;
  }
  Vector< T > t = *this;
  if (t.size_ + c > t.capacity_)
  {
    size_t newCap = (t.capacity_ == 0) ? c : std::max(t.size_ + c, t.capacity_ * 2);
    t.grow(newCap);
  }
  for (size_t i = 0; i < c; ++i)
  {
    new (&t.data_[t.size_ + i]) T(std::move(*b));
    ++b;
  }
  t.size_ += c;
  swap(t);
}

template< class T >
void sedov::Vector< T >::unsafePushBack(const T & value)
{
  new(&data_[size_]) T(value);
  ++size_;
}

template< class T >
template< class P >
void sedov::Vector< T >::uniInsert(size_t i, P && v)
{
  if (i > size_)
  {
    throw std::out_of_range("Index is out of range");
  }
  size_t newCap = capacity_ * 2 + 1;
  if (size_ + 1 < capacity_)
  {
    newCap = capacity_;
  }
  T * newData = static_cast< T * >(operator new(sizeof(T) * newCap));
  size_t j = 0;
  try
  {
    for (; j < i; ++j)
    {
      new(&newData[j]) T(data_[j]);
    }
    new(&newData[i]) T(std::forward< P >(v));
    for (; j < size_; ++j)
    {
      new(&newData[j + 1]) T(data_[j]);
    }
  }
  catch (...)
  {
    for (size_t k = 0; k < j; ++k)
    {
      (&newData[k])->~T();
    }
    operator delete(newData);
    throw;
  }
  for (; j > 0; j--)
  {
    (&data_[j - 1])->~T();
  }
  operator delete(data_);
  data_ = newData;
  size_++;
  capacity_ = newCap;
}

template< class T >
bool sedov::operator==(const Vector< T > & lhs, const Vector< T > & rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

#endif
