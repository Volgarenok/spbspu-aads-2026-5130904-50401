#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <memory>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include "top-it-iters.hpp"

namespace kuznetsov {

  template< class T >
  struct Vector {
    Vector();
    ~Vector();
    explicit Vector(std::initializer_list< T > il);
    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    Vector(size_t size, const T& init);

    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&) noexcept;
    T& operator[](size_t i) noexcept;
    const T& operator[](size_t i) const noexcept;

    void swap(Vector< T >& rhs) noexcept;
    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void pushBack(const T& v);
    void pushBack(T&& v);

    void popBack();

    void insert(size_t pos, const T& v);
    void insert(size_t pos, T&& v);
    void insert(size_t pos, const Vector< T >& v, size_t start, size_t end);

    void insert(CIter< T > it, const T& v);
    void insert(CIter< T > it, T&& v);
    void insert(CIter< T > pos, CIter< T > start, CIter< T > end);

    void erase(size_t pos);
    void erase(size_t start, size_t count);

    CIter< T > erase(CIter< T > pos);
    CIter< T > erase(CIter< T > start, CIter< T > end);
    template< class C >
    CIter< T > erase(CIter< T > start, CIter< T > end, C c);

    void clear();
    bool contain(const T&) const noexcept;


    CIter< T > cbegin() const noexcept;
    CIter< T > cend() const noexcept;

    Iter< T > begin() noexcept;
    Iter< T > end() noexcept;

    CIter< T > begin() const noexcept;
    CIter< T > end() const noexcept;

    T& at(size_t pos);
    const T& at(size_t pos) const;

    void reserve(size_t required);
    void shrinkToFit();
    void pushBackRange(CIter< T > start, size_t c);
    void pushBackCount(size_t k, const T& val);
    void unsafePushBack(const T& val);

  private:
    explicit Vector(size_t c);
    template< class U>
    void generalInsert(size_t pos, U&& value);
    template< class U >
    void generalPushBack(U&& v);
    T* data_;
    size_t size_, cap_;
  };

  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
}

template< class T >
kuznetsov::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  cap_(0)
{
}

template< class T >
kuznetsov::Vector< T >::~Vector()
{
  for (size_t i = 0; i < size_; ++i) {
    (data_ + i)->~T();
  }
  ::operator delete[] (data_);
}

template< class T >
kuznetsov::Vector< T >::Vector(const Vector& other):
  Vector(other.size_)
{
  size_t i = 0;
  for (; i < other.size_; ++i) {
    new (data_ + i) T(other.data_[i]);
    ++size_;
  }
}

template< class T >
kuznetsov::Vector< T >::Vector(Vector&& o) noexcept :
  data_(o.data_),
  size_(o.size_),
  cap_(o.cap_)
{
  o.data_ = nullptr;
  o.size_ = 0;
  o.cap_ = 0;
}

template< class T >
kuznetsov::Vector< T >::Vector(size_t size, const T& init):
  Vector(size)
{
  for (size_t i = 0; i < size; ++i) {
    new (data_ + i) T(init);
    ++size_;
  }
}

template< class T >
kuznetsov::Vector< T >::Vector(size_t c):
  data_(c ? static_cast< T* >(::operator new[](sizeof(T) * c)) : nullptr),
  size_(0),
  cap_(c)
{}

template< class T >
kuznetsov::Vector<T>::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  for (auto it = il.begin(); it != il.end(); ++it) {
    new (data_ + size_)  T(*it);
    ++size_;
  }
}

template< class T >
kuznetsov::Vector< T >& kuznetsov::Vector< T >::operator=(const Vector& rhs)
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template< class T >
kuznetsov::Vector< T >& kuznetsov::Vector< T >::operator=(Vector&& o) noexcept
{
  if (this == std::addressof(o)) {
    return *this;
  }
  Vector< T > cpy(std::move(o));
  swap(cpy);
  return *this;
}

template< class T >
T& kuznetsov::Vector< T >::operator[](size_t i) noexcept
{
  const Vector* cthis = this;
  return const_cast< T& >((*cthis)[i]);
}

template< class T >
const T& kuznetsov::Vector< T >::operator[](size_t i) const noexcept
{
  return data_[i];
}

