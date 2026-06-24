#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <utility>
#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include "viter.hpp"

namespace burukov
{
  template<class T>
  struct Vector
  {
    ~Vector();
    Vector();
    Vector(const Vector<T> &);
    Vector(Vector<T> &&) noexcept;
    Vector(size_t size, const T &init);
    explicit Vector(std::initializer_list<T> il);
    Vector<T> &operator=(const Vector<T> &);
    Vector<T> &operator=(Vector<T> &&) noexcept;

    void swap(Vector<T> &other) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    T &operator[](size_t index) noexcept;
    const T &operator[](size_t index) const noexcept;
    T &at(size_t index);
    const T &at(size_t index) const;

    void pushBack(const T &value);
    void popBack();

    void insert(size_t index, const T &value);
    void insert(size_t index, const Vector<T> &source, size_t start, size_t end);
    void erase(size_t index);
    void erase(size_t start, size_t count);

    CIter<T> cbegin() const noexcept;
    CIter<T> cend() const noexcept;
    Iter<T> begin() noexcept;
    Iter<T> end() noexcept;
    CIter<T> begin() const noexcept;
    CIter<T> end() const noexcept;

    void insert(CIter<T> it, const T &value);
    void insert(CIter<T> it, T &&value);
    void insert(CIter<T> it, CIter<T> start, CIter<T> end);

    CIter<T> erase(CIter<T> it);
    CIter<T> erase(CIter<T> start, CIter<T> end);
    template<class C>
    CIter<T> erase(CIter<T> start, CIter<T> end, C condition);

    void reserve(size_t capacity);
    void shrinkToFit();
    void pushBackCount(size_t count, const T &value);
    template<class IT>
    void pushBackRange(IT begin, size_t count);
    void unsafePushBack(const T &value);

  private:
    T *data_;
    size_t size_, capacity_;
    explicit Vector(size_t size);
    void grow(size_t new_capacity);
    template<class P>
    void insertAt(size_t position, P &&value);
  };

  template<class T>
  bool operator==(const Vector<T> &lhs, const Vector<T> &rhs);
}

template<class T>
burukov::Vector<T>::~Vector()
{
  for (size_t i = 0; i < size_; ++i)
    data_[i].~T();
  size_ = 0;
  operator delete(data_);
}

template<class T>
burukov::Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

template<class T>
burukov::Vector<T>::Vector(const Vector<T> &other) : Vector(other.getCapacity())
{
  size_t i = 0;
  for (; i < other.getSize(); ++i)
    new(&data_[i]) T(other.data_[i]);
  size_ = other.size_;
}

template<class T>
burukov::Vector<T>::Vector(Vector<T> &&other) noexcept
  : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template<class T>
burukov::Vector<T>::Vector(size_t size)
  : data_(size ? static_cast<T *>(operator new(sizeof(T) * size)) : nullptr),
    size_(size), capacity_(size) {}

template<class T>
burukov::Vector<T>::Vector(size_t size, const T &init) : Vector(size)
{
  for (size_t i = 0; i < size; ++i)
    new(&data_[i]) T(init);
}

template<class T>
burukov::Vector<T>::Vector(std::initializer_list<T> il) : Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it)
    new(&data_[i++]) T(*it);
}

template<class T>
burukov::Vector<T> &burukov::Vector<T>::operator=(const Vector<T> &other)
{
  if (this == std::addressof(other))
    return *this;
  Vector<T> copy = other;
  swap(copy);
  return *this;
}

template<class T>
burukov::Vector<T> &burukov::Vector<T>::operator=(Vector<T> &&other) noexcept
{
  if (this == std::addressof(other))
    return *this;
  Vector<T> copy(std::move(other));
  swap(copy);
  return *this;
}

