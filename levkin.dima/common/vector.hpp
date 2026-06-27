#ifndef CUSTOM_VECTOR
#define CUSTOM_VECTOR

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace stuff {
  template < class T > inline T* newc(size_t);
  template < class T > inline void delc(T*, size_t);
  template < class T > inline void destroy(T*);
  template < class T > inline void destroyRange(T*, size_t);
  template < class T, class... Args > inline void construct(T*, Args&&... args);

  template < typename T > class VCIter;
  template < class T > class VIter
  {
  public:
    T* curr;
    VIter(T* ptr) : curr(ptr) {}
    T& operator*() { return *(curr); }
    VIter& operator++()
    {
      curr++;
      return *this;
    }

    VIter operator++(int)
    {
      VIter temp = *this;
      ++(*this);
      return temp;
    }

    VIter& operator--()
    {
      --curr;
      return *this;
    }

    VIter operator--(int)
    {
      VIter temp = *this;
      --(*this);
      return temp;
    }
    bool operator==(const VIter& other) const { return curr == other.curr; }
    bool operator!=(const VIter& other) const { return !(*this == other); }

    bool operator==(const stuff::VCIter< T >& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const VCIter< T >& other) const
    {
      return curr != other.curr;
    }
    VIter operator+(size_t n) const { return VIter(curr + n); }
    VIter operator-(size_t n) const { return VIter(curr - n); }
  };
  template < class T > class VCIter
  {
  public:
    T* curr;
    VCIter(T* ptr) : curr(ptr) {}
    const T& operator*() { return *(curr); }
    VCIter& operator++()
    {
      curr++;
      return *this;
    }

    VCIter operator++(int)
    {
      VCIter temp = *this;
      ++(*this);
      return temp;
    }

    VCIter& operator--()
    {
      --curr;
      return *this;
    }

    VCIter operator--(int)
    {
      VCIter temp = *this;
      --(*this);
      return temp;
    }
    bool operator==(const VCIter& other) const { return curr == other.curr; }
    bool operator!=(const VCIter& other) const { return !(*this == other); }

    bool operator==(const VIter< T >& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const VIter< T >& other) const
    {
      return curr != other.curr;
    }
    VCIter operator+(size_t n) const { return VCIter(curr + n); }
    VCIter operator-(size_t n) const { return VCIter(curr - n); }
  };
  template < class T > struct Vector {
    ~Vector();
    Vector();
    Vector(const Vector&);
    Vector(Vector< T >&& rhs) noexcept;
    explicit Vector(std::initializer_list< T > il);
    Vector& operator=(const Vector&);
    Vector& operator=(Vector< T >&& rhs) noexcept;
    T& operator[](size_t) noexcept;
    const T& operator[](size_t) const noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void pushBack(const T& v);
    void popBack();
    void insert(size_t i, const T& v);
    void insertBefore(VIter< T > it, const T&& v);

    void erase(size_t i);
    void erase(VIter< T > it);
    void erase(VIter< T > from, VIter< T > till);
    T& at(size_t index);
    const T& at(size_t index) const;
    void pushBackCount(size_t k, const T& val);
    void reserve(size_t);

    void shrinkToFit();
    template < class IT > void pushBackRange(IT b, size_t c);
    VIter< T > begin();
    VCIter< T > cbegin();
    VIter< T > end();
    VCIter< T > cend();

  private:
    explicit Vector(size_t);
    void swap(Vector< T >&) noexcept;
    void expand();
    void reallocate(size_t new_cap);
    void expandIfFull();
    void unsafePushBack();
    T* data_;
    size_t size_, capacity_;
  };

  template < class T >
  bool operator==(const stuff::Vector< T >& lhs, const stuff::Vector< T >& rhs);
  template < class T >
  std::ostream& operator<<(std::ostream& os, const Vector< T >& v);
  template < class T > VIter< T > Vector< T >::begin()
  {
    return VIter< T >(data_);
  }

  template < class T > VIter< T > Vector< T >::end()
  {
    return VIter< T >(data_ + size_);
  }

  template < class T > VCIter< T > Vector< T >::cbegin()
  {
    return VCIter< T >(data_);
  }

  template < class T > VCIter< T > Vector< T >::cend()
  {
    return VCIter< T >(data_ + size_);
  }

}