template< class T >
void kuznetsov::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(cap_, rhs.cap_);
}

template< class T >
bool kuznetsov::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t kuznetsov::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t kuznetsov::Vector< T >::getCapacity() const noexcept
{
  return cap_;
}

template< class T >
template< class U >
void kuznetsov::Vector< T >::generalPushBack(U&& v)
{
  size_t newCap = size_ < cap_ ? size_ : cap_ * 1.5 + 1;
  reserve(newCap);
  new (data_ + size_) T(std::forward< U >(v));
  ++size_;
}

template< class T >
void kuznetsov::Vector< T >::pushBack(const T& v)
{
  generalPushBack(v);
}

template< class T >
void kuznetsov::Vector< T >::pushBack(T&& v)
{
  generalPushBack(std::move(v));
}

template< class T >
void kuznetsov::Vector< T >::popBack()
{
  if (size_ > 0) {
    (data_ + size_ - 1)->~T();
    size_--;
  }
}

template< class T >
template< class U>
void kuznetsov::Vector< T >::generalInsert(size_t pos, U&& v)
{
  size_t newCap = cap_ * 1.5 + 1;
  if (size_ + 1 < cap_) {
    newCap = cap_;
  }
  T* newData = static_cast< T* >(::operator new[](sizeof(T) * newCap));
  size_t i = 0;
  try {
    for (; i < pos; ++i) {
      new (newData + i) T(data_[i]);
    }
    new (newData + pos) T(std::forward< U >(v));
    for (; i < size_; ++i) {
      new (newData + i + 1) T(data_[i]);
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      (newData + j)->~T();
    }
    ::operator delete[] (newData);
    throw;
  }
  for (; i > 0; i--) {
    (data_ + i - 1)->~T();
  }
  ::operator delete[] (data_);
  data_ = newData;
  size_++;
  cap_ = newCap;
}

template< class T >
void kuznetsov::Vector< T >::insert(size_t pos, const T& v)
{
  if (pos > size_) {
    throw std::out_of_range("position out of range");
  }
  generalInsert(pos, v);
}

template< class T >
void kuznetsov::Vector< T >::insert(size_t pos, T&& v)
{
  if (pos > size_) {
    throw std::out_of_range("position out of range");
  }
  generalInsert(pos, std::move(v));
}

template< class T >
void kuznetsov::Vector< T >::insert(CIter< T > pos, CIter< T > start, CIter< T > end)
{
  if (pos < CIter< T >(data_) || pos > CIter< T >(data_ + size_)) {
    throw std::out_of_range("iterator position out of range");
  }
  size_t index = pos - CIter< T >(data_);
  size_t count = end - start;
  if (count == 0) return;
  size_t newCap = cap_;
  if (size_ + count > cap_) {
    newCap = cap_ * 1.5 + count;
  }
  T* newData = static_cast< T* >(::operator new[] (sizeof(T) * newCap));
  size_t i = 0;
  try {
    for (; i < index; ++i) {
      new (newData + i) T(data_[i]);
    }
    for (auto it = start; it != end; ++it) {
      new (newData + i) T(*it);
      ++i;
    }
    for (size_t j = index; j < size_; ++j) {
      new (newData + i) T(data_[j]);
      ++i;
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      (newData + j)->~T();
    }
    ::operator delete[] (newData);
    throw;
  }
  for (size_t j = 0; j < size_; ++j) {
    (data_ + j)->~T();
  }
  ::operator delete[] (data_);
  data_ = newData;
  size_ += count;
  cap_ = newCap;
}

template< class T >
void kuznetsov::Vector< T >::insert(CIter< T > it, const T& v)
{
  if (it < CIter< T >(data_) || it > CIter< T >(data_ + size_)) {
    throw std::out_of_range("position out of range");
  }
  size_t pos = it - CIter< T >(data_);
  generalInsert(pos, v);
}

template< class T >
void kuznetsov::Vector< T >::insert(CIter< T > it, T&& v)
{
  if (it < CIter< T >(data_) || it > CIter< T >(data_ + size_)) {
    throw std::out_of_range("position out of range");
  }
  size_t pos = it - CIter< T >(data_);
  generalInsert(pos, std::move(v));
}

