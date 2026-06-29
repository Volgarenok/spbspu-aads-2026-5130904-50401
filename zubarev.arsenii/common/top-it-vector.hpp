#ifndef TOP_IT_VECTOR
#define TOP_IT_VECTOR
#include "vector-citer.hpp"
#include "vector-iter.hpp"
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <limits>

namespace zubarev
{
  template< class T >
  struct Vector
  {
    ~Vector();
    Vector();
    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    explicit Vector(std::initializer_list< T > il);
    explicit Vector(size_t size, const T& init);
    Vector< T >& operator=(const Vector< T >&);
    Vector< T >& operator=(Vector< T >&&) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void reserve(size_t);
    void shrinkToFit();

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;
    void swap(Vector< T >& rhs) noexcept;
    bool contains(const T& value) const;
    VectIter< T > begin();
    VectIter< T > end();

    CVectIter< T > begin() const;
    CVectIter< T > end() const;

    CVectIter< T > cbegin() const;
    CVectIter< T > cend() const;

    void pushBack(const T& v);
    void pushBackCount(size_t k, const T& v);
    template< class IT >
    void pushBackRange(IT a, size_t count);
    void unsafePushBack(const T& v);

    void popBack();

    void insert(size_t i, const T& v);
    void insert(size_t pos, const Vector< T >& v, size_t beg, size_t end);

    void erase(size_t i);
    void erase(size_t beg, size_t end);

    VectIter< T > insert(CVectIter< T > pos, const T& v);
    VectIter< T > insert(CVectIter< T > pos, size_t count, const T& v);
    VectIter< T > insert(CVectIter< T > pos, VectIter< T > first, VectIter< T > last);

    void erase(CVectIter< T > pos);
    void erase(CVectIter< T > pos, size_t count);
    void erase(CVectIter< T > first, CVectIter< T > last);
    void erase(VectIter< T > first, VectIter< T > last);

  private:
    explicit Vector(size_t size);
    size_t calcCap(size_t curr, size_t needed);
    T* allocate(size_t n);
    void deallocate(T* ptr);
    void destroy_range(T* ptr, size_t first, size_t last);
    T* data_;
    size_t size_, capacity_;
  };
  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
}

template< class T >
zubarev::Vector< T >::~Vector()
{
  destroy_range(data_, 0, size_);
  deallocate(data_);
}

template< class T >
zubarev::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
zubarev::Vector< T >::Vector(const Vector& rhs):
  Vector(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); ++i) {
    new (data_ + i) T(rhs[i]);
  }
}

template< class T >
zubarev::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template< class T >
zubarev::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t i = 0;

  for (auto it = il.begin(); it != il.end(); ++it) {
    new (data_ + i) T(*it);
    i++;
  }
}

template< class T >
zubarev::Vector< T >::Vector(size_t size, const T& init):

  size_(size),
  capacity_(size)
{
  data_ = allocate(size_);
  size_t i = 0;
  try {
    for (; i < size_; ++i) {
      new (data_ + i) T(init);
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[j].~T();
    }
    throw;
  }
}

