#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <initializer_list>
#include "vector-iterators.hpp"
namespace topit
{
  template< class T >
  struct Vector
  {
    Vector();
    ~Vector();
    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    Vector(size_t, const T&);
    explicit Vector(std::initializer_list< T >);

    VIter< T > begin() const noexcept;
    VIter< T > end() const noexcept;
    VCIter< T > cbegin() const noexcept;
    VCIter< T > cend() const noexcept;

    Vector< T >& operator=(Vector< T >&&);
    Vector< T >& operator=(const Vector< T >&);

    bool has(const T&) const;
    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void reserve(size_t);
    void shrinkToFit();

    T& operator[](size_t) noexcept;
    const T& operator[](size_t) const noexcept;
    T& at(size_t);
    const T& at(size_t) const;

    void pushBack(const T&);
    void pushBackCount(size_t, const T&);
    void popBack();

    void insert(size_t, const T&);
    void insert(const Vector< T >&, size_t, size_t, size_t);
    VIter< T > insert(const VIter< T >&, const T&);
    VIter< T > insert(const VIter< T >&, const T&, size_t);
    VIter< T > insert(const VIter< T >&, const VIter< T >&);

    void erase(size_t);
    void erase(size_t, size_t);
    void erase(const VIter< T >&);
    void erase(const VIter< T >&, size_t);
    void erase(const VIter< T >&, const VIter< T >&);

    void swap(Vector< T >&) noexcept;
    void changeVectorInSomeWay();

  private:
    T* data_;
    size_t size_, capacity_;
    explicit Vector(size_t size);
    void unsafePuchBack(const T&);
  };
  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
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
  delete [] data_;
}
template< class T >
topit::Vector< T >::Vector(const Vector& rhs):
  Vector(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); ++i)
  {
    data_[i] = rhs[i];
  }
}
template< class T >
topit::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
}
template< class T >
topit::Vector< T >::Vector(size_t size):
  data_(size ? new T[size] : nullptr),
  size_(size),
  capacity_(size)
{}
template< class T >
topit::Vector< T >::Vector(size_t size, const T& init):
  Vector(size)
{
  for (size_t i = 0; i < size; ++i)
  {
    data_[i] = init;
  }
}
template< class T >
topit::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it)
  {
    data_[i++] = *it;
  }
}

template< class T >
topit::VIter< T > topit::Vector< T >::begin() const noexcept
{
  VIter< T > tmp{data_};
  return tmp;
}
template< class T >
topit::VIter< T > topit::Vector< T >::end() const noexcept
{
  VIter< T > tmp{data_ + size_};
  return tmp;
}
template< class T >
topit::VCIter< T > topit::Vector< T >::cbegin() const noexcept
{
  VCIter< T > tmp{data_};
  return tmp;
}
template< class T >
topit::VCIter< T > topit::Vector< T >::cend() const noexcept
{
  VCIter< T > tmp{data_ + size_};
  return tmp;
}

