#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <initializer_list>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <new>

#include "vector_iterator.hpp"

namespace studilova
{
  template< class T >
  struct Vector
  {
    public:
      Vector();
      ~Vector();
      Vector(const Vector< T >&);
      Vector(Vector< T >&&) noexcept;
      Vector(size_t size, const T& init);
      explicit Vector(std::initializer_list< T > il);

      Vector< T >& operator=(const Vector< T >&);
      Vector< T >& operator=(Vector< T >&&) noexcept;

      void swap(Vector< T >& rhs) noexcept;

      bool isEmpty() const noexcept;
      size_t getSize() const noexcept;
      size_t getCapacity() const noexcept;

      void reserve(size_t);
      void shrinkToFit();
      void pushBackCount(size_t k, const T& value);

      template< class IT >
      void pushBackRange(IT b, size_t c);

      T& operator[](size_t id) noexcept;
      const T& operator[](size_t id) const noexcept;
      T& at(size_t id);
      const T& at(size_t id) const;

      void pushBack(const T& value);
      void popBack();

      void insert(size_t i, const T& v);
      void erase(size_t i);

      void insert(size_t i, const Vector< T >& rhs, size_t start, size_t end);
      void erase(size_t start, size_t end);

      using It = VectorIterator< T >;
      using CIt = VectorIterator< const T >;

      It begin() noexcept;
      It end() noexcept;
      CIt begin() const noexcept;
      CIt end() const noexcept;
      CIt cbegin() const noexcept;
      CIt cend() const noexcept;

      It insert(CIt pos, const T& value);

      template< class FwdIterator >
      It insert(CIt pos, FwdIterator first, FwdIterator last);

      It insert(CIt pos, std::initializer_list< T > init);

      It erase(CIt pos);
      It erase(CIt first, CIt last);

      template< class P >
      It remove_if(P p);

    private:
      T* data_;
      size_t size_;
      size_t cap_;

      void grow(size_t new_cap);
      void destroyAll() noexcept;
      explicit Vector(size_t size);

     void unsafePushBack(const T& value);
  };

  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
}

template< class T >
studilova::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  cap_(0)
{}

template< class T >
studilova::Vector< T >::Vector(size_t size):
  data_(size ? static_cast< T* >(operator new(sizeof(T) * size)) : nullptr),
  size_(0),
  cap_(size)
{}

template< class T >
studilova::Vector< T >::Vector(size_t size, const T& init):
  Vector(size)
{
  try
  {
    for (; size_ < size; ++size_)
    {
      new (data_ + size_) T(init);
    }
  }
  catch (...)
  {
    destroyAll();
    operator delete(data_);
    data_ = nullptr;
    cap_ = 0;
    throw;
  }
}

template< class T >
studilova::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  try
  {
    for (const T& value : il)
    {
      unsafePushBack(value);
    }
  }
  catch (...)
  {
    destroyAll();
    operator delete(data_);
    data_ = nullptr;
    cap_ = 0;
    throw;
  }
}

template< class T >
studilova::Vector< T >::Vector(const Vector< T >& rhs):
  Vector(rhs.cap_)
{
  try
  {
    for (; size_ < rhs.size_; ++size_)
    {
      new (data_ + size_) T(rhs.data_[size_]);
    }
  }
  catch (...)
  {
    destroyAll();
    operator delete(data_);
    data_ = nullptr;
    cap_ = 0;
    throw;
  }
}

template< class T >
studilova::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  cap_(rhs.cap_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.cap_ = 0;
}

template< class T >
studilova::Vector< T >::~Vector()
{
  destroyAll();
  operator delete(data_);
}

template< class T >
studilova::Vector< T >& studilova::Vector< T >::operator=(const Vector< T >& rhs)
{
  if (this != std::addressof(rhs))
  {
    Vector< T > tmp(rhs);
    swap(tmp);
  }
  return *this;
}

template< class T >
studilova::Vector< T >& studilova::Vector< T >::operator=(Vector< T >&& rhs) noexcept
{
  if (this != std::addressof(rhs))
  {
    Vector< T > tmp(std::move(rhs));
    swap(tmp);
  }
  return *this;
}

template< class T >
void studilova::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(cap_, rhs.cap_);
}

template< class T >
bool studilova::Vector< T >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class T >
size_t studilova::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t studilova::Vector< T >::getCapacity() const noexcept
{
  return cap_;
}

