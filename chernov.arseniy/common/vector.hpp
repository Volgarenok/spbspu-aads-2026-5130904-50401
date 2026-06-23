#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <initializer_list>
#include <cstddef>
#include <memory>
#include <stdexcept>

#include <viter.hpp>
#include <vciter.hpp>

namespace chernov {
  template< class T >
  struct Vector {
    Vector();
    Vector(const Vector & v);
    Vector(Vector && v) noexcept;
    ~Vector();
    Vector(size_t size, const T & init);
    explicit Vector(std::initializer_list< T > il);
    Vector & operator=(const Vector & v);
    Vector & operator=(Vector && v) noexcept;

    VIter< T > begin();
    VIter< T > end();
    VCIter< T > begin() const;
    VCIter< T > end() const;
    VCIter< T > cbegin() const;
    VCIter< T > cend() const;

    void swap(Vector< T > & v) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void reserve(size_t size);
    void shrinkToFit();

    T & at(size_t pos);
    const T & at(size_t pos) const;
    T & operator[](size_t pos) noexcept;
    const T & operator[](size_t pos) const noexcept;

    void pushBack(const T & val);
    void pushBack(T && val);
    void pushBackCount(size_t k, const T & val);
    void pushBackCount(size_t k, T && val);
    template< class IT >
    void pushBackRange(IT first, size_t size);
    void popBack();

    void insert(size_t pos, const T & val);
    void insert(size_t pos, T && val);
    void insert(size_t pos, const Vector< T > & v, size_t fisrt, size_t last);
    VIter< T > insert(VIter< T > pos, const T & val);
    VIter< T > insert(VIter< T > pos, T && val);
    VIter< T > insert(VIter< T > pos, VIter< T > first, VIter< T > last);

    void erase(size_t pos);
    void erase(size_t fisrt, size_t last);
    VIter< T > erase(VIter< T > pos);
    VIter< T > erase(VIter< T > fisrt, VIter< T > last);

  private:
    T * data_;
    size_t size_, capacity_;

    explicit Vector(size_t size);
    void extend();

    template< class U >
    void unsafePushBack(U && val);
    template< class U >
    void generalPushBack(U && val);
    template< class U >
    void generalPushBackCount(size_t k, U && val);

    template< class U >
    void generalInsert(size_t pos, U && val);
    template< class U >
    VIter< T > generalInsert(VIter< T > pos, U && val);
  };

  template< class T >
  bool operator==(const Vector< T > & lhs, const Vector< T > & rhs);
}

template< class T >
chernov::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
chernov::Vector< T >::Vector(const Vector< T > & v):
  Vector(v.size_)
{
  size_t i = 0;
  try {
    for (; i < v.getSize(); ++i) {
      new (data_ + i) T(v[i]);
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[j].~T();
    }
    ::operator delete (data_);
    throw;
  }
}

template< class T >
chernov::Vector< T >::Vector(Vector< T > && v) noexcept:
  data_(v.data_),
  size_(v.size_),
  capacity_(v.capacity_)
{
  v.data_ = nullptr;
  v.size_ = 0;
  v.capacity_ = 0;
}

template< class T >
chernov::Vector< T >::~Vector()
{
  for (size_t i = 0; i < size_; ++i) {
    data_[i].~T();
  }
  ::operator delete (data_);
}

template< class T >
chernov::Vector< T >::Vector(size_t size):
  data_(size ? static_cast< T * >(::operator new (sizeof(T) * size)) : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
chernov::Vector< T >::Vector(size_t size, const T & init):
  Vector(size)
{
  size_t i = 0;
  try {
    for (; i < size; ++i) {
      new (data_ + i) T(init);
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[j].~T();
    }
    ::operator delete (data_);
    throw;
  }
}

template< class T >
chernov::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t i = 0;
  try {
    for (auto iter = il.begin(); iter != il.end(); ++iter) {
      new (data_ + i) T(*iter);
      ++i;
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[j].~T();
    }
    ::operator delete (data_);
    throw;
  }
}

template< class T >
chernov::Vector< T > & chernov::Vector< T >::operator=(const Vector< T > & v)
{
  if (this == std::addressof(v)) {
    return *this;
  }
  Vector< T > cpy = v;
  swap(cpy);
  return *this;
}

