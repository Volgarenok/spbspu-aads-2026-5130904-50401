#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iterator>

namespace tarasenko
{
  template< class T >
  struct Vector;

  template< class T >
  struct VecIt
  {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    friend struct Vector< T >;

    VecIt() noexcept;

    T& operator*() const;
    T* operator->() const;

    T& operator[](long long index) const;

    VecIt& operator++();
    VecIt operator++(int);

    VecIt& operator--();
    VecIt operator--(int);

    VecIt& operator+=(long long offset);
    VecIt& operator-=(long long offset);

    VecIt operator+(long long offset) const noexcept;
    VecIt operator-(long long offset) const noexcept;

    long long operator-(const VecIt& another) const noexcept;

    bool operator==(const VecIt& another) const noexcept;
    bool operator!=(const VecIt& another) const noexcept;
    bool operator>(const VecIt& another) const noexcept;
    bool operator<(const VecIt& another) const noexcept;
    bool operator>=(const VecIt& another) const noexcept;
    bool operator<=(const VecIt& another) const noexcept;
  private:
    Vector< T >* owner_;
    size_t index_;

    explicit VecIt(Vector< T >* owner, size_t index) noexcept;
    explicit VecIt(Vector< T >* owner, T* ptr) noexcept;
  };

  template< class T >
  VecIt< T > operator+(long long offset, const VecIt< T >& it) noexcept;

  template< class T >
  struct VecConstIt
  {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    friend struct Vector< T >;

    VecConstIt() noexcept;

    const T& operator*() const;
    const T* operator->() const;

    const T& operator[](long long index) const;

    VecConstIt& operator++();
    VecConstIt operator++(int);

    VecConstIt& operator--();
    VecConstIt operator--(int);

    VecConstIt& operator+=(long long offset);
    VecConstIt& operator-=(long long offset);

    VecConstIt operator+(long long offset) const noexcept;
    VecConstIt operator-(long long offset) const noexcept;

    long long operator-(const VecConstIt& another) const noexcept;

    bool operator==(const VecConstIt& another) const noexcept;
    bool operator!=(const VecConstIt& another) const noexcept;
    bool operator>(const VecConstIt& another) const noexcept;
    bool operator<(const VecConstIt& another) const noexcept;
    bool operator>=(const VecConstIt& another) const noexcept;
    bool operator<=(const VecConstIt& another) const noexcept;
  private:
    const Vector< T >* owner_;
    size_t index_;

    explicit VecConstIt(const Vector< T >* owner, size_t index) noexcept;
    explicit VecConstIt(const Vector< T >* owner, T* ptr) noexcept;
  };

  template< class T >
  VecConstIt< T > operator+(long long offset, const VecConstIt< T >& it) noexcept;

  template< class T >
  struct Vector
  {

    friend struct VecIt< T >;

    Vector();
    ~Vector();
    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    explicit Vector(size_t size, const T& init);
    explicit Vector(std::initializer_list< T >);
    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void reserve(size_t required);
    void shrinkToFit();

    T& operator[](size_t index) noexcept;
    const T& operator[](size_t index) const noexcept;
    T& at(size_t index);
    const T& at(size_t index) const;

    void pushBack(const T& v);
    void pushBackCount(size_t k, const T& val);
    void unsafePushBack(const T& val);
    template< class IT >
    void pushBackRange(IT begin, size_t c);
    void popBack();

    void insert(size_t i, const T& elem);
    void insert(size_t i, const Vector< T >& rhs, size_t start, size_t end);
    void insert(VecIt< T > pos, const T& elem);
    template < class FwdIt >
    void insert(VecIt< T > pos, FwdIt begin, size_t size);
    template < class It >
    void insert(VecIt< T > pos, It from);
    void erase(size_t i);
    void erase(size_t start, size_t end);
    void erase(VecIt< T > pos);
    void erase(VecIt< T > pos, size_t size);
    void erase(VecIt< T > start, VecIt< T > end);
    template< class Predicate >
    void erase(VecIt< T > start, VecIt< T > end, Predicate func);
    void extend(size_t new_cap);

    void swap(Vector< T >& rhs) noexcept;

    VecIt< T > begin() noexcept;
    VecIt< T > end() noexcept;

    VecConstIt< T > begin() const noexcept;
    VecConstIt< T > end() const noexcept;

    VecConstIt< T > cbegin() const noexcept;
    VecConstIt< T > cend() const noexcept;

  private:
    T* data_;
    size_t size_;
    size_t cap_;

    explicit Vector(size_t size);
  };

  template< class T >
  bool operator==(const Vector< T >& lhs, const Vector< T >& rhs);
}