template<class T>
void burukov::Vector<T>::swap(Vector<T> &other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template<class T>
bool burukov::Vector<T>::isEmpty() const noexcept
{
  return size_ == 0;
}

template<class T>
size_t burukov::Vector<T>::getSize() const noexcept
{
  return size_;
}

template<class T>
size_t burukov::Vector<T>::getCapacity() const noexcept
{
  return capacity_;
}

template<class T>
T &burukov::Vector<T>::operator[](size_t index) noexcept
{
  const Vector<T> *const_this = this;
  return const_cast<T &>((*const_this)[index]);
}

template<class T>
const T &burukov::Vector<T>::operator[](size_t index) const noexcept
{
  return data_[index];
}

template<class T>
T &burukov::Vector<T>::at(size_t index)
{
  const Vector<T> *const_this = this;
  return const_cast<T &>(const_this->at(index));
}

template<class T>
const T &burukov::Vector<T>::at(size_t index) const
{
  if (index < getSize())
    return (*this)[index];
  throw std::out_of_range("bad index");
}

template<class T>
void burukov::Vector<T>::grow(size_t new_capacity)
{
  T *new_data = static_cast<T *>(operator new(sizeof(T) * new_capacity));
  size_t constructed = 0;
  try
  {
    for (size_t i = 0; i < size_; ++i)
    {
      new(&new_data[i]) T(std::move(data_[i]));
      ++constructed;
    }
    for (size_t i = 0; i < size_; ++i)
      data_[i].~T();
    operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
  }
  catch(...)
  {
    for (size_t i = 0; i < constructed; ++i)
      new_data[i].~T();
    operator delete(new_data);
    throw;
  }
}

template<class T>
void burukov::Vector<T>::pushBack(const T &value)
{
  Vector<T> temporary = *this;
  if (temporary.size_ == temporary.capacity_)
  {
    size_t new_capacity = (temporary.capacity_ == 0) ? 1 : temporary.capacity_ * 2;
    temporary.grow(new_capacity);
  }
  new(&temporary.data_[temporary.size_]) T(value);
  ++temporary.size_;
  swap(temporary);
}

template<class T>
void burukov::Vector<T>::popBack()
{
  if (size_)
  {
    --size_;
    data_[size_].~T();
  }
}

template<class T>
void burukov::Vector<T>::insert(size_t index, const T &value)
{
  if (index > size_)
    throw std::out_of_range("Index out of range");
  insertAt(index, value);
}

template<class T>
void burukov::Vector<T>::insert(size_t index, const Vector<T> &source,
                                 size_t start, size_t end)
{
  if (index > size_)
    throw std::out_of_range("Index out of range");
  if (end > source.getSize())
    throw std::out_of_range("End of vector out of range");
  size_t count = end - start;
  size_t new_capacity = capacity_ + count;
  if (size_ + count < capacity_)
    new_capacity = capacity_;
  T *new_data = static_cast<T *>(operator new(sizeof(T) * new_capacity));
  size_t i = 0, k = 0;
  try
  {
    for (; i < index; ++i)
      new(&new_data[i]) T(data_[i]);
    for (; k < count; ++k)
      new(&new_data[i + k]) T(source[start + k]);
    for (; i < size_; ++i)
      new(&new_data[i + k]) T(data_[i]);
  }
  catch (...)
  {
    for (size_t l = 0; l < i + k; ++l)
      (&new_data[l])->~T();
    operator delete(new_data);
    throw;
  }
  for (i = 0; i < size_; ++i)
    (&data_[i])->~T();
  operator delete(data_);
  data_ = new_data;
  size_ += count;
  capacity_ = new_capacity;
}

template<class T>
void burukov::Vector<T>::erase(size_t index)
{
  if (index >= size_)
    throw std::out_of_range("Index out of range");
  if (index == size_ - 1)
  {
    popBack();
    return;
  }
  T *new_data = static_cast<T *>(operator new(sizeof(T) * (size_ - 1)));
  size_t i = 0;
  try
  {
    for (; i < index; ++i)
      new(&new_data[i]) T(data_[i]);
    for (; i < size_ - 1; ++i)
      new(&new_data[i]) T(data_[i + 1]);
  }
  catch (...)
  {
    for (size_t k = 0; k < i; ++k)
      (&new_data[k])->~T();
    operator delete(new_data);
    throw;
  }
  for (i = 0; i < size_; ++i)
    (&data_[i])->~T();
  operator delete(data_);
  data_ = new_data;
  size_--;
}

template<class T>
void burukov::Vector<T>::erase(size_t start, size_t count)
{
  if (start >= size_)
    throw std::out_of_range("Start out of range");
  if (start == size_ - 1)
  {
    popBack();
    return;
  }
  if (start + count >= size_)
    count = size_ - start;
  T *new_data = static_cast<T *>(operator new(sizeof(T) * (size_ - count)));
  size_t i = 0;
  try
  {
    for (; i < start; ++i)
      new(&new_data[i]) T(data_[i]);
    for (; i < size_ - count; ++i)
      new(&new_data[i]) T(data_[i + count]);
  }
  catch (...)
  {
    for (; i > 0; i--)
      (&new_data[i - 1])->~T();
    operator delete(new_data);
    throw;
  }
  for (i = 0; i < size_; ++i)
    (&data_[i])->~T();
  operator delete(data_);
  data_ = new_data;
  size_ -= count;
}

template<class T>
burukov::CIter<T> burukov::Vector<T>::cbegin() const noexcept
{
  return CIter<T>(data_);
}

template<class T>
burukov::CIter<T> burukov::Vector<T>::cend() const noexcept
{
  return CIter<T>(data_ + size_);
}

template<class T>
burukov::Iter<T> burukov::Vector<T>::begin() noexcept
{
  return Iter<T>(data_);
}

template<class T>
burukov::Iter<T> burukov::Vector<T>::end() noexcept
{
  return Iter<T>(data_ + size_);
}

template<class T>
burukov::CIter<T> burukov::Vector<T>::begin() const noexcept
{
  return CIter<T>(data_);
}

template<class T>
burukov::CIter<T> burukov::Vector<T>::end() const noexcept
{
  return CIter<T>(data_ + size_);
}

template<class T>
void burukov::Vector<T>::insert(CIter<T> it, const T &value)
{
  if (it < CIter<T>(data_) || it > CIter<T>(data_ + size_))
    throw std::out_of_range("position out of range");
  size_t position = it - CIter<T>(data_);
  insertAt(position, value);
}

template<class T>
void burukov::Vector<T>::insert(CIter<T> it, T &&value)
{
  if (it < CIter<T>(data_) || it > CIter<T>(data_ + size_))
    throw std::out_of_range("position out of range");
  size_t position = it - CIter<T>(data_);
  insertAt(position, std::move(value));
}

template<class T>
void burukov::Vector<T>::insert(CIter<T> it, CIter<T> start, CIter<T> end)
{
  if (it < CIter<T>(data_) || it > CIter<T>(data_ + size_))
    throw std::out_of_range("iterator index is out of range");
  size_t index = it - CIter<T>(data_);
  size_t count = end - start;
  if (count == 0)
    return;
  size_t new_capacity = capacity_;
  if (size_ + count > capacity_)
    new_capacity = capacity_ * 2 + count;
  T *new_data = static_cast<T *>(operator new(sizeof(T) * new_capacity));
  size_t j = 0;
  try
  {
    for (; j < index; ++j)
      new(&new_data[j]) T(data_[j]);
    for (auto it = start; it != end; ++it)
      new(&new_data[j++]) T(*it);
    for (size_t k = index; k < size_; ++k)
      new(&new_data[j++]) T(data_[k]);
  }
  catch (...)
  {
    for (size_t k = 0; k < j; ++k)
      (&new_data[k])->~T();
    operator delete(new_data);
    throw;
  }
  for (size_t j = 0; j < size_; ++j)
    (&data_[j])->~T();
  operator delete(data_);
  data_ = new_data;
  size_ += count;
  capacity_ = new_capacity;
}

template<class T>
burukov::CIter<T> burukov::Vector<T>::erase(CIter<T> it)
{
  if (it < CIter<T>(data_) || it >= CIter<T>(data_ + size_))
    throw std::out_of_range("index is out of range in erase");
  size_t position = it - cbegin();
  erase(position);
  return CIter<T>(data_ + position);
}

template<class T>
burukov::CIter<T> burukov::Vector<T>::erase(CIter<T> start, CIter<T> end)
{
  if (start < CIter<T>(data_) || end > CIter<T>(data_ + size_) || start > end)
    throw std::out_of_range("range out of range in erase");
  size_t count = end - start;
  size_t position = start - cbegin();
  erase(position, count);
  return CIter<T>(data_ + position);
}

template<class T>
template<class C>
burukov::CIter<T> burukov::Vector<T>::erase(CIter<T> start, CIter<T> end, C condition)
{
  if (start < CIter<T>(data_) || end > CIter<T>(data_ + size_) || start > end)
    throw std::out_of_range("out of range in erase with predicate");
  size_t start_index = start - CIter<T>(data_);
  size_t end_index = end - CIter<T>(data_);
  Vector<T> temporary;
  for (size_t i = 0; i < start_index; ++i)
    temporary.pushBack(data_[i]);
  for (size_t i = start_index; i < end_index; ++i)
    if (!condition(data_[i]))
      temporary.pushBack(data_[i]);
  for (size_t i = end_index; i < size_; ++i)
    temporary.pushBack(data_[i]);
  swap(temporary);
  return CIter<T>(data_ + start_index);
}

template<class T>
void burukov::Vector<T>::reserve(size_t capacity)
{
  if (capacity <= capacity_)
    return;
  Vector<T> temporary = *this;
  temporary.grow(capacity);
  swap(temporary);
}

template<class T>
void burukov::Vector<T>::shrinkToFit()
{
  if (size_ < capacity_)
  {
    T *new_data = static_cast<T *>(operator new(sizeof(T) * size_));
    size_t constructed = 0;
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        new(&new_data[i]) T(std::move(data_[i]));
        ++constructed;
      }
      for (size_t i = 0; i < size_; ++i)
        data_[i].~T();
      operator delete(data_);
      data_ = new_data;
      capacity_ = size_;
    }
    catch(...)
    {
      for (size_t i = 0; i < constructed; ++i)
        new_data[i].~T();
      operator delete(new_data);
      throw;
    }
  }
}