template< class T >
chernov::Vector< T > & chernov::Vector< T >::operator=(Vector< T > && v) noexcept
{
  if (this == std::addressof(v)) {
    return *this;
  }
  Vector< T > cpy = std::move(v);
  swap(cpy);
  return *this;
}

template< class T >
chernov::VIter< T > chernov::Vector< T >::begin()
{
  return VIter< T >{data_};
}

template< class T >
chernov::VIter< T > chernov::Vector< T >::end()
{
  return VIter< T >{data_ + size_};
}

template< class T >
chernov::VCIter< T > chernov::Vector< T >::begin() const
{
  return VCIter< T >{data_};
}

template< class T >
chernov::VCIter< T > chernov::Vector< T >::end() const
{
  return VCIter< T >{data_ + size_};
}

template< class T >
chernov::VCIter< T > chernov::Vector< T >::cbegin() const
{
  return VCIter< T >{data_};
}

template< class T >
chernov::VCIter< T > chernov::Vector< T >::cend() const
{
  return VCIter< T >{data_ + size_};
}

template< class T >
void chernov::Vector< T >::swap(Vector< T > & v) noexcept
{
  std::swap(data_, v.data_);
  std::swap(size_, v.size_);
  std::swap(capacity_, v.capacity_);
}

template< class T >
void chernov::Vector< T >::extend()
{
  const size_t new_capacity = (capacity_ == 0) ? 1 : 2 * capacity_;
  reserve(new_capacity);
}

template< class T >
bool chernov::Vector< T >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class T >
size_t chernov::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t chernov::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void chernov::Vector< T >::reserve(size_t size)
{
  Vector< T > cpy(size);
  cpy.size_ = 0;
  for (size_t i = 0; i < size_; ++i) {
    new (cpy.data_ + i) T(data_[i]);
    ++cpy.size_;
  }
  swap(cpy);
}

template< class T >
void chernov::Vector< T >::shrinkToFit()
{
  reserve(size_);
}

template< class T >
T & chernov::Vector< T >::at(size_t pos)
{
  const Vector< T > * cthis = this;
  return const_cast< T & >(cthis->at(pos));
}

template< class T >
const T & chernov::Vector< T >::at(size_t pos) const
{
  if (pos >= size_) {
    throw std::out_of_range("Out of array's size");
  }
  return data_[pos];
}

template< class T >
T & chernov::Vector< T >::operator[](size_t pos) noexcept
{
  return data_[pos];
}

template< class T >
const T & chernov::Vector< T >::operator[](size_t pos) const noexcept
{
  return data_[pos];
}

template< class T >
template< class U >
void chernov::Vector< T >::unsafePushBack(U && val)
{
  new (data_ + size_) T(std::forward< U >(val));
  ++size_;
}

template< class T >
template< class U >
void chernov::Vector< T >::generalPushBack(U && val)
{
  if (size_ == capacity_) {
    extend();
  }
  unsafePushBack(val);
}

template< class T >
void chernov::Vector< T >::pushBack(const T & val)
{
  generalPushBack(val);
}

template< class T >
void chernov::Vector< T >::pushBack(T && val)
{
  generalPushBack(std::move(val));
}