template< class T >
tarasenko::Vector< T >::Vector():
data_(nullptr),
size_(0),
cap_(0)
{}

template< class T >
tarasenko::Vector< T >::~Vector()
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  ::operator delete (data_);
}

template< class T >
bool tarasenko::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t tarasenko::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t tarasenko::Vector< T >::getCapacity() const noexcept
{
  return cap_;
}

template< class T >
void tarasenko::Vector< T >::extend(size_t new_cap)
{
  if (new_cap <= cap_)
  {
    return;
  }

  size_t created = 0;
  T* new_data = static_cast< T* >(::operator new(sizeof(T) * new_cap));
  try
  {
    for (size_t i = 0; i < getSize(); ++i)
    {
      new (new_data + i) T(std::move(data_[i]));
      ++created;
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < created; ++i)
    {
      new_data[i].~T();
    }
    ::operator delete(new_data);
    throw;
  }

  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  ::operator delete(data_);
  data_ = new_data;
  cap_ = new_cap;
}

template< class T >
void tarasenko::Vector< T >::pushBack(const T& v)
{
  if (size_ == cap_)
  {
    extend(cap_ ? cap_ * 2 : 2);
  }
  new (data_ + size_) T(v);
  ++size_;
}

template< class T >
void tarasenko::Vector< T >::popBack()
{
  if (getSize())
  {
    data_[size_ - 1].~T();
    --size_;
  }
}

template< class T >
T& tarasenko::Vector< T >::operator[](size_t index) noexcept
{
  const Vector< T >* cthis = this;
  return const_cast< T& >((*cthis)[index]);
}

template< class T >
const T& tarasenko::Vector< T >::operator[](size_t index) const noexcept
{
  return data_[index];
}

template< class T >
T& tarasenko::Vector< T >::at(size_t index)
{
  const Vector< T >* cthis = this;
  return const_cast< T& >(cthis->at(index));
}

template< class T >
const T& tarasenko::Vector< T >::at(size_t index) const
{
  if (index < getSize())
  {
    return (*this)[index];
  }
  throw std::out_of_range("bad index");
}

template< class T >
tarasenko::Vector< T >::Vector(const Vector< T >& rhs):
  Vector(rhs.getSize())
{
  try
  {
    for (; size_ < rhs.getSize(); ++size_)
    {
      new (data_ + size_) T(rhs[size_]);
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    size_ = 0;
    throw;
  }
}

template< class T >
tarasenko::Vector< T >::Vector(size_t size):
  data_(static_cast< T* >(::operator new(sizeof(T) * size))),
  size_(0),
  cap_(size)
{}

template< class T >
tarasenko::Vector< T >::Vector(size_t size, const T& init):
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
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    size_ = 0;
    throw;
  }
}

template< class T >
bool tarasenko::operator==(const Vector< T >& lhs, const Vector< T >& rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

template< class T >
tarasenko::Vector< T >& tarasenko::Vector< T >::operator=(const Vector< T >& rhs)
{
  Vector< T > copy = rhs;
  swap(copy);
  return *this;
}

template< class T >
void tarasenko::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(cap_, rhs.cap_);
}