template<class T>
void burukov::Vector<T>::pushBackCount(size_t count, const T &value)
{
  if (count == 0)
    return;
  Vector<T> temporary = *this;
  if (temporary.size_ + count > temporary.capacity_)
  {
    size_t new_capacity = (temporary.capacity_ == 0) ? count : std::max(temporary.size_ + count, temporary.capacity_ * 2);
    temporary.grow(new_capacity);
  }
  for (size_t i = 0; i < count; ++i)
    new(&temporary.data_[temporary.size_ + i]) T(value);
  temporary.size_ += count;
  swap(temporary);
}

template<class T>
template<class IT>
void burukov::Vector<T>::pushBackRange(IT begin, size_t count)
{
  if (count == 0)
    return;
  Vector<T> temporary = *this;
  if (temporary.size_ + count > temporary.capacity_)
  {
    size_t new_capacity = (temporary.capacity_ == 0) ? count : std::max(temporary.size_ + count, temporary.capacity_ * 2);
    temporary.grow(new_capacity);
  }
  for (size_t i = 0; i < count; ++i)
  {
    new(&temporary.data_[temporary.size_ + i]) T(std::move(*begin));
    ++begin;
  }
  temporary.size_ += count;
  swap(temporary);
}

template<class T>
void burukov::Vector<T>::unsafePushBack(const T &value)
{
  new(&data_[size_]) T(value);
  ++size_;
}

