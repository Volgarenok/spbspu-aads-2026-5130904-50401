#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP

#include <cstddef>
#include <utility>
#include <memory>
#include <stdexcept>
#include <initializer_list>

namespace ulanova
{
  template <class T>
  struct Vector
  {
    using iterator = T*;
    using const_iterator = const T*;

    ~Vector();
    Vector();
    Vector(const Vector<T>&);
    Vector(Vector<T>&&) noexcept;
    Vector(size_t size, const T& init);
    explicit Vector(std::initializer_list<T> il);

    Vector& operator=(const Vector<T>&);
    Vector& operator=(Vector<T>&&) noexcept;

    void swap(Vector<T>&) noexcept;

    bool isEmpty() const noexcept;
    size_t getsize() const noexcept;
    size_t getcapacity() const noexcept;

    void reserve(size_t k);
    void shrinktoFit();
    void pushBackCount(size_t k, const T& v);

    template <class IT>
    void pushBackRange(IT b, size_t c);

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    void push_back(const T& v);
    void popback();

    void insert(size_t i, const T& v);
    void erase(size_t i);

    void insert(size_t i, const Vector<T>& rhs, size_t start, size_t end);
    void erase(size_t start, size_t end);

    template <class VectorIterator, class FwdIterator>
    void insert(VectorIterator pos, FwdIterator begin, FwdIterator end);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
  private:
    T* data_;
    size_t size_;
    size_t capacity_;

    explicit Vector(size_t size);
    void unsafePushBack(const T& v);
  };

  template <class T>
  bool operator==(const Vector<T>& lhs, const Vector<T>& rhs);
}

template <class T>
ulanova::Vector<T>::Vector(std::initializer_list<T> il):
  Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it)
  {
    data_[i++] = *it;
  }
}

template <class T>
ulanova::Vector<T>::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template <class T>
ulanova::Vector<T>::Vector(size_t size):
  data_(size ? new T[size] : nullptr),
  size_(size),
  capacity_(size)
{}

template <class T>
ulanova::Vector<T>::Vector(size_t size,const T& init):
  Vector(size)
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = init;
  }
}

template <class T>
ulanova::Vector<T>::Vector(const Vector<T>& rhs):
  Vector(rhs.getsize())
{
  try
  {
    for (size_t i = 0; i < rhs.getsize(); ++i)
    {
      data_[i] = rhs[i];
    }
  }
  catch (...)
  {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }
}

template <class T>
ulanova::Vector<T>::Vector(Vector<T>&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template <class T>
ulanova::Vector<T>::~Vector()
{
  delete[] data_;
}

template <class T>
ulanova::Vector<T>& ulanova::Vector<T>::operator=(const Vector<T>& rhs)
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }
  Vector<T> cpy(rhs);
  swap(cpy);
  return *this;
}

template <class T>
ulanova::Vector<T>& ulanova::Vector<T>::operator=(Vector<T>&& rhs) noexcept
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }
  Vector<T> cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template <class T>
void ulanova::Vector<T>::swap(Vector<T>& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template <class T>
bool ulanova::Vector<T>::isEmpty() const noexcept
{
  return size_ == 0;
}

template <class T>
size_t ulanova::Vector<T>::getsize() const noexcept
{
  return size_;
}

template <class T>
size_t ulanova::Vector<T>::getcapacity() const noexcept
{
  return capacity_;
}

template <class T>
void ulanova::Vector<T>::reserve(size_t k)
{
  if ( k <= capacity_)
  {
    return;
  }
  T* newdata = new T[k];
  try
  {
    for (size_t i = 0; i < size_; ++i)
    {
      newdata[i] = data_[i];
    }
  }
  catch (...)
  {
    delete[] newdata;
    throw;
  }
  delete[] data_;
  data_ = newdata;
  capacity_ = k;
}

template <class T>
void ulanova::Vector<T>::shrinktoFit()
{
  if (size_ == capacity_)
  {
    return;
  }
  Vector<T> tmp(size_);
  for (size_t i = 0; i < size_; ++i)
  {
    tmp[i] = data_[i];
  }
  swap(tmp);
}

template <class T>
void ulanova::Vector<T>::unsafePushBack(const T& v)
{
  data_[size_] = v;
  ++size_;
}

template <class T>
void ulanova::Vector<T>::push_back(const T& v)
{
  if (size_ == capacity_)
  {
    size_t newcap = capacity_ == 0 ? 1 : capacity_ * 2;
    reserve(newcap);
  }
  unsafePushBack(v);
}

template <class T>
void ulanova::Vector<T>::pushBackCount(size_t k, const T& v)
{
  reserve(size_ + k);
  for (size_t i = 0; i < k; ++i)
  {
    unsafePushBack(v);
  }
}

template <class T>
template <class IT>
void ulanova::Vector<T>::pushBackRange(IT b, size_t c)
{
  reserve(size_ + c);
  for (size_t i = 0; i < c; ++i, ++b)
  {
    unsafePushBack(*b);
  }
}

template <class T>
T& ulanova::Vector<T>::operator[](size_t id) noexcept
{
  return data_[id];
}

template <class T>
const T& ulanova::Vector<T>::operator[](size_t id) const noexcept
{
  return data_[id];
}

template <class T>
T& ulanova::Vector<T>::at(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("bad id");
  }
  return data_[id];
}