template< class T >
tarasenko::Vector< T >::Vector(Vector&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  cap_(rhs.cap_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.cap_ = 0;
}

template< class T >
tarasenko::Vector< T >& tarasenko::Vector< T >::operator=(Vector&& rhs) noexcept
{
  Vector< T > copy = std::move(rhs);
  swap(copy);
  return *this;
}

template< class T >
void tarasenko::Vector< T >::insert(size_t i, const T& elem)
{
  size_t new_size = size_ + 1;
  Vector< T > copy(new_size);
  for (size_t k = 0; k < i; ++k)
  {
    new (copy.data_ + k) T((*this)[k]);
    ++copy.size_;
  }
  new (copy.data_ + i) T(elem);
  ++copy.size_;
  for (size_t k = i; k < size_; ++k)
  {
    new (copy.data_ + k + 1) T ((*this)[k]);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
void tarasenko::Vector< T >::insert(size_t i, const Vector< T >& rhs, size_t start, size_t end)
{
  size_t quantity = end - start;
  size_t new_size = size_ + quantity;
  Vector< T > copy(new_size);
  for (size_t k = 0; k < i; ++k)
  {
    new (copy.data_ + k) T((*this)[k]);
    ++copy.size_;
  }
  for (size_t k = i; k < i + quantity; ++k)
  {
    new (copy.data_ + k) T(rhs[start + k - i]);
    ++copy.size_;
  }
  for (size_t k = i + quantity; k < new_size; ++k)
  {
    new (copy.data_ + k) T((*this)[k - quantity]);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
void tarasenko::Vector< T >::erase(size_t i)
{
  size_t new_size = size_ - 1;
  Vector< T > copy(new_size);
  for (size_t k = 0; k < i; ++k)
  {
    new (copy.data_ + k) T((*this)[k]);
    ++copy.size_;
  }
  for (size_t k = i; k < new_size; ++k)
  {
    new (copy.data_ + k) T((*this)[k + 1]);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
void tarasenko::Vector< T >::erase(size_t start, size_t end)
{
  size_t quantity = end - start;
  size_t new_size = size_ - quantity;
  Vector< T > copy(size_ - quantity);
  for (size_t i = 0; i < start; ++i)
  {
    new (copy.data_ + i) T((*this)[i]);
    ++copy.size_;
  }
  for (size_t i = start; i < new_size; ++i)
  {
    new (copy.data_ + i) T((*this)[i + quantity]);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
tarasenko::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  try
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      new (data_ + size_) T(*it);
      ++size_;
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    size_ = 0;
    throw;
  }
}

template< class T >
tarasenko::VecIt< T > tarasenko::Vector< T >::begin() noexcept
{
  return tarasenko::VecIt< T >(this, static_cast< size_t >(0));
}

template< class T >
tarasenko::VecIt< T > tarasenko::Vector< T >::end() noexcept
{
  return tarasenko::VecIt< T >(this, static_cast< size_t >(size_));
}

template< class T >
tarasenko::VecIt< T >::VecIt() noexcept:
  owner_(nullptr),
  index_(0)
{}

template< class T >
tarasenko::VecIt< T >::VecIt(tarasenko::Vector< T >* owner, size_t index) noexcept:
  owner_(owner),
  index_(index)
{}

template< class T >
T& tarasenko::VecIt< T >::operator*() const
{
  return (*owner_)[index_];
}

template< class T >
T* tarasenko::VecIt< T >::operator->() const
{
  return &(**this);
}

template< class T >
T& tarasenko::VecIt< T >::operator[](long long index) const
{
  return *(*this + index);
}

template< class T >
tarasenko::VecIt< T >& tarasenko::VecIt< T >::operator++()
{
  ++index_;
  return *this;
}

template< class T >
tarasenko::VecIt< T > tarasenko::VecIt< T >::operator++(int)
{
  auto temp = *this;
  ++index_;
  return temp;
}

template< class T >
tarasenko::VecIt< T >& tarasenko::VecIt< T >::operator--()
{
  --index_;
  return *this;
}

template< class T >
tarasenko::VecIt< T > tarasenko::VecIt< T >::operator--(int)
{
  auto temp = *this;
  --index_;
  return temp;
}

template< class T >
bool tarasenko::VecIt< T >::operator==(const VecIt< T >& another) const noexcept
{
  return (owner_ == another.owner_) && (index_ == another.index_);
}

template< class T >
bool tarasenko::VecIt< T >::operator!=(const VecIt< T >& another) const noexcept
{
  return !(*this == another);
}

template< class T >
bool tarasenko::VecIt< T >::operator>(const VecIt& another) const noexcept
{
  return index_ > another.index_;
}

template< class T >
bool tarasenko::VecIt< T >::operator<(const VecIt& another) const noexcept
{
  return index_ < another.index_;
}

template< class T >
bool tarasenko::VecIt< T >::operator>=(const VecIt& another) const noexcept
{
  return !(index_ < another.index_);
}

template< class T >
bool tarasenko::VecIt< T >::operator<=(const VecIt& another) const noexcept
{
  return !(index_ > another.index_);
}

template< class T >
tarasenko::VecIt< T > tarasenko::VecIt< T >::operator+(long long offset) const noexcept
{
  return VecIt< T >(owner_, index_ + offset);
}

template< class T >
tarasenko::VecIt< T > tarasenko::VecIt< T >::operator-(long long offset) const noexcept
{
  return VecIt< T >(owner_, index_ - offset);
}

template< class T >
long long tarasenko::VecIt< T >::operator-(const tarasenko::VecIt< T >& another) const noexcept
{
  return static_cast< long long >(index_) - static_cast< long long >(another.index_);
}

template< class T >
tarasenko::VecIt< T >& tarasenko::VecIt< T >::operator+=(long long offset)
{
  index_ += offset;
  return *this;
}

template< class T >
tarasenko::VecIt< T >& tarasenko::VecIt< T >::operator-=(long long offset)
{
  index_ -= offset;
  return *this;
}

template< class T >
tarasenko::VecIt< T > tarasenko::operator+(long long offset, const tarasenko::VecIt< T >& it) noexcept
{
  return it + offset;
}

template< class T >
tarasenko::VecIt< T >::VecIt(tarasenko::Vector< T >* owner, T* ptr) noexcept:
  owner_(owner),
  index_(ptr - owner->data_)
{}

template< class T >
template< class FwdIt >
void tarasenko::Vector< T >::insert(tarasenko::VecIt< T > pos, FwdIt begin, size_t size)
{
  size_t new_size = size_ + size;
  Vector< T > copy(new_size);
  size_t i = 0;
  for (auto it = this->begin(); it != pos; ++it, ++i)
  {
    new (copy.data_ + i) T(*it);
    ++copy.size_;
  }
  for (size_t k = 0; k < size; ++k, ++i)
  {
    new (copy.data_ + i) T(*(begin++));
    ++copy.size_;
  }
  for (; pos != end(); ++pos, ++i)
  {
    new (copy.data_ + i) T(*pos);
    ++copy.size_;
  }
  swap(copy);
}

template <class T >
void tarasenko::Vector< T >::erase(tarasenko::VecIt< T > pos)
{
  size_t new_size = size_ - 1;
  Vector< T > copy(new_size);
  size_t i = 0;
  for (auto it = begin(); it != pos; ++it, ++i)
  {
    new (copy.data_ + i) T(*it);
    ++copy.size_;
  }
  for (++pos; pos != end(); ++pos, ++i)
  {
    new (copy.data_ + i) T(*pos);
    ++copy.size_;
  }
  swap(copy);
}

template <class T >
void tarasenko::Vector< T >::insert(tarasenko::VecIt< T > pos, const T& elem)
{
  size_t new_size = size_ + 1;
  Vector< T > copy(new_size);
  size_t i = 0;
  for (auto it = begin(); it != pos; ++it, ++i)
  {
    new (copy.data_ + i) T(*it);
    ++copy.size_;
  }
  new (copy.data_ + i) T(elem);
  ++copy.size_;
  for (++i; pos != end(); ++pos, ++i)
  {
    new (copy.data_ + i) T(*pos);
    ++copy.size_;
  }
  swap(copy);
}

template <class T >
void tarasenko::Vector< T >::erase(tarasenko::VecIt< T > pos, size_t size)
{
  size_t new_size = size_ - size;
  Vector< T > copy(new_size);
  size_t i = 0;
  for (auto it = begin(); it != pos; ++it, ++i)
  {
    new (copy.data_ + i) T(*it);
    ++copy.size_;
  }
  for (pos += size; pos != end(); ++pos, ++i)
  {
    new (copy.data_ + i) T(*pos);
    ++copy.size_;
  }
  swap(copy);
}

template <class T >
void tarasenko::Vector< T >::erase(tarasenko::VecIt< T > start, tarasenko::VecIt< T > end)
{
  erase(start, end - start);
}

template< class T >
template< class It >
void tarasenko::Vector< T >::insert(VecIt< T > pos, It from)
{
  insert(pos, *from);
}

template< class T >
template< class Predicate >
void tarasenko::Vector< T >::erase(tarasenko::VecIt< T > start, tarasenko::VecIt< T > end, Predicate func)
{
  size_t to_remove = 0;
  for (auto it = start; it != end; ++it)
  {
    if (func(*it))
    {
      ++to_remove;
    }
  }
  size_t new_size = size_ - to_remove;
  Vector< T > copy(new_size);
  size_t i = 0;
  for (auto it = begin(); it != start; ++it, ++i)
  {
    new (copy.data_ + i) T(*it);
    ++copy.size_;
  }
  for (auto it = start; it != end; ++it)
  {
    if (!func(*it))
    {
      new (copy.data_ + i) T(*it);
      ++i;
      ++copy.size_;
    }
  }
  for (auto it = end; it != this->end(); ++it, ++i)
  {
    new (copy.data_ + i) T(*it);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
void tarasenko::Vector< T >::reserve(size_t required)
{
  if (required > cap_)
  {
    extend(required);
  }
}

template< class T >
void tarasenko::Vector< T >::shrinkToFit()
{
  if (size_ == cap_)
  {
    return;
  }

  Vector < T > copy(size_);
  for (size_t i = 0; i < size_; ++i)
  {
    new (copy.data_ + i) T(data_[i]);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
void tarasenko::Vector< T >::pushBackCount(size_t k, const T& val)
{
  size_t new_size = size_ + k;
  Vector< T > copy(new_size);
  for (size_t i = 0; i < size_; ++i)
  {
    new (copy.data_ + copy.size_) T(data_[i]);
    ++copy.size_;
  }
  for (size_t i = 0; i < k; ++i)
  {
    new (copy.data_ + copy.size_) T(val);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
template< class IT >
void tarasenko::Vector< T >::pushBackRange(IT begin, size_t c)
{
  size_t new_size = size_ + c;
  Vector< T > copy(new_size);
  IT it = begin;
  for (size_t i = 0; i < size_; ++i)
  {
    new (copy.data_ + copy.size_) T(data_[i]);
    ++copy.size_;
  }
  for (size_t i = 0; i < c; ++i, ++it)
  {
    new (copy.data_ + copy.size_) T(*it);
    ++copy.size_;
  }
  swap(copy);
}

template< class T >
tarasenko::VecConstIt< T >::VecConstIt() noexcept:
  owner_(nullptr),
  index_(0)
{}

template< class T >
tarasenko::VecConstIt< T >::VecConstIt(const tarasenko::Vector< T >* owner, size_t index) noexcept:
  owner_(owner),
  index_(index)
{}

template< class T >
tarasenko::VecConstIt< T >::VecConstIt(const tarasenko::Vector< T >* owner, T* ptr) noexcept:
  owner_(owner),
  index_(ptr - owner->data_)
{}

template< class T >
tarasenko::VecConstIt< T >& tarasenko::VecConstIt< T >::operator++()
{
  ++index_;
  return *this;
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::VecConstIt< T >::operator++(int)
{
  auto temp = *this;
  ++index_;
  return temp;
}

template< class T >
tarasenko::VecConstIt< T >& tarasenko::VecConstIt< T >::operator--()
{
  --index_;
  return *this;
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::VecConstIt< T >::operator--(int)
{
  auto temp = *this;
  --index_;
  return temp;
}

template< class T >
const T& tarasenko::VecConstIt< T >::operator*() const
{
  return (*owner_)[index_];
}

template< class T >
const T* tarasenko::VecConstIt< T >::operator->() const
{
  return &(**this);
}

template< class T >
bool tarasenko::VecConstIt< T >::operator==(const VecConstIt< T >& another) const noexcept
{
  return (owner_ == another.owner_) && (index_ == another.index_);
}

template< class T >
bool tarasenko::VecConstIt< T >::operator!=(const VecConstIt< T >& another) const noexcept
{
  return !(*this == another);
}

template< class T >
bool tarasenko::VecConstIt< T >::operator>(const VecConstIt< T >& another) const noexcept
{
  return index_ > another.index_;
}

template< class T >
bool tarasenko::VecConstIt< T >::operator<(const VecConstIt< T >& another) const noexcept
{
  return index_ < another.index_;
}

template< class T >
bool tarasenko::VecConstIt< T >::operator>=(const VecConstIt< T >& another) const noexcept
{
  return !(index_ < another.index_);
}

template< class T >
bool tarasenko::VecConstIt< T >::operator<=(const VecConstIt< T >& another) const noexcept
{
  return !(index_ > another.index_);
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::VecConstIt< T >::operator+(long long offset) const noexcept
{
  return VecIt< T >(owner_, index_ + offset);
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::VecConstIt< T >::operator-(long long offset) const noexcept
{
  return VecIt< T >(owner_, index_ - offset);
}

template< class T >
long long tarasenko::VecConstIt< T >::operator-(const VecConstIt< T >& another) const noexcept
{
  return static_cast< long long >(index_) - static_cast< long long >(another.index_);
}

template< class T >
tarasenko::VecConstIt< T >& tarasenko::VecConstIt< T >::operator+=(long long offset)
{
  index_ += offset;
  return *this;
}

template< class T >
tarasenko::VecConstIt< T >& tarasenko::VecConstIt< T >::operator-=(long long offset)
{
  index_ -= offset;
  return *this;
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::operator+(long long offset, const tarasenko::VecConstIt< T >& it) noexcept
{
  return it + offset;
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::Vector< T >::begin() const noexcept
{
  return tarasenko::VecConstIt< T >(this, static_cast< size_t >(0));
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::Vector< T >::end() const noexcept
{
  return tarasenko::VecConstIt< T >(this, static_cast< size_t >(size_));
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::Vector< T >::cbegin() const noexcept
{
  return tarasenko::VecConstIt< T >(this, static_cast< size_t >(0));
}

template< class T >
tarasenko::VecConstIt< T > tarasenko::Vector< T >::cend() const noexcept
{
  return tarasenko::VecConstIt< T >(this, static_cast< size_t >(size_));
}

#endif