template<class T>
template<class P>
void burukov::Vector<T>::insertAt(size_t position, P &&value)
{
  if (position > size_)
    throw std::out_of_range("Index is out of range");
  size_t new_capacity = capacity_ * 2 + 1;
  if (size_ + 1 < capacity_)
    new_capacity = capacity_;
  T *new_data = static_cast<T *>(operator new(sizeof(T) * new_capacity));
  size_t i = 0;
  try
  {
    for (; i < position; ++i)
      new(&new_data[i]) T(data_[i]);
    new(&new_data[position]) T(std::forward<P>(value));
    for (; i < size_; ++i)
      new(&new_data[i + 1]) T(data_[i]);
  }
  catch (...)
  {
    for (size_t k = 0; k < i; ++k)
      (&new_data[k])->~T();
    operator delete(new_data);
    throw;
  }
  for (; i > 0; i--)
    (&data_[i - 1])->~T();
  operator delete(data_);
  data_ = new_data;
  size_++;
  capacity_ = new_capacity;
}

template<class T>
bool burukov::operator==(const Vector<T> &lhs, const Vector<T> &rhs)
{
  bool is_equal = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; i < lhs.getSize() && is_equal; ++i)
    is_equal = is_equal && lhs[i] == rhs[i];
  return is_equal;
}

#endif