template < class T >
template < class IT >
void stuff::Vector< T >::pushBackRange(IT b, size_t c)
{
  if (size_ + c > capacity_) {
    reserve(size_ + c);
  }
  for (size_t i = 0; i < c; ++i) {
    data_[size_++] = *b;
    ++b;
  }
}

template < class T > void stuff::Vector< T >::reserve(size_t new_cap)
{
  if (new_cap < size_)
    return;
  reallocate(new_cap);
}

template < class T > void stuff::Vector< T >::reallocate(size_t new_cap)
{
  T* nw = newc< T >(new_cap);

  size_t progress = 0;
  size_t till = std::min(size_, new_cap);
  try {
    for (; progress < till; ++progress) {
      stuff::construct< T >(&nw[progress], std::move(data_[progress]));
    }
  } catch (...) {
    delc(nw, progress);
    throw;
  }
  delc(data_, size_);
  data_ = nw;
  capacity_ = new_cap;
}

template < class T > void stuff::Vector< T >::shrinkToFit() { reserve(size_); }

template < class T >
void stuff::Vector< T >::pushBackCount(size_t k, const T& val)
{
  if (size_ + k > capacity_) {
    reserve(size_ + k);
  }
  for (size_t i = 0; i < k; ++i) {
    data_[size_++] = val;
  }
}

template < class T >
stuff::Vector< T >::Vector(std::initializer_list< T > il) : Vector(il.size())
{
  for (auto it = il.begin(); it != il.end(); ++it) {
    data_[size_++] = *it;
  }
}

template < class T >
stuff::Vector< T >::Vector(size_t cap)
    : data_(newc< T >(cap)), size_(0), capacity_(cap)
{
}
template < class T >
stuff::Vector< T >& stuff::Vector< T >::operator=(const Vector< T >& rhs)
{
  Vector< T > copiedVec = rhs;
  swap(copiedVec);
  return *this;
}

template < class T >
bool stuff::operator==(
    const stuff::Vector< T >& lhs, const stuff::Vector< T >& rhs)
{
  bool IsEqual = lhs.getSize() == rhs.getSize();

  for (size_t i = 0;
       i < lhs.getSize() && (IsEqual = IsEqual && (lhs[i] == rhs[i])); ++i) {
  }
  return IsEqual;
}

