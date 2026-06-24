#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <new>
#include <algorithm>
#include <initializer_list>
#include "viter.hpp"

namespace vasyakin
{
  template< class T >
  struct Vector
  {

    using iterator = VectorIterator< T >;
    using const_iterator = VectorIterator< const T >;

    Vector();
    ~Vector();
    Vector(size_t size, const T& init);

    Vector(const Vector< T >&);
    Vector(Vector< T >&&) noexcept;
    Vector< T >& operator=(const Vector< T >&);
    Vector< T >& operator=(Vector< T >&&) noexcept;

    explicit Vector(std::initializer_list< T > il);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    void swap(Vector< T >& rhs) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void reserve(size_t k);
    void shrinkToFit();
    void pushBackCount(size_t k, const T& value);
    template< class IT >
    void pushBackRange(IT b, size_t c);
    void unsafePushBack(const T& value);

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    void push_back(const T& value);
    void popBack();

    void insert(size_t i, const T& v);
    void insert(size_t i, const Vector< T >& rhs, size_t start, size_t end);

    iterator insert(const_iterator pos, const T& value);
    template< class FwdIterator >
    iterator insert(const_iterator pos, FwdIterator first, FwdIterator last);
    iterator insert(const_iterator pos, std::initializer_list< T > init);

    void erase(size_t i);
    void erase(size_t start, size_t end);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    template< class P >
    iterator remove_if(P p);

  private:
    T* data_;
    size_t size_, cap_;

    explicit Vector(size_t size);
    void destroyAll() noexcept;
    void grow(size_t new_cap);
  };

  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
}

template< class T >
vasyakin::Vector< T >::Vector(std::initializer_list< T > il):
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
vasyakin::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  cap_(0)
{}

template< class T >
vasyakin::Vector< T >::~Vector()
{
  destroyAll();
  operator delete(data_);
}

template< class T >
void vasyakin::Vector< T >::destroyAll() noexcept
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  size_ = 0;
}

template< class T >
vasyakin::Vector< T >::Vector(size_t size):
  data_(size ? static_cast< T* >(operator new(sizeof(T) * size)) : nullptr),
  size_(0),
  cap_(size)
{}

template< class T >
vasyakin::Vector< T >::Vector(size_t size, const T& init):
  Vector(size)
{
  for (size_t i = 0; i < size; ++i)
  {
    new (&data_[i]) T(init);
  }
  size_ = size;
}

template< class T >
vasyakin::Vector< T >::Vector(const Vector< T > & rhs):
  Vector(rhs.getCapacity())
{
  for (size_t i = 0; i < rhs.getSize(); ++i)
  {
    new (&data_[i]) T(rhs.data_[i]);
  }
  size_ = rhs.size_;
}

template< class T >
vasyakin::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  cap_(rhs.cap_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.cap_ = 0;
}

template< class T >
vasyakin::Vector< T >& vasyakin::Vector< T >::operator=(const Vector< T >& rhs)
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
vasyakin::Vector< T >& vasyakin::Vector< T >::operator=(Vector< T >&& rhs) noexcept
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
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::begin() noexcept
{
  return iterator(data_);
}

template< class T >
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::end() noexcept
{
  return iterator(data_ + size_);
}

template< class T >
typename vasyakin::Vector< T >::const_iterator vasyakin::Vector< T >::begin() const noexcept
{
  return const_iterator(data_);
}

template< class T >
typename vasyakin::Vector< T >::const_iterator vasyakin::Vector< T >::end() const noexcept
{
  return const_iterator(data_ + size_);
}

template< class T >
typename vasyakin::Vector< T >::const_iterator vasyakin::Vector< T >::cbegin() const noexcept
{
  return const_iterator(data_);
}

template< class T >
typename vasyakin::Vector< T >::const_iterator vasyakin::Vector< T >::cend() const noexcept
{
  return const_iterator(data_ + size_);
}

template< class T >
void vasyakin::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(cap_, rhs.cap_);
}

template< class T >
bool vasyakin::Vector< T >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class T >
size_t vasyakin::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t vasyakin::Vector< T >::getCapacity() const noexcept
{
  return cap_;
}

template< class T >
void vasyakin::Vector< T >::reserve(size_t k)
{
  if (k <= cap_)
  {
    return;
  }

  Vector< T > tmp = *this;
  tmp.grow(k);
  swap(tmp);
}