template <class T>
const T& ulanova::Vector<T>::at(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("bad id");
  }
  return data_[id];
}

template <class T>
void ulanova::Vector<T>::popback()
{
  if (size_ == 0)
  {
    throw std::out_of_range("size is zero: no data");
  }
  --size_;
}

template <class T>
void ulanova::Vector<T>::insert(size_t i, const T& v)
{
  if (i > size_)
  {
    throw std::out_of_range("bad insert");
  }
  Vector<T> tmp;
  tmp.reserve(size_ + 1);

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

template <class T>
void ulanova::Vector<T>::insert(size_t i, const Vector<T>& rhs, size_t start, size_t end)
{
  if (i > size_ || start > end || end > rhs.size_)
  {
    throw std::out_of_range("bad insert");
  }
  Vector<T> tmp;
  tmp.reserve(size_ + end - start);

  for (size_t j = 0; j < i; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }

  for (size_t j = start; j < end; ++j)
  {
    tmp.unsafePushBack(rhs[j]);
  }

  for(size_t j = i; j < size_; ++j)
  {
    tmp.unsafePushBack(data_[j]);
  }

  swap(tmp);
}

template <class T>
void ulanova::Vector<T>::erase(size_t i)
{
  if (i >= size_)
  {
    throw std::out_of_range("bad erase");
  }

  erase(i, i + 1);
}

template <class T>
void ulanova::Vector<T>::erase(size_t start, size_t end)
{
  if (start > end || end > size_)
  {
    throw std::out_of_range("bad erase");
  }

  Vector<T> tmp;
  tmp.reserve(size_ - (end - start));

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

template <class T>
template <class VectorIterator, class FwdIterator>
void ulanova::Vector<T>::insert(VectorIterator pos, FwdIterator begin, FwdIterator end)
{
  if (pos < this->begin() || pos > this->end())
  {
    throw std::out_of_range("bad insert position");
  }

  size_t index = pos - data_;
  size_t count = 0;

  for (auto it = begin; it != end; ++it)
  {
    ++count;
  }

  Vector<T> tmp;
  tmp.reserve(size_ + count);

  for (size_t i = 0; i < index; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }

  for (auto it = begin; it != end; ++it)
  {
    tmp.unsafePushBack(*it);
  }

  for (size_t i = index; i < size_; ++i)
  {
    tmp.unsafePushBack(data_[i]);
  }

  swap(tmp);
}

template <class T>
typename ulanova::Vector<T>::iterator ulanova::Vector<T>::begin() noexcept
{
  return data_;
}

template <class T>
typename ulanova::Vector<T>::iterator ulanova::Vector<T>::end() noexcept
{
  return data_ + size_;
}

template <class T>
typename ulanova::Vector<T>::const_iterator ulanova::Vector<T>::begin() const noexcept
{
  return data_;
}

template <class T>
typename ulanova::Vector<T>::const_iterator ulanova::Vector<T>::end() const noexcept
{
  return data_ + size_;
}

template <class T>
typename ulanova::Vector<T>::const_iterator ulanova::Vector<T>::cbegin() const noexcept
{
  return begin();
}

template <class T>
typename ulanova::Vector<T>::const_iterator ulanova::Vector<T>::cend() const noexcept
{
  return end();
}

template <class T>
bool ulanova::operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
  if (lhs.getsize() != rhs.getsize())
  {
    return false;
  }

  for (size_t i = 0; i < lhs.getsize(); ++i)
  {
    if (!(lhs[i] == rhs[i]))
    {
      return false;
    }
  }

  return true;
}

#endif
