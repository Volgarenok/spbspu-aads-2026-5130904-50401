#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "vciter.hpp"
#include "viter.hpp"

namespace zharov
{
  template< class T >
  struct Vector
  {
    ~Vector();
    Vector();
    Vector(const Vector&);
    Vector(Vector&&);
    explicit Vector(std::initializer_list< T > il);
    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&);
    Vector(size_t size, const T& init);

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;
    bool has(const T& val) const;

    void swap(Vector< T >& rhs) noexcept;
    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void reserve(size_t);
    void shrinkToFit();

    VIter< T > begin();
    VCIter< T > begin() const;
    VCIter< T > cbegin() const;
    VIter< T > end();
    VCIter< T > end() const;
    VCIter< T > cend() const;

    void pushBack(const T& v);
    void pushBackCount(size_t k, const T& v);
    template< class IT >
    void pushBackRange(IT b, size_t c);
    void popBack();
    void insert(size_t id, const T& v);
    void insert(size_t id, const Vector< T >& v, size_t start, size_t end);
    VIter< T > insert(VIter< T > pos, const T& v);
    VIter< T > insert(VIter< T > pos, T&& v);
    VIter< T > insert(VIter< T > pos, VCIter< T > start, VCIter< T > end);
    void erase(size_t id);
    void erase(size_t start, size_t end);
    void erase(VIter< T > start, VIter< T > finish);
    void erase(VIter< T > pos);
    template< class Cmp >
    void erase(VIter< T > pos, Cmp cmp);

  private:
    template< class U >
    VIter< T > generalInsert(VIter< T > pos, U&& v);
    void unsafePushBack(const T& v);
    explicit Vector(size_t c);
    T* data_;
    size_t size_, capacity_;
  };

  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
}

template< class T >
zharov::Vector< T >::~Vector()
{
  for (size_t j = 0; j < size_; ++j)
  {
    data_[j].~T();
  }
  ::operator delete(data_);
}

template< class T >
zharov::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
zharov::Vector< T >::Vector(const Vector& other):
  Vector(other.size_)
{
  size_t i = 0;
  for (; i < other.size_; ++i)
  {
    new (data_ + i) T(other.data_[i]);
  }
  size_ = other.size_;
}

template< class T >
zharov::Vector< T >::Vector(Vector&& o):
  data_(o.data_),
  size_(o.size_),
  capacity_(o.capacity_)
{
  o.data_ = nullptr;
}

template< class T >
zharov::Vector< T >::Vector(size_t c):
  data_(c ? static_cast< T* >(::operator new(sizeof(T) * c)) : nullptr),
  size_(0),
  capacity_(c)
{}

template< class T >
zharov::Vector< T >::Vector(size_t size, const T& init):
  Vector(size)
{
  size_t i = 0;
  for (; i < size; ++i)
  {
    new (data_ + i) T(init);
  }
  size_ = size;
}

template< class T >
zharov::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it, ++i)
  {
    new (data_ + i) T(*it);
  }
  size_ = il.size();
}