template< class T >
void vasyakin::Vector< T >::shrinkToFit()
{
  if (size_ < cap_)
  {
    T* new_data = static_cast< T* >(operator new(sizeof(T) * size_));
    size_t constructed = 0;
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        new (&new_data[i]) T(std::move(data_[i]));
        ++constructed;
      }

      for (size_t i = 0; i < size_; ++i)
      {
        data_[i].~T();
      }

      operator delete(data_);
      data_ = new_data;
      cap_ = size_;
    }
    catch(...)
    {
      for (size_t i = 0; i < constructed; ++i)
      {
        new_data[i].~T();
      }
      operator delete(new_data);
      throw;
    }
  }
}

template< class T >
void vasyakin::Vector< T >::pushBackCount(size_t k, const T& value)
{
  if (k == 0)
  {
    return;
  }

  Vector< T > tmp = *this;

  if (tmp.size_ + k > tmp.cap_)
  {
    size_t new_cap = (tmp.cap_ == 0) ? k : std::max(tmp.size_ + k, tmp.cap_ * 2);
    tmp.grow(new_cap);
  }

  for (size_t i = 0; i < k; ++i)
  {
    new (&tmp.data_[tmp.size_ + i]) T(value);
  }

  tmp.size_ += k;
  swap(tmp);
}

template< class T >
template< class IT >
void vasyakin::Vector< T >::pushBackRange(IT b, size_t c)
{
  if (c == 0)
  {
    return;
  }

  Vector< T > tmp = *this;

  if (tmp.size_ + c > tmp.cap_)
  {
    size_t new_cap = (tmp.cap_ == 0) ? c : std::max(tmp.size_ + c, tmp.cap_ * 2);
    tmp.grow(new_cap);
  }

  for (size_t i = 0; i < c; ++i)
  {
    new (&tmp.data_[tmp.size_ + i]) T(std::move(*b));
    ++b;
  }

  tmp.size_ += c;
  swap(tmp);
}

template< class T >
void vasyakin::Vector< T >::unsafePushBack(const T& value)
{
  new(&data_[size_]) T(value);
  ++size_;
}

template< class T >
T& vasyakin::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T >* cthis = this;
  return const_cast< T& >((*cthis)[id]);
}

template< class T >
const T& vasyakin::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T& vasyakin::Vector< T >::at(size_t id)
{
  const Vector< T >* cthis = this;
  return const_cast< T& >(cthis->at(id));
}

template< class T >
const T& vasyakin::Vector< T >::at(size_t id) const
{
  if (id < getSize())
  {
    return (*this).operator[](id);
  }
  throw std::out_of_range("bad id");
}

template < class T >
void vasyakin::Vector< T >::grow(size_t new_cap)
{
  T* new_data = static_cast< T* >(operator new(sizeof(T) * new_cap));
  size_t constructed = 0;
  try
  {
    for (size_t i = 0; i < size_; ++i)
    {
      new (&new_data[i]) T(std::move(data_[i]));
      ++constructed;
    }

    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }

    operator delete(data_);
    data_ = new_data;
    cap_ = new_cap;
  }
  catch(...)
  {
    for (size_t i = 0; i < constructed; ++i)
    {
      new_data[i].~T();
    }
    operator delete(new_data);
    throw;
  }
}

template< class T >
void vasyakin::Vector< T >::push_back(const T& value)
{
  Vector< T > temp = *this;

  if (temp.size_ == temp.cap_)
  {
    size_t new_cap = (temp.cap_ == 0) ? 1 : temp.cap_ * 2;
    temp.grow(new_cap);
  }

  new (&temp.data_[temp.size_]) T(value);
  ++temp.size_;

  swap(temp);
}

template< class T >
void vasyakin::Vector< T >::popBack()
{
  if (size_ > 0)
  {
    --size_;
    data_[size_].~T();
  }
}

template< class T >
void vasyakin::Vector< T >::insert(size_t i, const T& v)
{
  if (i > size_)
  {
    throw std::out_of_range("Too much");
  }

  Vector< T > temp = *this;

  if (temp.size_ == temp.cap_)
  {
    size_t new_cap = (temp.cap_ == 0) ? 1 : temp.cap_ * 2;
    temp.grow(new_cap);
  }

  for (size_t j = temp.size_; j > i; --j)
  {
    new (&temp.data_[j]) T(std::move(temp.data_[j - 1]));
    temp.data_[j - 1].~T();
  }

  new (&temp.data_[i]) T(v);
  ++temp.size_;

  swap(temp);
}

