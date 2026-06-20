#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include "vector_citer.hpp"
#include "vector_iter.hpp"
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace hvostov {
  template< class T >
  class Vector {
  public:
    ~Vector();
    Vector();
    Vector(const Vector< T >&);
    Vector(size_t size, const T& init);
    Vector(size_t size, const T* arr);
    explicit Vector(std::initializer_list< T >);
    Vector(Vector< T >&&) noexcept;

    Vector< T >& operator=(const Vector< T >&);
    Vector< T >& operator=(Vector< T >&&);
    T& operator[](size_t index) noexcept;
    const T& operator[](size_t index) const noexcept;

    T& at(size_t index);
    const T& at(size_t index) const;

    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    size_t getCapacity() const noexcept;

    void shrinkToFit();
    void reserve(size_t);
    void swap(Vector< T >& rhs) noexcept;

    void pushBack(const T& v);
    void pushBackCount(size_t k, const T& val);
    template< class IT >
    void pushBackRange(IT b, size_t k);
    void popBack();

    void insert(size_t pos, const T& v);
    void insert(size_t i, const Vector< T >& rhs, size_t start, size_t end);
    Iterator< T > insert(Iterator< T > pos, const T& val);
    Iterator< T > insert(Iterator< T > pos, Iterator< T > first, Iterator< T > last);

    void erase(size_t pos);
    void erase(size_t start, size_t end);
    Iterator< T > erase(Iterator< T > pos);
    Iterator< T > erase(Iterator< T > first, Iterator< T > last);

    Iterator< T > begin();
    Iterator< T > end();
    Iterator< T > begin() const;
    Iterator< T > end() const;
    CIterator< T > cbegin() const;
    CIterator< T > cend() const;

  private:
    explicit Vector(size_t size);
    void unsafePushBack(const T& val);
    T* data_;
    size_t size_, capacity_;
  };

  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
}

template< class T >
hvostov::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); it++) {
    data_[i++] = *it;
  }
}

template< class T >
void hvostov::Vector< T >::reserve(size_t k)
{
  if (k == capacity_) {
    return;
  }
  size_t new_size = size_;
  if (size_ > k) {
    new_size = k;
  }
  T* new_data = new T[k];
  for (size_t i = 0; i < new_size; i++) {
    try {
      new_data[i] = data_[i];
    } catch (...) {
      delete[] new_data;
      throw;
    }
  }
  delete[] data_;
  data_ = new_data;
  size_ = new_size;
  capacity_ = k;
}

template< class T >
void hvostov::Vector< T >::shrinkToFit()
{
  reserve(size_);
}