template< class T >
zharov::Vector< T >& zharov::Vector< T >::operator=(const Vector& rhs)
{
  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template< class T >
zharov::Vector< T >& zharov::Vector< T >::operator=(Vector&& o)
{
  Vector< T > cpy(std::move(o));
  swap(cpy);
  return *this;
}

template< class T >
const T& zharov::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T& zharov::Vector< T >::operator[](size_t id) noexcept
{
  const Vector* cthis = this;
  return const_cast< T& >((*cthis)[id]);
}

template< class T >
const T& zharov::Vector< T >::at(size_t id) const
{
  if (id < getSize())
  {
    return (*this)[id];
  }
  throw std::out_of_range("Bad index");
}

template< class T >
T& zharov::Vector< T >::at(size_t id)
{
  const Vector< T >* cthis = this;
  return const_cast< T& >(cthis->at(id));
}

template< class T >
bool zharov::Vector< T >::has(const T& val) const
{
  for (auto i = cbegin(); i != cend(); ++i)
  {
    if (*i == val)
    {
      return true;
    };
  }
  return false;
}

template< class T >
void zharov::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
bool zharov::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t zharov::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t zharov::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void zharov::Vector< T >::reserve(size_t new_capacity)
{
  T* new_data = static_cast< T* >(::operator new(sizeof(T) * new_capacity));
  size_t i = 0;
  try
  {
    for (; i < size_; ++i)
    {
      new (new_data + i) T(data_[i]);
    }
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
    {
      new_data[j].~T();
    }
    ::operator delete(new_data);
  }

  for (size_t j = 0; j < size_; ++j)
  {
    data_[j].~T();
  }
  ::operator delete(data_);
  data_ = new_data;
  capacity_ = new_capacity;
}

template< class T >
void zharov::Vector< T >::shrinkToFit()
{
  reserve(size_);
}

template< class T >
zharov::VIter< T > zharov::Vector< T >::begin()
{
  return VIter< T >(data_);
}

template< class T >
zharov::VCIter< T > zharov::Vector< T >::begin() const
{
  return VCIter< T >(data_);
}

template< class T >
zharov::VCIter< T > zharov::Vector< T >::cbegin() const
{
  return VCIter< T >(data_);
}

template< class T >
zharov::VIter< T > zharov::Vector< T >::end()
{
  return VIter< T >(data_ + size_);
}

template< class T >
zharov::VCIter< T > zharov::Vector< T >::end() const
{
  return VCIter< T >(data_ + size_);
}

template< class T >
zharov::VCIter< T > zharov::Vector< T >::cend() const
{
  return VCIter< T >(data_ + size_);
}

template< class T >
void zharov::Vector< T >::pushBack(const T& v)
{
  if (isEmpty())
  {
    reserve(2);
  }
  if (size_ == capacity_)
  {
    reserve(capacity_ * 2);
  }
  unsafePushBack(v);
}

template< class T >
template< class IT >
void zharov::Vector< T >::pushBackRange(IT b, size_t c)
{
  if (size_ + c > capacity_)
  {
    reserve(size_ + c);
  }
  for (size_t i = 0; i < c; ++i)
  {
    unsafePushBack(*b);
    ++b;
  }
}

template< class T >
void zharov::Vector< T >::pushBackCount(size_t k, const T& v)
{
  if (size_ + k > capacity_)
  {
    reserve(size_ + k);
  }
  for (size_t i = 0; i < k; ++i)
  {
    unsafePushBack(v);
  }
}

template< class T >
void zharov::Vector< T >::unsafePushBack(const T& v)
{
  assert(size_ < capacity_);
  new (data_ + size_) T(v);
  ++size_;
}

template< class T >
void zharov::Vector< T >::popBack()
{
  data_[size_ - 1].~T();
  --size_;
}

template< class T >
template< class U >
zharov::VIter< T > zharov::Vector< T >::generalInsert(VIter< T > pos, U&& v)
{
  if (pos == end())
  {
    pushBack(std::forward< U >(v));
    return VIter< T >(pos + size_ - 1);
  }
  std::ptrdiff_t ind = pos - begin();
  Vector< T > cpy(size_ + 1);
  for (VIter< T > it = begin(); it < pos; ++it)
  {
    cpy.unsafePushBack(*it);
  }
  cpy.unsafePushBack(std::forward< U >(v));
  for (VIter< T > it = pos; it < end(); ++it)
  {
    cpy.unsafePushBack(*it);
  }
  swap(cpy);
  return VIter< T >(data_ + ind);
}

template< class T >
zharov::VIter< T >
  zharov::Vector< T >::insert(VIter< T > pos, VCIter< T > start, VCIter< T > finish)
{
  if (start > finish)
  {
    throw std::logic_error("insert: start > end");
  }
  if (pos > end())
  {
    throw std::out_of_range("insert: pos out of range");
  }
  std::ptrdiff_t ind = pos - begin();
  std::ptrdiff_t len = finish - start;
  Vector< T > cpy(size_ + len);
  for (VIter< T > it = begin(); it < pos; ++it)
  {
    cpy.unsafePushBack(*it);
  }
  for (; start < finish; ++start)
  {
    cpy.unsafePushBack(*start);
  }
  for (VIter< T > it = pos; it < end(); ++it)
  {
    cpy.unsafePushBack(*it);
  }
  swap(cpy);
  return VIter< T >(data_ + ind);
}

template< class T >
void zharov::Vector< T >::insert(size_t id, const T& v)
{
  generalInsert(begin() + id, v);
}

template< class T >
zharov::VIter< T > zharov::Vector< T >::insert(VIter< T > pos, const T& v)
{
  return generalInsert(pos, v);
}

template< class T >
zharov::VIter< T > zharov::Vector< T >::insert(VIter< T > pos, T&& v)
{
  return generalInsert(pos, v);
}

template< class T >
void zharov::Vector< T >::insert(size_t id, const Vector< T >& v, size_t start, size_t end)
{
  insert(begin() + id, v.begin() + start, v.begin() + end);
}

template< class T >
void zharov::Vector< T >::erase(VIter< T > pos)
{
  Vector< T > cpy(size_ - 1);
  for (auto it = begin(); it < pos; ++it)
  {
    cpy.unsafePushBack(*it);
  }
  for (auto it = pos + 1; it < end(); ++it)
  {
    cpy.unsafePushBack(*it);
  }
  swap(cpy);
}

template< class T >
template< class Cmp >
void zharov::Vector< T >::erase(VIter< T > pos, Cmp cmp)
{
  if (cmp(*pos))
  {
    erase(pos);
  }
}

template< class T >
void zharov::Vector< T >::erase(VIter< T > start, VIter< T > finish)
{
  if (start > finish)
  {
    throw std::logic_error("erase: start > end");
  }
  if (finish > end())
  {
    throw std::out_of_range("erase: end out of range");
  }
  if (start > end())
  {
    throw std::out_of_range("erase: start out of range");
  }
  std::ptrdiff_t len = finish - start;
  Vector< T > cpy(size_ - len);
  for (auto it = begin(); it < start; ++it)
  {
    cpy.unsafePushBack(*it);
  }
  for (auto it = finish; it < end(); ++it)
  {
    cpy.unsafePushBack(*it);
  }
  swap(cpy);
}

template< class T >
void zharov::Vector< T >::erase(size_t id)
{
  erase(begin() + id);
}

template< class T >
void zharov::Vector< T >::erase(size_t start, size_t end)
{
  erase(begin() + start, begin() + end);
}

template< class T >
bool zharov::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{
  bool res = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; i < lhs.getSize(); ++i)
  {
    res = res && (lhs[i] == rhs[i]);
  }
  return res;
}

#endif