template < class T >
stuff::Vector< T >::Vector(const Vector< T >& rhs)
    : data_(nullptr), size_(rhs.size_), capacity_(rhs.size_)
{
  if (capacity_ > 0) {
    data_ = newc< T >(capacity_);
    for (size_t i = 0; i < size_; ++i) {
      try {
        stuff::construct< T >(&data_[i], rhs.data_[i]);
      } catch (...) {
        delc(data_, i);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }
}

template < class T > stuff::Vector< T >::~Vector() { delc(data_, size_); }

template < class T > void stuff::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template < class T >
stuff::Vector< T >::Vector() : data_(nullptr), size_(0), capacity_(0)
{
}

template < class T > const T& stuff::Vector< T >::at(size_t index) const
{
  if (index >= size_)
    throw std::out_of_range("index out of bounds");
  return data_[index];
}

template < class T > T& stuff::Vector< T >::at(size_t index)
{
  return const_cast< T& >(static_cast< const Vector< T >& >(*this).at(index));
}

template < class T > bool stuff::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template < class T > void stuff::Vector< T >::expand()
{
  size_t nw_capacity = capacity_ + (capacity_ >> 1) + 1;
  T* nw = nullptr;
  size_t creationProgress = 0;
  try {

    nw = newc< T >(nw_capacity);
    for (size_t i = 0; i < size_; ++i) {
      stuff::construct< T >(&nw[i], std::move(data_[i]));
      creationProgress++;
    }

    delc(data_, size_);

    data_ = nw;
    capacity_ = nw_capacity;

  } catch (...) {
    delc(nw, creationProgress);
    throw;
  }
}

template < class T > void stuff::Vector< T >::expandIfFull()
{
  if (size_ == capacity_) {
    expand();
  }
}

template < class T > T& stuff::Vector< T >::operator[](size_t i) noexcept
{
  return data_[i];
}

template < class T >
const T& stuff::Vector< T >::operator[](size_t i) const noexcept
{
  return data_[i];
}

template < class T > void stuff::Vector< T >::pushBack(const T& val)
{
  expandIfFull();
  stuff::construct< T >(&data_[size_++], val);
}

template < class T > size_t stuff::Vector< T >::getSize() const noexcept
{
  return size_;
}

template < class T > size_t stuff::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template < class T > void stuff::Vector< T >::popBack()
{
  if (size_) {
    size_--;
  }
}

template < class T > void stuff::Vector< T >::insert(size_t index, const T& val)
{
  if (index > size_) {
    throw std::out_of_range(
        "Wrong place to insert (maybe i'm gonna add something interesting like "
        "adding it to the end in this case or just place it the that  position "
        "and fill the gap with garbage ");
  }

  expandIfFull();

  if (size_ > index) {
    stuff::construct< T >(&data_[size_], std::move(data_[size_ - 1]));

    for (size_t i = size_ - 1; i > index; --i) {
      data_[i] = std::move(data_[i - 1]);
    }

    data_[index] = val;
  } else {
    stuff::construct< T >(&data_[index], val);
  }

  size_++;
}

template < class T >
stuff::Vector< T >::Vector(Vector< T >&& rhs) noexcept
    : data_(nullptr), size_(0), capacity_(0)
{
  swap(rhs);
}

template < class T >
stuff::Vector< T >& stuff::Vector< T >::operator=(Vector< T >&& rhs) noexcept
{
  if (this == &rhs)
    return *this;
  swap(rhs);
  return *this;
}

template < class T > void stuff::Vector< T >::erase(size_t index)
{
  if (index >= size_) {
    throw std::out_of_range("index oout of bounds for erase");
  }

  for (size_t i = index; i < size_ - 1; ++i) {
    data_[i] = data_[i + 1];
  }
  size_--;
}

template < class T > void stuff::Vector< T >::erase(stuff::VIter< T > it)
{
  erase(it.curr - data_);
}
template < class T >
void stuff::Vector< T >::erase(stuff::VIter< T > from, stuff::VIter< T > till)
{
  if (from == till)
    return;

  size_t start = from.curr - data_;
  size_t span = till.curr - from.curr;
  size_t end = start + span;

  for (size_t i = end; i < size_; ++i) {
    data_[i - span] = std::move(data_[i]);
  }

  size_ -= span;

  destroyRange(data_ + size_, span);
}

template < class T > T* stuff::newc(size_t size)
{
  return static_cast< T* >(::operator new(sizeof(T) * size));
}

template < class T > void stuff::delc(T* ptr, size_t len)
{
  destroyRange(ptr, len);
  ::operator delete(ptr);
}

template < class T > void stuff::destroyRange(T* ptr, size_t len)
{
  for (size_t i = 0; i < len; ++i) {
    destroy(&ptr[i]);
  }
}

template < class T > inline void stuff::destroy(T* ptr) { ptr->~T(); }

template < class T, class... Args >
inline void stuff::construct(T* place, Args&&... args)
{
  new (place) T(std::forward< Args >(args)...);
}

template < class T >
std::ostream& stuff::operator<<(std::ostream& os, const Vector< T >& v)
{
  os << "[";
  for (size_t i = 0; i < v.getSize(); ++i) {
    os << v[i] << (i == v.getSize() - 1 ? "" : ", ");
  }
  os << "]";
  return os;
}

#endif