template< class T >
zubarev::Vector< T >::Vector(size_t size):
  data_(size ? allocate(size) : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
zubarev::Vector< T >& zubarev::Vector< T >::operator=(const Vector< T >& rhs)
{
  if (this == std::addressof(rhs)) {
    return *this;
  }

  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template< class T >
zubarev::Vector< T >& zubarev::Vector< T >::operator=(Vector< T >&& rhs) noexcept
{
  if (this == std::addressof(rhs)) {
    return *this;
  }

  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
bool zubarev::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t zubarev::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t zubarev::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void zubarev::Vector< T >::reserve(size_t n)
{
  if (n > capacity_) {

    T* newData = allocate(n);

    size_t i = 0;
    try {
      for (; i < size_; ++i) {
        new (newData + i) T(data_[i]);
      }
    } catch (...) {
      destroy_range(newData, 0, size_);
      deallocate(newData);
      throw;
    }

    destroy_range(data_, 0, size_);
    deallocate(data_);

    data_ = newData;
    capacity_ = n;
  } else {
    return;
  }
}

template< class T >
void zubarev::Vector< T >::shrinkToFit()
{
  if (size_ < capacity_) {
    T* newData = allocate(size_);

    size_t i = 0;
    try {
      for (; i < size_; ++i) {
        new (newData + i) T(data_[i]);
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        newData[j].~T();
      }
      deallocate(newData);
      throw;
    }

    destroy_range(data_, 0, size_);
    deallocate(data_);

    data_ = newData;
    capacity_ = size_;
  }
}

template< class T >
T* zubarev::Vector< T >::allocate(size_t n)
{
  if (n > std::numeric_limits< size_t >::max() / sizeof(T)) {
    throw std::bad_alloc();
  }
  return static_cast< T* >(::operator new(sizeof(T) * n));
}

template< class T >
void zubarev::Vector< T >::deallocate(T* ptr)
{
  ::operator delete(ptr);
}

template< class T >
void zubarev::Vector< T >::destroy_range(T* ptr, size_t first, size_t last)
{
  for (size_t j = first; j < last; ++j) {
    ptr[j].~T();
  }
}

template< class T >
size_t zubarev::Vector< T >::calcCap(size_t curr, size_t needed)
{
  if (curr == 0) {
    return 1;
  }
  while (curr <= needed) {
    if (curr > std::numeric_limits< size_t >::max() / 2) {
      throw std::overflow_error("overflow empty capacity");
    }
    curr *= 2;
  }
  return curr;
}

template< class T >
T& zubarev::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T >* cthis = this;

  return const_cast< T& >((*cthis)[id]);
}

template< class T >
const T& zubarev::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T& zubarev::Vector< T >::at(size_t id)
{
  const Vector< T >* cthis = this;
  return const_cast< T& >(cthis->at(id));
}

template< class T >
const T& zubarev::Vector< T >::at(size_t id) const
{
  if (id < getSize()) {
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
}

template< class T >
void zubarev::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
zubarev::VectIter< T > zubarev::Vector< T >::begin()
{
  return VectIter< T >(data_);
}

template< class T >
zubarev::VectIter< T > zubarev::Vector< T >::end()
{
  return VectIter< T >(data_ + size_);
}

template< class T >
zubarev::CVectIter< T > zubarev::Vector< T >::begin() const
{
  return CVectIter< T >(data_);
}

template< class T >
zubarev::CVectIter< T > zubarev::Vector< T >::end() const
{
  return CVectIter< T >(data_ + size_);
}

template< class T >
zubarev::CVectIter< T > zubarev::Vector< T >::cbegin() const
{
  return CVectIter< T >(data_);
}
template< class T >
zubarev::CVectIter< T > zubarev::Vector< T >::cend() const
{
  return CVectIter< T >(data_ + size_);
}

template< class T >
void zubarev::Vector< T >::pushBack(const T& v)
{
  if (size_ >= capacity_) {
    Vector< T > tmp(*this);
    tmp.reserve(calcCap(tmp.capacity_, size_ + 1));

    tmp.unsafePushBack(v);
    swap(tmp);
  } else {
    unsafePushBack(v);
  }
}

template< class T >
void zubarev::Vector< T >::pushBackCount(size_t count, const T& v)
{
  if (size_ + count >= capacity_) {
    Vector< T > tmp(*this);
    tmp.reserve(calcCap(tmp.capacity_, size_ + count));
    for (size_t i = 0; i < count; ++i) {
      tmp.unsafePushBack(v);
    }

    swap(tmp);
  } else {
    for (size_t i = 0; i < count; ++i) {
      unsafePushBack(v);
    }
  }
}

template< class T >
template< class IT >
void zubarev::Vector< T >::pushBackRange(IT a, size_t count)
{

  if (size_ + count >= capacity_) {
    Vector< T > tmp(*this);
    tmp.reserve(calcCap(tmp.capacity_, size_ + count));

    for (size_t i = 0; i < count; ++i) {
      tmp.unsafePushBack(*(a + i));
    }
    swap(tmp);
  } else {
    for (size_t i = 0; i < count; ++i) {
      unsafePushBack(*(a + i));
    }
  }
}

template< class T >
void zubarev::Vector< T >::unsafePushBack(const T& v)
{
  assert(size_ < capacity_);
  new (data_ + size_) T(v);
  ++size_;
}

template< class T >
void zubarev::Vector< T >::popBack()
{
  if (size_ == 0) {
    throw std::out_of_range("popBack to empty vector");
  }

  size_--;
  data_[size_].~T();
}

template< class T >
void zubarev::Vector< T >::insert(size_t index, const T& v)
{
  Vector< T > tmp;
  tmp.reserve(size_ + 1);
  size_t i = 0;
  size_t made = 0;
  try {
    for (; i < index; ++i) {
      new (tmp.data_ + i) T(data_[i]);
      ++made;
    }
    new (tmp.data_ + i) T(v);
    ++i;
    ++made;
    for (size_t j = index; j < size_; ++i, ++j) {
      new (tmp.data_ + i) T(data_[j]);
      ++made;
    }
    tmp.size_ = size_ + 1;
    swap(tmp);
  } catch (...) {
    destroy_range(tmp.data_, 0, made);
    throw;
  }
}

template< class T >
void zubarev::Vector< T >::insert(size_t pos, const Vector< T >& v, size_t beg, size_t end)
{
  size_t count = end - beg;
  Vector< T > tmp;
  tmp.reserve(size_ + count);
  size_t i = 0;
  size_t made = 0;
  try {
    for (; i < pos; ++i) {
      new (tmp.data_ + i) T(data_[i]);
      ++made;
    }
    for (size_t j = beg; j < end; ++j, ++i) {
      new (tmp.data_ + i) T(v.data_[j]);
      ++made;
    }

    for (size_t j = pos; j < size_; ++i, ++j) {
      new (tmp.data_ + i) T(data_[j]);
      ++made;
    }
    tmp.size_ = size_ + count;
    swap(tmp);
  } catch (...) {
    destroy_range(tmp.data_, 0, made);
    throw;
  }
}

template< class T >
void zubarev::Vector< T >::erase(size_t index)
{
  for (size_t i = index; i < size_ - 1; ++i) {
    data_[i].~T();
    new (data_ + i) T(std::move(data_[i + 1]));
  }
  data_[size_ - 1].~T();
  --size_;
}

template< class T >
void zubarev::Vector< T >::erase(size_t beg, size_t end)
{
  size_t count = end - beg;
  for (size_t i = end; i < size_; ++i) {
    data_[i - count].~T();
    new (data_ + i - count) T(std::move(data_[i]));
  }
  destroy_range(data_, size_ - count, size_);

  size_ -= count;
}

template< class T >
zubarev::VectIter< T > zubarev::Vector< T >::insert(CVectIter< T > pos, const T& v)
{
  size_t index = pos - cbegin();
  insert(index, v);
  return begin() + index;
}

template< class T >
zubarev::VectIter< T > zubarev::Vector< T >::insert(CVectIter< T > pos, size_t count, const T& v)
{
  size_t index = pos - cbegin();
  Vector< T > tmp;
  tmp.reserve(size_ + count);
  size_t i = 0;
  size_t made = 0;
  try {
    for (; i < index; ++i) {
      new (tmp.data_ + i) T(data_[i]);
      ++made;
    }
    for (size_t j = 0; j < count; ++j, ++i) {
      new (tmp.data_ + i) T(v);
      ++made;
    }

    for (size_t j = index; j < size_; ++i, ++j) {
      new (tmp.data_ + i) T(data_[j]);
      ++made;
    }
    tmp.size_ = size_ + count;
    swap(tmp);
    return begin() + index;
  } catch (...) {
    destroy_range(tmp.data_, 0, made);
    throw;
  }
}

template< class T >
zubarev::VectIter< T > zubarev::Vector< T >::insert(CVectIter< T > pos, VectIter< T > first, VectIter< T > last)
{
  size_t index = pos - begin();
  size_t count = last - first;

  Vector< T > tmp;
  tmp.reserve(size_ + count);
  size_t i = 0;
  size_t made = 0;
  try {
    for (; i < index; ++i) {
      new (tmp.data_ + i) T(data_[i]);
      ++made;
    }
    for (auto it = first; it < last; ++it, ++i) {
      new (tmp.data_ + i) T(*it);
      ++made;
    }
    for (size_t j = index; j < size_; ++i, ++j) {
      new (tmp.data_ + i) T(data_[j]);
      ++made;
    }

    tmp.size_ = size_ + count;
    swap(tmp);
    return begin() + index;
  } catch (...) {
    destroy_range(tmp.data_, 0, made);
    throw;
  }
}

template< class T >
void zubarev::Vector< T >::erase(CVectIter< T > pos)
{
  size_t index = pos - cbegin();
  erase(index);
}

template< class T >
void zubarev::Vector< T >::erase(CVectIter< T > pos, size_t count)
{
  size_t b = pos - cbegin();
  size_t e = b + count;
  erase(b, e);
}

template< class T >
void zubarev::Vector< T >::erase(CVectIter< T > first, CVectIter< T > last)
{
  size_t b = first - cbegin();
  size_t e = last - cbegin();
  erase(b, e);
}

template< class T >
bool zubarev::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{

  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i)
    ;
  return isEqual;
}
template< class T >
bool zubarev::Vector< T >::contains(const T& value) const
{
  for (auto it = cbegin(); it != cend(); ++it) {
    if (*it == value) {
      return true;
    }
  }
  return false;
}

#endif
