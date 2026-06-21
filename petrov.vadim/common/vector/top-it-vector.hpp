#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP

#include "viter.hpp"
#include "vciter.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <memory>

namespace topit
{
  template< class T >
  struct Vector
  {
    Vector();
    ~Vector();
    explicit Vector(size_t size);
    Vector(const Vector &);
    Vector(Vector &&) noexcept;
    Vector(size_t size, const T & init);
    explicit Vector(std::initializer_list< T > il);
    Vector< T > & operator=(const Vector< T > &);
    Vector< T > & operator=(Vector< T > &&);

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    VIter< T > begin() noexcept;
    VIter< T > end() noexcept;
    VCIter< T > begin() const noexcept;
    VCIter< T > end() const noexcept;

    T & operator[](size_t id) noexcept;
    const T & operator[](size_t id) const noexcept;
    T & at(size_t id);
    const T & at(size_t id) const;

    void reserve(size_t required);
    void shrinkToFit();
    void pushBackCount(size_t k, const T& v);
    template< class IT >
    void pushBackRange(IT b, size_t c);

    void pushBack(const T & v);
    void popBack();
    void insert(size_t i, const T & v);
    void insert(size_t i, const Vector< T > & rhs, size_t start, size_t end);
    void erase(size_t i);
    void erase(size_t start, size_t end);
    void swap(Vector< T > & rhs) noexcept;

    VIter< T > insert(VIter< T > pos, const T & val);
    VIter< T > insert(VIter< T > pos, size_t count, const T & val);
    VIter< T > insert(VIter< T > pos, VCIter< T > first, VCIter< T > last);

    VIter< T > erase(VIter< T > pos);
    VIter< T > erase(VIter< T > first, VIter< T > last);
    VIter< T > erase(VIter< T > pos, size_t count);

    void destroyRange(T * ptr, size_t from, size_t to) noexcept;
    T * allocateRow(size_t size);
    void deallocateRow(T * ptr) noexcept;

    private:
      T * data_;
      size_t size_;
      size_t capacity_;
      void unsafePushBack(const T & val);
  };
  template< class T >
  bool operator==(const Vector< T > & lhs, const Vector< T > & rhs);
}

template< class T >
topit::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t constructed = 0;
  try
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      new (data_ + constructed++) T(*it);
    }
    size_ = constructed;
  }
  catch(...)
  {
    destroyRange(data_, 0, constructed);
    deallocateRow(data_);
    throw;
  }
}

template< class T >
topit::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
topit::Vector< T >::~Vector()
{
  destroyRange(data_, 0, size_);
  deallocateRow(data_);
}

template< class T >
topit::Vector< T >::Vector(const Vector< T > & rhs):
  Vector(rhs.getSize())
{
  size_t constructed = 0;
  try
  {
    for (; constructed < rhs.size_; ++constructed)
    {
      new (data_ + constructed) T(rhs[constructed]);
    }
    size_ = constructed;
  }
  catch(...)
  {
    destroyRange(data_, 0, constructed);
    deallocateRow(data_);
    throw;
  }
}