template< class T >
topit::Vector< T >& topit::Vector< T >::operator=(const Vector< T >& rhs)
{
  Vector< T > cpy{rhs};
  swap(cpy);
  return *this;
}
template< class T >
topit::Vector< T >& topit::Vector< T >::operator=(Vector< T >&& rhs)
{
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
bool topit::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

template< class T >
bool topit::Vector< T >::has(const T& value) const
{
  for (size_t i = 0; i < size_; ++i)
  {
    if (data_[i] == value)
    {
      return true;
    }
  }

  return false;
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
void topit::Vector< T >::reserve(size_t cap)
{
  if (cap < size_)
  {
    return;
  }
  topit::Vector< T > tmp;
  tmp.capacity_ = cap;
  tmp.size_ = size_;
  size_t i = 0;
  try
  {
    tmp.data_ = new T [cap];
    for (; i < size_; ++i)
    {
      tmp.data_[i] = data_[i];
    }
  }
  catch (...)
  {
    delete [] tmp.data_;
    throw;
  }
  swap(tmp);
}
template< class T >
void topit::Vector< T >::shrinkToFit()
{
  if (capacity_ == size_)
  {
    return;
  }
  if (size_ == 0)
  {
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
    return;
  }
  reserve(size_);
}

template< class T >
T& topit::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T >* cthis = this;
  return const_cast< T& >((*cthis)[id]);
}
template< class T >
const T& topit::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T& topit::Vector< T >::at(size_t id)
{
  const Vector< T >* cthis = this;
  return const_cast< T& >(cthis->at(id));
}
template< class T >
const T& topit::Vector< T >::at(size_t id) const
{
  if (id < getSize())
  {
    return this->operator[](id);
  }
  throw std::out_of_range("bad id");
}

template< class T >
void topit::Vector< T >::pushBack(const T& v)
{
  if (size_ == capacity_)
  {
    T* newData = nullptr;
    size_t i = 0;
    try
    {
      newData = new T [capacity_ + 1];
      for (; i < capacity_; ++i)
      {
        newData[i] = data_[i];
      }
      newData[i++] = v;
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        newData[j].~T();
      }
      delete [] newData;
      throw;
    }
    for (size_t j = 0; j < size_; ++j)
    {
      data_[j].~T();
    }
    delete [] data_;
    data_ = newData;
    ++size_;
    ++capacity_;
    return;
  }
  data_[size_] = v;
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
void topit::Vector< T >::insert(size_t id, const T& value)
{
  if (id > size_)
  {
    throw std::out_of_range("Too much");
  }
  Vector< T > cpy;
  size_t newCapacity = size_ == capacity_ ? capacity_ * 2 : capacity_;
  size_t i = 0;
  T* newData = nullptr;
  try
  {
    newData = new T[newCapacity];
    for (; i < id; ++i)
    {
      newData[i] = data_[i];
    }
    newData[i] = value;
    ++i;
    for (; i < size_ + 1; ++i)
    {
      newData[i] = data_[i - 1];
    }
  }
  catch (...)
  {
    delete [] newData;
    throw;
  }
  cpy.capacity_ = newCapacity;
  cpy.size_ = size_ + 1;
  cpy.data_ = newData;
  swap(cpy);
}
template< class T >
void topit::Vector< T >::insert(const Vector< T >& toPaste, size_t start, size_t end, size_t id)
{
  Vector< T > cpy{*this};
  for (; start < end; ++start)
  {
    cpy.insert(id, toPaste[start]);
    ++id;
  }
  swap(cpy);
}
template< class T >
topit::VIter< T > topit::Vector< T >::insert(const VIter< T >& iterator, const T& value)
{
  size_t id = iterator - begin();
  insert(id, value);
  return VIter< T >{begin() + id};
}
template< class T >
topit::VIter< T > topit::Vector< T >::insert(const VIter< T >& iterator, const T& value, size_t count)
{
  size_t id = iterator - begin();
  for (size_t i = 0; i < count; ++i)
  {
    insert(id + i, value);
  }
  return VIter< T >{begin() + id};
}
template< class T >
topit::VIter< T > topit::Vector< T >::insert(const VIter< T >& placeToInsert, const VIter< T >& valueFromIterator)
{
  size_t id = placeToInsert - begin();
  insert(id, *valueFromIterator);
  return VIter< T >{begin() + id};
}

template< class T >
void topit::Vector< T >::erase(size_t id)
{
  if (id > size_)
  {
    throw std::out_of_range("Too much");
  }
  Vector< T > temp{*this};
  temp.data_[id].~T();
  for (size_t i = id; i < size_ - 1; ++i)
  {
    temp.data_[i] = std::move(temp.data_[i + 1]);
  }
  --temp.size_;
  swap(temp);
}
template< class T >
void topit::Vector< T >::erase(size_t start, size_t end)
{
  Vector< T > temp{*this};
  for (size_t i = 0; i < (end - start); ++i)
  {
    temp.erase(start);
  }
  swap(temp);
}
template< class T >
void topit::Vector< T >::erase(const VIter< T >& iterator)
{
  size_t id = iterator - begin();
  erase(id);
}
template< class T >
void topit::Vector< T >::erase(const VIter< T >& iterator, size_t count)
{
  size_t id = iterator - begin();
  for (size_t i = 0; i < count; ++i)
  {
    erase(id);
  }
}
template< class T >
void topit::Vector< T >::erase(const VIter< T >& start, const VIter< T >& end)
{
  size_t id = start - begin();
  size_t count = end - start;
  for (size_t i = 0; i < count; ++i)
  {
    erase(id);
  }
}

template< class T >
void topit::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}
template< class T >
void topit::Vector< T >::changeVectorInSomeWay()
{
  Vector< T > cpy(*this);
  swap(cpy);
}
#endif