template< class T >
void vasyakin::Vector< T >::insert(size_t i, const Vector< T >& rhs, size_t start, size_t end)
{
  if (i > size_ || start > end || end > rhs.size_)
  {
    throw std::out_of_range("Too much");
  }

  size_t count = end - start;
  if (count == 0)
  {
    return;
  }

  Vector< T > temp = *this;

  if (temp.size_ + count > temp.cap_)
  {
    size_t new_cap = (temp.cap_ == 0) ? count : std::max(temp.cap_ * 2, temp.size_ + count);
    temp.grow(new_cap);
  }

  for (size_t j = temp.size_; j > i; --j)
  {
    new (&temp.data_[j + count - 1]) T(std::move(temp.data_[j - 1]));
    temp.data_[j - 1].~T();
  }

  for (size_t j = 0; j < count; ++j)
  {
    new (&temp.data_[i + j]) T(rhs.data_[start + j]);
  }

  temp.size_ += count;

  swap(temp);
}

template< class T >
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::insert(const_iterator pos, const T& value)
{
  size_t index = pos - begin();
  insert(index, value);
  return begin() + index;
}

template< class T >
template< class FwdIterator >
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::insert(const_iterator pos, FwdIterator first, FwdIterator last)
{
  size_t index = pos - begin();

  size_t count = 0;
  for (FwdIterator it = first; it != last; ++it)
  {
    ++count;
  }

  if (count == 0)
  {
    return begin() + index;
  }

  Vector< T > tmp = *this;

  if (tmp.size_ + count > tmp.cap_)
  {
    size_t new_cap = (tmp.cap_ == 0) ? count : std::max(tmp.cap_ * 2, tmp.size_ + count);
    tmp.grow(new_cap);
  }

  for (size_t j = tmp.size_; j > index; --j)
  {
    new (&tmp.data_[j + count - 1]) T(std::move(tmp.data_[j - 1]));
    tmp.data_[j - 1].~T();
  }

  for (size_t j = 0; j < count; ++j)
  {
    new (&tmp.data_[index + j]) T(*first);
    ++first;
  }

  tmp.size_ += count;
  swap(tmp);

  return begin() + index;
}

template< class T >
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::insert(const_iterator pos, std::initializer_list< T > init)
{
  return insert(pos, init.begin(), init.end());
}

template< class T >
void vasyakin::Vector< T >::erase(size_t i)
{
  if (i >= size_)
  {
    throw std::out_of_range("Too much");
  }

  Vector< T > temp(size_ - 1);

  for (size_t j = 0; j < i; ++j)
  {
    new (&temp.data_[j]) T(data_[j]);
  }

  for (size_t j = i + 1; j < size_; ++j)
  {
    new (&temp.data_[j - 1]) T(data_[j]);
  }

  temp.size_ = size_ - 1;

  swap(temp);
}

template< class T >
void vasyakin::Vector< T >::erase(size_t start, size_t end)
{
  if (start > end || end > size_)
  {
    throw std::out_of_range("Too much");
  }

  size_t count = end - start;
  if (count == 0)
  {
    return;
  }

  Vector< T > temp(size_ - count);

  for (size_t j = 0; j < start; ++j)
  {
    new (&temp.data_[j]) T(data_[j]);
  }

  for (size_t j = end; j < size_; ++j)
  {
    new (&temp.data_[j - count]) T(data_[j]);
  }

  temp.size_ = size_ - count;

  swap(temp);
}

template< class T >
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::erase(const_iterator pos)
{
  size_t index = pos - begin();
  erase(index);
  return begin() + index;
}

template< class T >
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::erase(const_iterator first, const_iterator last)
{
  size_t start = first - begin();
  size_t end = last - begin();
  erase(start, end);
  return begin() + start;
}

template< class T >
template< class P >
typename vasyakin::Vector< T >::iterator vasyakin::Vector< T >::remove_if(P p)
{
  size_t write = 0;

  for (size_t read = 0; read < size_; ++read)
  {
    if (!p(data_[read]))
    {
      if (write != read)
      {
        data_[write] = std::move(data_[read]);
      }
      ++write;
    }
  }

  for (size_t i = write; i < size_; ++i)
  {
    data_[i].~T();
  }

  size_ = write;

  return begin() + write;
}

template< class T >
bool vasyakin::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

#endif