template< class T >
template< class U >
void chernov::Vector< T >::generalPushBackCount(size_t k, U && val)
{
  Vector< T > cpy(std::max(capacity_, size_ + k));
  cpy.size_ = 0;
  for (size_t i = 0; i < size_; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  for (size_t i = 0; i < k; ++i) {
    cpy.unsafePushBack(val);
  }
  swap(cpy);
}

template< class T >
void chernov::Vector< T >::pushBackCount(size_t k, const T & val)
{
  generalPushBackCount(k, val);
}

template< class T >
void chernov::Vector< T >::pushBackCount(size_t k, T && val)
{
  generalPushBackCount(k, std::move(val));
}

template< class T >
template< class IT >
void chernov::Vector< T >::pushBackRange(IT first, size_t size)
{
  Vector< T > cpy(std::max(capacity_, size_ + size));
  cpy.size_ = 0;
  for (size_t i = 0; i < size_; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  for (size_t i = 0; i < size; ++i) {
    cpy.unsafePushBack(*(first++));
  }
  swap(cpy);
}

template< class T >
void chernov::Vector< T >::popBack()
{
  if (size_ > 0) {
    --size_;
    data_[size_].~T();
  }
}

template< class T >
template< class U >
void chernov::Vector< T >::generalInsert(size_t pos, U && val)
{
  if (pos > size_) {
    throw std::out_of_range("Out of array's size");
  }
  Vector< T > cpy(std::max(capacity_, size_ + 1));
  cpy.size_ = 0;
  for (size_t i = 0; i < pos; ++i)
    cpy.unsafePushBack(data_[i]);
  cpy.unsafePushBack(std::forward< U >(val));
  for (size_t i = pos; i < size_; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  swap(cpy);
}

template< class T >
void chernov::Vector< T >::insert(size_t pos, const T & val)
{
  generalInsert(pos, val);
}

template< class T >
void chernov::Vector< T >::insert(size_t pos, T && val)
{
  generalInsert(pos, std::move(val));
}

template< class T >
void chernov::Vector< T >::insert(size_t pos, const Vector< T > & v, size_t first, size_t last)
{
  if (pos > size_) {
    throw std::out_of_range("Out of array's size");
  }
  if (last <= first || v.getSize() < last) {
    throw std::out_of_range("Incorrect start or end");
  }
  size_t insert_size = last - first;
  Vector< T > cpy(std::max(capacity_, size_ + insert_size));
  cpy.size_ = 0;
  for (size_t i = 0; i < pos; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  for (size_t i = first; i < last; ++i) {
    cpy.unsafePushBack(v.data_[i]);
  }
  for (size_t i = pos; i < size_; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  swap(cpy);
}

template< class T >
template< class U >
chernov::VIter< T > chernov::Vector< T >::generalInsert(VIter< T > pos, U && val)
{
  size_t index = static_cast<size_t>(pos - begin());
  generalInsert(index, std::forward<U>(val));
  return VIter< T >{data_ + index};
}

template< class T >
chernov::VIter< T > chernov::Vector< T >::insert(VIter< T > pos, const T & val)
{
  return generalInsert(pos, val);
}

template< class T >
chernov::VIter< T > chernov::Vector< T >::insert(VIter< T > pos, T && val)
{
  return generalInsert(pos, std::move(val));
}

template< class T >
chernov::VIter< T > chernov::Vector< T >::insert(VIter< T > pos, VIter< T > first, VIter< T > last)
{
  if (first == last) {
    return pos;
  }
  size_t index = pos - begin();
  size_t insert_size = last - first;
  Vector< T > cpy(std::max(capacity_, size_ + insert_size));
  cpy.size_ = 0;
  for (size_t i = 0; i < index; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  for (VIter< T > it = first; it != last; ++it) {
    cpy.unsafePushBack(*it);
  }
  for (size_t i = index; i < size_; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  swap(cpy);
  return VIter< T >{data_ + index};
}

template< class T >
void chernov::Vector< T >::erase(size_t pos)
{
  if (pos >= size_) {
    throw std::out_of_range("Out of array's size");
  }
  if (pos == size_ - 1) {
    popBack();
    return;
  }
  Vector< T > cpy(size_ - 1);
  cpy.size_ = 0;
  for (size_t i = 0; i < pos; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  for (size_t i = pos + 1; i < size_; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  swap(cpy);
}

template< class T >
void chernov::Vector< T >::erase(size_t first, size_t last)
{
  if (first > size_ || last > size_ || first > last) {
    throw std::out_of_range("Invalid erase range");
  }
  if (first == last) {
    return;
  }
  size_t remove_count = last - first;
  Vector< T > cpy(size_ - remove_count);
  cpy.size_ = 0;
  for (size_t i = 0; i < first; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  for (size_t i = last; i < size_; ++i) {
    cpy.unsafePushBack(data_[i]);
  }
  swap(cpy);
}

template< class T >
chernov::VIter< T > chernov::Vector< T >::erase(VIter< T > pos)
{
  size_t index = pos - begin();
  erase(index);
  return VIter< T >{data_ + index};
}

template< class T >
chernov::VIter< T > chernov::Vector< T >::erase(VIter< T > first, VIter< T > last)
{
  if (first == last) {
    return last;
  }
  size_t index_first = first - begin();
  size_t index_last  = last - begin();
  erase(index_first, index_last);
  return VIter< T >{data_ + index_first};
}

template< class T >
bool chernov::operator==(const Vector< T > & lhs, const Vector< T > & rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; i < lhs.getSize() && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

#endif