template< class T >
void studilova::Vector< T >::destroyAll() noexcept
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  size_ = 0;
}

template< class T >
void studilova::Vector< T >::unsafePushBack(const T& value)
{
  new (data_ + size_) T(value);
  ++size_;
}

template< class T >
void studilova::Vector< T >::grow(size_t new_cap)
{
  if (new_cap <= cap_)
  {
    return;
  }

  T* new_data = static_cast< T* >(operator new(sizeof(T) * new_cap));
  size_t done = 0;

  try
  {
    for (; done < size_; ++done)
    {
      new (new_data + done) T(std::move(data_[done]));
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < done; ++i)
    {
      new_data[i].~T();
    }
    operator delete(new_data);
    throw;
  }

  destroyAll();
  operator delete(data_);

  data_ = new_data;
  size_ = done;
  cap_ = new_cap;
}

template< class T >
void studilova::Vector< T >::reserve(size_t new_cap)
{
  grow(new_cap);
}

template< class T >
void studilova::Vector< T >::shrinkToFit()
{
  if (size_ == cap_)
  {
    return;
  }

  Vector< T > tmp(size_);
  for (size_t i = 0; i < size_; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }
  swap(tmp);
}

template< class T >
void studilova::Vector< T >::pushBack(const T& value)
{
  if (size_ == cap_)
  {
    size_t new_cap = cap_ == 0 ? 1 : cap_ * 2;
    grow(new_cap);
  }

  new (data_ + size_) T(value);
  ++size_;
}

template< class T >
void studilova::Vector< T >::pushBackCount(size_t k, const T& value)
{
  if (k == 0)
  {
    return;
  }

  size_t new_cap = cap_;
  if (size_ + k > new_cap)
  {
    new_cap = cap_ == 0 ? k : std::max(size_ + k, cap_ * 2);
  }

  Vector< T > tmp(new_cap);
  for (size_t i = 0; i < size_; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }
  for (size_t i = 0; i < k; ++i)
  {
    tmp.unsafePushBack(value);
  }
  swap(tmp);
}

template< class T >
template< class IT >
void studilova::Vector< T >::pushBackRange(IT b, size_t c)
{
  if (c == 0)
  {
    return;
  }

  size_t new_cap = cap_;
  if (size_ + c > new_cap)
  {
    new_cap = cap_ == 0 ? c : std::max(size_ + c, cap_ * 2);
  }

  Vector< T > tmp(new_cap);
  for (size_t i = 0; i < size_; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }
  for (size_t i = 0; i < c; ++i, ++b)
  {
    tmp.unsafePushBack(*b);
  }
  swap(tmp);
}

template< class T >
void studilova::Vector< T >::popBack()
{
  if (size_ == 0)
  {
    return;
  }

  --size_;
  data_[size_].~T();
}

template< class T >
T& studilova::Vector< T >::operator[](size_t id) noexcept
{
  return data_[id];
}

template< class T >
const T& studilova::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T& studilova::Vector< T >::at(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("Vector index is out of range");
  }
  return data_[id];
}

template< class T >
const T& studilova::Vector< T >::at(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("Vector index is out of range");
  }
  return data_[id];
}

template< class T >
void studilova  ::Vector< T >::insert(size_t i, const T& v)
{
  if (i > size_)
  {
    throw std::out_of_range("Vector insert index is out of range");
  }

  size_t new_cap = size_ + 1 > cap_ ? (cap_ == 0 ? 1 : cap_ * 2) : cap_;
  Vector< T > tmp(new_cap);

  for (size_t j = 0; j < i; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }
  tmp.unsafePushBack(v);
  for (size_t j = i; j < size_; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }

  swap(tmp);
}

template< class T >
void studilova::Vector< T >::erase(size_t i)
{
  if (i >= size_)
  {
    throw std::out_of_range("Vector erase index is out of range");
  }

  Vector< T > tmp(cap_);

  for (size_t j = 0; j < i; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }
  for (size_t j = i + 1; j < size_; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }

  swap(tmp);
}