template< class T >
void kuznetsov::Vector< T >::insert(size_t pos, const Vector< T >& v, size_t start, size_t end)
{
  if (pos > size_) {
    throw std::out_of_range("position out of range in insert");
  }
  if (end > v.getSize()) {
    throw std::out_of_range("end of second vector out of range");
  }
  size_t count = (end - start);
  size_t newCap = cap_ + count;
  if (size_ + count < cap_) {
    newCap = cap_;
  }
  T* newData = static_cast< T* >(::operator new[] (sizeof(T) * newCap));
  size_t i = 0;
  size_t j = 0;
  try {
    for (; i < pos; ++i) {
      new (newData + i) T(data_[i]);
    }
    for (;j < count; ++j) {
      new (newData + i + j) T(v[start + j]);
    }
    for (; i < size_; ++i) {
      new (newData + i + j) T(data_[i]);
    }
  } catch (...) {
    for (size_t k = 0; k < i + j; ++k) {
      (newData + k)->~T();
    }
    ::operator delete[] (newData);
    throw;
  }
  for (i = 0; i < size_; ++i) {
    (data_ + i)->~T();
  }
  ::operator delete[] (data_);
  data_ = newData;
  size_ += count;
  cap_ = newCap;
}

template< class T >
void kuznetsov::Vector< T >::erase(size_t pos)
{
  if (pos >= size_) {
    throw std::out_of_range("pos out of range in erase");
  }
  if (pos == size_ - 1) {
    popBack();
    return;
  }
  T* copy = static_cast< T* >(::operator new[] (sizeof(T) * (size_ - 1)));
  size_t i = 0;
  try {
    for (; i < pos; ++i) {
      new (copy + i) T(data_[i]);
    }
    for (; i < size_ - 1; ++i) {
      new (copy + i) T(data_[i + 1]);
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      (copy + j)->~T();
    }
    ::operator delete[] (copy);
    throw;
  }
  for (i = 0; i < size_; ++i) {
    (data_ + i)->~T();
  }
  ::operator delete[] (data_);
  data_ = copy;
  size_--;
}

template< class T >
void kuznetsov::Vector< T >::erase(size_t start, size_t count)
{
  if (start >= size_) {
    throw std::out_of_range("start out of range in erase");
  }
  if (start == size_ - 1) {
    popBack();
    return;
  }
  if (start + count >= size_) {
    count = size_ - start;
  }
  Vector< int > copy(size_);
  size_t i = 0;
  for (; i < start; ++i) {
    copy.unsafePushBack((data_[i]));
  }
  for (; i < size_ - count; ++i) {
    copy.unsafePushBack((data_[i + count]));
  }
  swap(copy);
}

template< class T >
kuznetsov::CIter< T > kuznetsov::Vector< T >::erase(CIter< T > pos)
{
  if (pos < CIter< T >(data_) || pos >= CIter< T >(data_ + size_)) {
    throw std::out_of_range("position out of range in erase");
  }
  size_t i = pos - cbegin();
  erase(i);
  return CIter< T >{data_ + i};
}

template< class T >
kuznetsov::CIter< T > kuznetsov::Vector< T >::erase(CIter< T > start, CIter< T > end)
{
  if (start < CIter< T >(data_) || end > CIter< T >(data_ + size_) || start > end) {
    throw std::out_of_range("range out of range in erase");
  }
  size_t count = end - start;
  size_t pos = start - cbegin();
  erase(pos, count);
  return CIter< T >{data_ + pos};
}

template< class T >
template< class C >
kuznetsov::CIter< T > kuznetsov::Vector< T >::erase(CIter< T > start, CIter< T > end, C c)
{
  if (start < CIter< T >(data_) || end > CIter< T >(data_ + size_) || start > end) {
    throw std::out_of_range("out of range in erase with predicate");
  }
  size_t startIndex = start - CIter< T >(data_);
  size_t endIndex = end - CIter< T >(data_);
  Vector< T > temp = Vector< T >(size_);
  for (size_t i = 0; i < startIndex; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = startIndex; i < endIndex; ++i) {
    if (!c(data_[i])) {
      temp.pushBack(data_[i]);
    }
  }
  for (size_t i = endIndex; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
  return CIter< T > {data_ + startIndex};
}

template< class T >
void kuznetsov::Vector< T >::clear()
{
  for (size_t i = 0; i < size_; ++i) {
    (data_ + i)->~T();
  }
  size_ = 0;
}

template< class T >
bool kuznetsov::Vector< T >::contain(const T& obj) const noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    if (data_[i] == obj) {
      return true;
    }
  }
  return false;
}