template< class T >
hvostov::Vector< T >::Vector(size_t size):
  data_(size ? new T[size] : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
hvostov::Vector< T >::Vector(size_t size, const T* arr):
  Vector(size)
{
  for (size_t i = 0; i < size; i++) {
    try {
      data_[i] = arr[i];
    } catch (...) {
      delete[] data_;
      throw;
    }
  }
}

template< class T >
hvostov::Vector< T >::Vector(size_t size, const T& init):
  Vector(size)
{
  for (size_t i = 0; i < size; i++) {
    try {
      data_[i] = init;
    } catch (...) {
      delete[] data_;
      throw;
    }
  }
}

template< class T >
hvostov::Vector< T >::~Vector()
{
  delete[] data_;
}

template< class T >
void hvostov::Vector< T >::insert(size_t pos, const T& v)
{
  if (pos > size_) {
    throw std::out_of_range("Insert error! Insert position > size of vector!");
  }
  Vector< T > cpy{*this};
  if (cpy.size_ + 1 > cpy.capacity_) {
    cpy.reserve(cpy.capacity_ == 0 ? 1 : cpy.capacity_ * 2);
  }
  for (size_t i = cpy.size_; i > pos; --i) {
    cpy.data_[i] = cpy.data_[i - 1];
  }
  cpy.data_[pos] = v;
  ++cpy.size_;
  swap(cpy);
}

template< class T >
void hvostov::Vector< T >::insert(size_t pos, const Vector< T >& rhs, size_t start, size_t end)
{
  if (pos > size_) {
    throw std::out_of_range("Insert error: position > size");
  }
  if (start > rhs.size_ || end > rhs.size_ || start > end) {
    throw std::out_of_range("Insert error: invalid range in rhs");
  }
  size_t count = end - start;
  Vector< T > cpy(*this);
  if (cpy.size_ + count > cpy.capacity_) {
    cpy.reserve(cpy.size_ + count);
  }
  for (size_t i = cpy.size_; i > pos; --i) {
    cpy.data_[i + count - 1] = std::move(cpy.data_[i - 1]);
  }
  for (size_t i = 0; i < count; ++i) {
    cpy.data_[pos + i] = rhs.data_[start + i];
  }
  cpy.size_ += count;
  swap(cpy);
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::insert(Iterator< T > pos, const T& val)
{
  size_t index = pos - begin();
  insert(index, val);
  return begin() + index;
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::insert(Iterator< T > pos, Iterator< T > first, Iterator< T > last)
{
  size_t index = pos - begin();
  size_t count = last - first;

  if (count == 0) {
    return begin() + index;
  }

  Vector< T > cpy(*this);

  if (cpy.size_ + count > cpy.capacity_) {
    cpy.reserve(cpy.size_ + count);
  }

  for (size_t i = cpy.size_; i > index; --i) {
    cpy.data_[i + count - 1] = std::move(cpy.data_[i - 1]);
  }

  for (size_t i = 0; i < count; ++i) {
    cpy.data_[index + i] = *(first + i);
  }

  cpy.size_ += count;
  swap(cpy);

  return begin() + index;
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::erase(Iterator< T > pos)
{
  size_t index = pos - begin();
  erase(index);
  return begin() + index;
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::erase(Iterator< T > first, Iterator< T > last)
{
  size_t start = first - begin();
  size_t end = last - begin();
  erase(start, end);
  return begin() + start;
}

template< class T >
void hvostov::Vector< T >::erase(size_t pos)
{
  if (pos >= size_) {
    throw std::out_of_range("Erase error: position >= size");
  }
  Vector< T > cpy(*this);
  for (size_t i = pos; i < cpy.size_ - 1; ++i) {
    cpy.data_[i] = std::move(cpy.data_[i + 1]);
  }
  cpy.data_[cpy.size_ - 1].~T();
  --cpy.size_;
  swap(cpy);
}

template< class T >
void hvostov::Vector< T >::erase(size_t start, size_t end)
{
  if (start > size_ || end > size_ || start > end) {
    throw std::out_of_range("Erase error: invalid range");
  }
  size_t count = end - start;
  if (count == 0) {
    return;
  }
  Vector< T > cpy(*this);
  for (size_t i = end; i < cpy.size_; ++i) {
    cpy.data_[i - count] = std::move(cpy.data_[i]);
  }
  for (size_t i = 0; i < count; ++i) {
    cpy.data_[cpy.size_ - 1 - i].~T();
  }
  cpy.size_ -= count;
  swap(cpy);
}

template< class T >
hvostov::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
hvostov::Vector< T >::Vector(const Vector< T >& rhs):
  Vector(rhs.size())
{
  for (size_t i = 0; i < rhs.size(); i++) {
    data_[i] = rhs[i];
  }
}

template< class T >
hvostov::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
}

template< class T >
void hvostov::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
hvostov::Vector< T >& hvostov::Vector< T >::operator=(const Vector< T >& rhs)
{
  Vector< T > cpy{rhs};
  swap(cpy);
  return *this;
}

template< class T >
hvostov::Vector< T >& hvostov::Vector< T >::operator=(Vector< T >&& rhs)
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector< T > cpy{std::move(rhs)};
  swap(cpy);
  return *this;
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::begin()
{
  return Iterator< T >{data_};
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::end()
{
  return Iterator< T >{data_ + size_};
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::begin() const
{
  return Iterator< T >{data_};
}

template< class T >
hvostov::Iterator< T > hvostov::Vector< T >::end() const
{
  return Iterator< T >{data_ + size_};
}

template< class T >
hvostov::CIterator< T > hvostov::Vector< T >::cbegin() const
{
  return CIterator< T >{data_};
}

template< class T >
hvostov::CIterator< T > hvostov::Vector< T >::cend() const
{
  return CIterator< T >{data_ + size_};
}

template< class T >
bool hvostov::Vector< T >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class T >
size_t hvostov::Vector< T >::size() const noexcept
{
  return size_;
}

template< class T >
size_t hvostov::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void hvostov::Vector< T >::pushBack(const T& v)
{
  if (size_ >= capacity_) {
    Vector< T > cpy(*this);
    size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
    cpy.reserve(new_capacity);
    cpy.unsafePushBack(v);
    swap(cpy);
  } else {
    unsafePushBack(v);
  }
}

template< class T >
void hvostov::Vector< T >::unsafePushBack(const T& v)
{
  data_[size_] = v;
  size_++;
}

template< class T >
void hvostov::Vector< T >::pushBackCount(size_t k, const T& val)
{
  Vector< T > cpy(*this);
  if (size_ + k >= capacity_) {
    cpy.reserve(size_ + k);
  }
  for (size_t i = size_; i < size_ + k; i++) {
    cpy.unsafePushBack(val);
  }
  swap(cpy);
}

template< class T >
template< class IT >
void hvostov::Vector< T >::pushBackRange(IT b, size_t k)
{
  Vector< T > cpy{*this};
  if (size_ + k >= capacity_) {
    cpy.reserve(capacity_ + k);
  }
  for (size_t i = 0; i < k; i++) {
    cpy.unsafePushBack(*b);
    b++;
  }
  swap(cpy);
}

template< class T >
void hvostov::Vector< T >::popBack()
{
  if (size_ > 0) {
    size_--;
    data_[size_].~T();
  }
}

template< class T >
T& hvostov::Vector< T >::operator[](size_t index) noexcept
{
  return data_[index];
}

template< class T >
const T& hvostov::Vector< T >::operator[](size_t index) const noexcept
{
  return data_[index];
}

template< class T >
T& hvostov::Vector< T >::at(size_t index)
{
  const Vector< T >* cthis = this;
  return const_cast< T& >(cthis->at(index));
}

template< class T >
const T& hvostov::Vector< T >::at(size_t index) const
{
  if (index >= size_) {
    throw std::out_of_range("Bad index");
  }
  return data_[index];
}

template< class T >
bool hvostov::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{
  bool is_equal = lhs.size() == rhs.size();
  for (size_t i = 0; (i < lhs.size()) && is_equal; i++) {
    is_equal = is_equal && (lhs[i] == rhs[i]);
  }
  return is_equal;
}

#endif