template< class T >
void studilova::Vector< T >::insert(size_t i, const Vector< T >& rhs, size_t start, size_t end)
{
  if (i > size_ || start > end || end > rhs.size_)
  {
    throw std::out_of_range("Vector range insert is out of range");
  }

  size_t count = end - start;
  if (count == 0)
  {
    return;
  }

  size_t new_size = size_ + count;
  size_t new_cap = new_size > cap_ ? std::max(new_size, cap_ == 0 ? count : cap_ * 2) : cap_;

  Vector< T > tmp(new_cap);

  for (size_t j = 0; j < i; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }
  for (size_t j = start; j < end; ++j)
  {
    tmp.unsafePushBack(rhs.data_[j]);
  }
  for (size_t j = i; j < size_; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }

  swap(tmp);
}

template< class T >
void studilova::Vector< T >::erase(size_t start, size_t end)
{
  if (start > end || end > size_)
  {
    throw std::out_of_range("Vector range erase is out of range");
  }

  if (start == end)
  {
    return;
  }

  Vector< T > tmp(cap_);

  for (size_t i = 0; i < start; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }
  for (size_t i = end; i < size_; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }

  swap(tmp);
}

template< class T >
typename studilova::Vector< T >::It studilova::Vector< T >::begin() noexcept
{
  return It(data_);
}

template< class T >
typename studilova::Vector< T >::It studilova::Vector< T >::end() noexcept
{
  return It(data_ + size_);
}

template< class T >
typename studilova::Vector< T >::CIt studilova::Vector< T >::begin() const noexcept
{
  return CIt(data_);
}

template< class T >
typename studilova::Vector< T >::CIt studilova::Vector< T >::end() const noexcept
{
  return CIt(data_ + size_);
}

template< class T >
typename studilova::Vector< T >::CIt studilova::Vector< T >::cbegin() const noexcept
{
  return CIt(data_);
}

template< class T >
typename studilova::Vector< T >::CIt studilova::Vector< T >::cend() const noexcept
{
  return CIt(data_ + size_);
}

template< class T >
typename studilova::Vector< T >::It studilova::Vector< T >::insert(CIt pos, const T& value)
{
  size_t index = pos - cbegin();
  insert(index, value);
  return begin() + static_cast< std::ptrdiff_t >(index);
}

template< class T >
template< class FwdIterator >
typename studilova::Vector< T >::It studilova::Vector< T >::insert(CIt pos, FwdIterator first, FwdIterator last)
{
  size_t index = pos - cbegin();

  size_t count = 0;
  for (FwdIterator it = first; it != last; ++it)
  {
    ++count;
  }

  if (count == 0)
  {
    return begin() + static_cast< std::ptrdiff_t >(index);
  }

  size_t new_size = size_ + count;
  size_t new_cap = new_size > cap_ ? std::max(new_size, cap_ == 0 ? count : cap_ * 2) : cap_;

  Vector< T > tmp(new_cap);

  for (size_t i = 0; i < index; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }
  for (; first != last; ++first)
  {
    tmp.unsafePushBack(*first);
  }
  for (size_t i = index; i < size_; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }

  swap(tmp);
  return begin() + static_cast< std::ptrdiff_t >(index);
}

template< class T >
typename studilova::Vector< T >::It studilova::Vector< T >::insert(CIt pos, std::initializer_list< T > init)
{
  return insert(pos, init.begin(), init.end());
}

template< class T >
typename studilova::Vector< T >::It studilova::Vector< T >::erase(CIt pos)
{
  size_t index = pos - cbegin();
  erase(index);
  return begin() + static_cast< std::ptrdiff_t >(index);
}

template< class T >
typename studilova::Vector< T >::It studilova::Vector< T >::erase(CIt first, CIt last)
{
  size_t start = first - cbegin();
  size_t end = last - cbegin();
  erase(start, end);
  return begin() + static_cast< std::ptrdiff_t >(start);
}

template< class T >
template< class P >
typename studilova::Vector< T >::It studilova::Vector< T >::remove_if(P p)
{
  Vector< T > tmp(cap_);

  for (size_t i = 0; i < size_; ++i)
  {
    if (!p(data_[i]))
    {
      tmp.unsafePushBack(data_[i]);
    }
  }

  size_t result_index = tmp.size_;
  swap(tmp);
  return begin() + static_cast< std::ptrdiff_t >(result_index);
}

template< class T >
bool studilova::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{
  if (lhs.getSize() != rhs.getSize())
  {
    return false;
  }

  for (size_t i = 0; i < lhs.getSize(); ++i)
  {
    if (!(lhs[i] == rhs[i]))
    {
      return false;
    }
  }

  return true;
}

#endif