template< class T >
kuznetsov::CIter< T > kuznetsov::Vector< T >::cbegin() const noexcept
{
  return CIter< T >(data_);
}

template< class T >
kuznetsov::CIter< T > kuznetsov::Vector< T >::cend() const noexcept
{
  return CIter< T >(data_ + size_);
}

template< class T >
kuznetsov::Iter< T > kuznetsov::Vector< T >::begin() noexcept
{
  return Iter< T >(data_);
}

template< class T >
kuznetsov::Iter< T > kuznetsov::Vector< T >::end() noexcept
{
  return Iter< T >(data_ + size_);
}

template< class T >
kuznetsov::CIter< T > kuznetsov::Vector< T >::begin() const noexcept
{
  return CIter< T >(data_);
}

template< class T >
kuznetsov::CIter< T > kuznetsov::Vector< T >::end() const noexcept
{
  return CIter< T >(data_ + size_);
}

template< class T >
T& kuznetsov::Vector< T >::at(size_t pos)
{
  const Vector< T >* cthis = this ;
  return  const_cast< T& >(cthis->at(pos));
}

template< class T >
const T& kuznetsov::Vector< T >::at(size_t pos) const
{
  if (pos >= size_) {
    throw std::out_of_range("Out of size array");
  }
  return (*this)[pos];
}


template< class T >
void kuznetsov::Vector< T >::pushBackCount(size_t k, const T& val)
{
  size_t newCap = size_ + k < cap_ ? cap_ : cap_ + cap_ / 2 + k;
  Vector< T > copy(newCap);
  size_t i = 0;
  for (; i < size_; ++i) {
    copy.unsafePushBack(data_[i]);
  }
  for (; i < size_ + k; ++i) {
    copy.unsafePushBack(val);
  }
  swap(copy);
}

template< class T >
void kuznetsov::Vector< T >::unsafePushBack(const T& val)
{
  new (data_ + size_) T(val);
  size_++;
}

template< class T >
void kuznetsov::Vector<T>::pushBackRange(CIter<T> start, size_t c)
{
  size_t newCap = size_ + c < cap_ ? cap_ : cap_ + cap_ / 2 + c;
  Vector< T > copy(newCap);
  size_t i = 0;
  for (size_t i = 0; i < size_; ++i) {
    copy.unsafePushBack(data_[i]);
  }
  for (i = 0; i < c; ++i) {
    copy.unsafePushBack(*start);
    ++start;
  }
  swap(copy);
}

template< class T >
bool kuznetsov::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{
  bool res = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; i < lhs.getSize(); ++i) {
    res = res && (lhs[i] == rhs[i]);
  }
  return res;
}

template < class T >
void kuznetsov::Vector<T>::reserve(size_t required)
{
  if (cap_ >= required) {
    return;
  }
  T* copy = static_cast< T* >(::operator new[] (sizeof(T) * required));
  size_t i = 0;
  try {
    for (; i < size_; ++i) {
      new (copy + i) T(data_[i]);
    }
  } catch (...) {
    for (; i > 0; --i) {
      (copy + i - 1)->~T();
    }
    ::operator delete[] (copy);
    throw;
  }
  for (i = 0; i < size_; ++i) {
    (data_ + i)->~T();
  }
  ::operator delete[] (data_);
  data_ = copy;
  cap_ = required;
}

template < class T >
void kuznetsov::Vector<T>::shrinkToFit()
{
  T* copy = static_cast< T* >(::operator new[] (sizeof(T) * size_));
  size_t i = 0;
  try {
    for (; i < size_; ++i) {
      new (copy + i) T(data_[i]);
    }
  } catch (...) {
    for (; i > 0; --i) {
      (copy + i - 1)->~T();
    }
    ::operator delete[] (copy);
    throw;
  }
  for (i = 0; i < size_; ++i) {
    (data_ + i)->~T();
  }
  ::operator delete[] (data_);
  data_ = copy;
  cap_ = size_;
}

#endif