template< class T >
topit::Vector< T >::Vector(Vector< T > && rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template< class T >
topit::Vector< T >::Vector(size_t size, const T & init):
  Vector(size)
{
  size_t constructed = 0;
  try
  {
    for (; constructed < size; ++constructed)
    {
      new (data_ + constructed) T(init);
    }
    size_ = constructed;
  }
  catch(...)
  {
    destroyRange(data_, 0, constructed);
    deallocateRow(data_);
    throw;
  }
}

template< class T >
topit::Vector< T > & topit::Vector< T >::operator=(const Vector< T > & rhs)
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
topit::Vector< T > & topit::Vector< T >::operator=(Vector< T > && rhs)
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
bool topit::Vector< T >::isEmpty() const noexcept
{
  return !size_;
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
topit::VIter< T > topit::Vector< T >::begin() noexcept
{
  return VIter< T >(data_);
}

template< class T >
topit::VIter< T > topit::Vector< T >::end() noexcept
{
  return VIter< T >(data_ + size_);
}

template< class T >
topit::VCIter< T > topit::Vector< T >::begin() const noexcept
{
  return VCIter< T >(data_);
}

template< class T >
topit::VCIter< T > topit::Vector< T >::end() const noexcept
{
  return VCIter< T >(data_ + size_);
}

template< class T >
T & topit::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T > * cthis = this;
  return const_cast< T & >((*cthis)[id]);
}

template< class T >
const T & topit::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T & topit::Vector< T >::at(size_t id)
{
  const Vector< T > * cthis = this;
  const T & ret = cthis->at(id);
  return const_cast< T & >(ret);
}

template< class T >
const T & topit::Vector< T >::at(size_t id) const
{
  if (id < getSize())
  {
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
}

template< class T >
void topit::Vector< T >::reserve(size_t required)
{
  if (required <= capacity_)
  {
    return;
  }

  T * newData = allocateRow(required);
  size_t constructed = 0;
  try
  {
    for (; constructed < size_; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = required;
}

template< class T >
void topit::Vector< T >::shrinkToFit()
{
  if (size_ == capacity_)
  {
    return;
  }

  T * newData = allocateRow(size_);
  size_t constructed = 0;
  try
  {
    for (; constructed < size_; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = size_;
}

template< class T >
void topit::Vector< T >::pushBackCount(size_t k, const T & val)
{
  if (k == 0)
  {
    return;
  }

  const size_t newSize = size_ + k;
  const size_t newCapacity = (newSize > capacity_) ? newSize : capacity_;

  T * newData = allocateRow(newCapacity);
  size_t constructed = 0;
  try
  {
    for (; constructed < size_; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (; constructed < newSize; ++constructed)
    {
      new (newData + constructed) T(val);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  size_ = newSize;
  capacity_ = newCapacity;
}

template< class T >
template< class IT >
void topit::Vector< T >::pushBackRange(IT b, size_t c)
{
  if (c == 0)
  {
    return;
  }

  const size_t newSize = size_ + c;
  const size_t newCapacity = (newSize > capacity_) ? newSize : capacity_;

  T * newData = allocateRow(newCapacity);
  size_t constructed = 0;
  IT it = b;
  try
  {
    for (; constructed < size_; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (; constructed < newSize; ++constructed, ++it)
    {
      new (newData + constructed) T(*it);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  size_ = newSize;
  capacity_ = newCapacity;
}

template< class T >
void topit::Vector< T >::pushBack(const T & v)
{
  if (size_ < capacity_)
  {
    new (data_ + size_) T(v);
    ++size_;
    return;
  }

  size_t newCapacity = capacity_ + 1;
  T * newData = allocateRow(newCapacity);
  size_t i = 0;
  try
  {
    for (; i < size_; ++i)
    {
      new (newData + i) T(data_[i]);
    }
    new (newData + i) T(v);
  }
  catch(...)
  {
    destroyRange(newData, 0, i);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = newCapacity;
  ++size_;
}

template< class T >
void topit::Vector< T >::popBack()
{
  if (size_ > 0)
  {
    data_[--size_].~T();
  }
}

template< class T >
void topit::Vector< T >::insert(size_t i, const T & v)
{
  if (i > size_)
  {
    throw std::out_of_range("bad id");
  }

  const size_t newCapacity = (size_ < capacity_) ? capacity_ : (capacity_ + 1);
  T * newData = allocateRow(newCapacity);
  size_t constructed = 0;
  try
  {
    for (; constructed < i; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    new (newData + i) T(v);
    ++constructed;

    for (size_t k = i; k < size_; ++k, ++constructed)
    {
      new (newData + constructed) T(data_[k]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = newCapacity;
  ++size_;
}

template< class T >
void topit::Vector< T >::insert(size_t i, const Vector< T > & rhs, size_t start, size_t end)
{
  if (i > size_ || start > end || end > rhs.getSize())
  {
    throw std::out_of_range("bad id");
  }

  if ((end - start) == 0)
  {
    return;
  }

  const size_t newSize = size_ + end - start;
  const size_t newCapacity = (newSize > capacity_) ? newSize : capacity_;
  T * newData = allocateRow(newCapacity);
  size_t constructed = 0;

  try
  {
    for (; constructed < i; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (size_t j = start; j < end; ++ j)
    {
      new (newData + constructed++) T(rhs[j]);
    }

    for (size_t j = i; j < size_; ++j)
    {
      new (newData + constructed++) T(data_[j]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = newCapacity;
  size_ = newSize;
}

template< class T >
void topit::Vector< T >::erase(size_t i)
{
  if (i >= size_)
  {
    throw std::out_of_range("bad id");
  }

  T * newData = allocateRow(capacity_);
  size_t constructed = 0;
  try
  {
    for (; constructed < i; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }
    for (; constructed < size_ - 1; ++constructed)
    {
      new (newData + constructed) T(data_[constructed + 1]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  --size_;
}

template< class T >
void topit::Vector< T >::erase(size_t start, size_t end)
{
  if (end > size_ || start > end)
  {
    throw std::out_of_range("bad id");
  }

  size_t newSize = size_ - end + start;
  T * newData = allocateRow(capacity_);
  size_t constructed = 0;
  try
  {
    for (; constructed < start; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (; constructed < newSize; ++constructed)
    {
      new (newData + constructed) T(data_[constructed + end - start]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  size_ = newSize;
}

template< class T >
void topit::Vector< T >::swap(Vector< T > & rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
topit::VIter< T > topit::Vector< T >::insert(VIter< T > pos, const T & val)
{
  if (pos < begin() || pos > end())
  {
    throw std::out_of_range("bad iterator");
  }

  size_t index = pos - begin();

  const size_t newCapacity = (size_ < capacity_) ? capacity_ : (capacity_ + 1);
  T * newData = allocateRow(newCapacity);
  size_t constructed = 0;
  try
  {
    for (; constructed < index; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    new (newData + constructed) T(val);
    ++constructed;

    for (size_t k = index; k < size_; ++k, ++constructed)
    {
      new (newData + constructed) T(data_[k]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = newCapacity;
  ++size_;
  return VIter< T >(data_ + index);
}

template< class T >
topit::VIter< T > topit::Vector< T >::insert(VIter< T > pos, size_t count, const T & val)
{
  if (pos < begin() || pos > end())
  {
    throw std::out_of_range("bad iterator");
  }

  if (count == 0)
  {
    return pos;
  }

  size_t index = pos - begin();

  const size_t newCapacity = (size_ + count < capacity_) ? capacity_ : (size_ + count);
  T * newData = allocateRow(newCapacity);
  size_t constructed = 0;
  try
  {
    for (; constructed < index; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (; constructed < index + count; ++constructed)
    {
      new (newData + constructed) T(val);
    }

    for (size_t k = index; k < size_; ++k, ++constructed)
    {
      new (newData + constructed) T(data_[k]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = newCapacity;
  size_ += count;

  return VIter< T >(data_ + index);
}

template< class T >
topit::VIter< T > topit::Vector< T >::insert(VIter< T > pos, VCIter< T > first, VCIter< T > last)
{
  if (pos < begin() || pos > end() || last < first)
  {
    throw std::out_of_range("bad iterator");
  }

  if (first == last)
  {
    return pos;
  }

  size_t index = pos - begin();

  const size_t lf = last-first;
  const size_t newCapacity = (size_ + lf < capacity_) ? capacity_ : size_ + lf;
  T * newData = allocateRow(newCapacity);
  size_t constructed = 0;
  try
  {
    for (; constructed < index; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    VCIter< T > it = first;
    while (it != last)
    {
      new (newData + constructed) T(*it);
      ++constructed;
      ++it;
    }

    for (size_t k = index; k < size_; ++k, ++constructed)
    {
      new (newData + constructed) T(data_[k]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  capacity_ = newCapacity;
  size_ += lf;

  return VIter< T >(data_ + index);
}

template< class T >
topit::VIter< T > topit::Vector< T >::erase(VIter< T > pos)
{
  if (pos < begin() || pos >= end())
  {
    throw std::out_of_range("bad id");
  }

  const size_t index = pos - begin();

  T * newData = allocateRow(capacity_);
  size_t constructed = 0;
  try
  {
    for (; constructed < index; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (size_t k = index + 1; k < size_; ++k, ++constructed)
    {
      new (newData + constructed) T(data_[k]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  --size_;

  if (size_ == 0)
  {
    return end();
  }

  return VIter< T >(data_ + index);
}

template< class T >
topit::VIter< T > topit::Vector< T >::erase(VIter< T > first, VIter< T > last)
{
  if (first < begin() || last > end() || last < first)
  {
    throw std::out_of_range("bad id");
  }

  const size_t index = first - begin();

  if (first == last)
  {
    return VIter< T >(data_ + index);
  }

  const size_t lf = last - first;

  T * newData = allocateRow(capacity_);
  size_t constructed = 0;
  try
  {
    for (; constructed < index; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (size_t k = index + lf; k < size_; ++k, ++constructed)
    {
      new (newData + constructed) T(data_[k]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  size_ -= lf;

  if (size_ == 0)
  {
    return end();
  }

  return VIter< T >(data_ + index);
}

template< class T >
topit::VIter< T > topit::Vector< T >::erase(VIter< T > pos, size_t count)
{
  const size_t index = pos - begin();

  if (pos < begin() || pos >= end() || index + count > size_)
  {
    throw std::out_of_range("bad id");
  }

  T * newData = allocateRow(capacity_);
  size_t constructed = 0;
  try
  {
    for (; constructed < index; ++constructed)
    {
      new (newData + constructed) T(data_[constructed]);
    }

    for (size_t k = index + count; k < size_; ++k, ++constructed)
    {
      new (newData + constructed) T(data_[k]);
    }
  }
  catch(...)
  {
    destroyRange(newData, 0, constructed);
    deallocateRow(newData);
    throw;
  }

  destroyRange(data_, 0, size_);
  deallocateRow(data_);
  data_ = newData;
  size_ -= count;

  if (size_ == 0)
  {
    return end();
  }

  return VIter< T >(data_ + index);
}

template< class T >
void topit::Vector< T >::destroyRange(T * ptr, size_t from, size_t to) noexcept
{
  for (size_t i = from; i < to; ++i)
  {
    ptr[i].~T();
  }
}

template< class T >
T * topit::Vector< T >::allocateRow(size_t size)
{
  if (size == 0)
  {
    return nullptr;
  }
  return static_cast< T * >(::operator new(sizeof(T) * size));
}

template< class T >
void topit::Vector< T >::deallocateRow(T * ptr) noexcept
{
  ::operator delete(ptr);
}

template< class T >
topit::Vector< T >::Vector(size_t size):
  data_(size ? allocateRow(size) : nullptr),
  size_(0),
  capacity_(size)
{}

template< class T >
bool topit::operator==(const topit::Vector< T > & lhs, const topit::Vector< T > & rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

template< class T >
void topit::Vector< T >::unsafePushBack(const T & val)
{
  new (data_ + size_) T(val);
  ++size_;
}

#endif
