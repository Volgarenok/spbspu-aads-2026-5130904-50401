#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include "top-it-iterator.hpp"
#include "top-it-const-iterator.hpp"
namespace topit
{
  template< class T >
  struct Vector {
  public:
    ~Vector();

    Vector();
    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    Vector(size_t size, const T& init);
    explicit Vector(std::initializer_list< T > il);
    Vector< T >& operator=(const Vector&);
    Vector< T >& operator=(Vector&&) noexcept;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;

    T& at(size_t it);
    const T& at(size_t it) const;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void reserve(size_t newCapacity);
    void shrimpToFit();

    VecIter< T > begin() const;
    VecConstIter< T > cbegin() const;
    VecIter< T > end() const;
    VecConstIter< T > cend() const;

    void pushBack(const T& v);
    void pushBackCount(size_t numberOfElements, const T& value);
    template< class IT >
    void pushBackRange(IT beginIterator, size_t numberOfElements);

    void insert(size_t position, const T& value);
    VecIter< T > insert(VecIter< T > position, const T& value);
    VecIter< T > insert(VecIter< T > position, size_t numberOfElements, const T& value);
    VecIter< T > insert(VecIter< T > position, VecIter< T > beginIterator, size_t numberOfElements);
    void insert(size_t position, const Vector< T >& anotherVector, size_t start, size_t end);

    void popBack();
    void erase(size_t position);
    void erase(size_t start, size_t end);
    void erase(VecIter< T > pos);
    void erase(VecIter< T > pos, size_t s);
    void erase(VecIter< T > start, VecIter< T > end);
    void swap(Vector< T >& rhs) noexcept;

  private:
    void unsafePushBack(const T& value);
    explicit Vector(size_t size);
    void destroy();
    T* data_;
    size_t size_, capacity_;
  };
  template< class T >
  bool operator==(const Vector< T >& rhs, const Vector< T >& lhs);
}

template< class T >
void topit::Vector< T >::destroy()
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  size_ = 0;
}


template< class T >
topit::Vector< T >::~Vector()
{
  destroy();
  ::operator delete (data_);
}

template< class T >
topit::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
topit::Vector< T >::Vector(const Vector& rhs):
  Vector(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); ++i)
  {
    new (data_ + i) T(rhs[i]);
  }
}

template< class T >
topit::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template< class T >
topit::Vector< T >::Vector(size_t size, const T& init):
  Vector(size)
{
  for (size_t i = 0; i < size; ++i)
  {
    new (data_ + i) T(init);
  }
}

template< class T >
topit::Vector< T >::Vector(size_t size):
  data_(size ? static_cast< T* >(::operator new (size * sizeof(T))) : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
topit::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it, ++i)
  {
    new (data_ + i) T(*it);
  }
}


template< class T >
topit::Vector< T >& topit::Vector< T >::operator=(const Vector< T >& rhs)
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
topit::Vector< T >& topit::Vector< T >::operator=(Vector< T >&& rhs) noexcept
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }

  Vector< T >cpy(std::move(rhs));
  swap(cpy);
  return *this;
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
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
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
topit::VecIter< T > topit::Vector< T >::begin() const
{
  return {data_};
}

template< class T >
topit::VecIter< T > topit::Vector< T >::end() const
{
  return {data_ + size_};
}

template< class T >
topit::VecConstIter< T > topit::Vector< T >::cbegin() const
{
  return {data_};
}

template< class T >
topit::VecConstIter< T > topit::Vector< T >::cend() const
{
  return {data_ + size_};
}

template< class T >
void topit::Vector< T >::reserve(size_t newCapacity)
{
  T* newData = static_cast< T* >(::operator new (newCapacity * sizeof(T)));
  size_t i = 0;
  try
  {
    for (; i < std::min(newCapacity, size_); ++i)
    {
      new (newData + i) T(data_[i]);
    }
  }
  catch(...)
  {
    for (size_t j = 0; j < i; ++j)
    {
      newData[j].~T();
    }
    ::operator delete (newData);
    throw;
  }
  destroy();
  ::operator delete (data_);
  data_ = newData;
  capacity_ = newCapacity;
  size_ = i;
}

template< class T >
void topit::Vector< T >::shrimpToFit()
{
  reserve(size_);
}

template< class T >
void topit::Vector< T >::pushBack(const T& value)
{
  if (size_ >= capacity_)
  {
    Vector< T > temp(*this);
    temp.reserve(temp.capacity_ == 0 ? 8 : temp.capacity_ * 2);
    temp.unsafePushBack(value);
    swap(temp);
  } else
  {
    unsafePushBack(value);
  }
}

template< class T >
template< class IT >
void topit::Vector< T >::pushBackRange(IT beginIterator, size_t numberOfElements)
{
  if (size_ + numberOfElements >= capacity_)
  {
    Vector< T > temp(*this);
    temp.reserve(std::max(size_ + numberOfElements, temp.capacity_ == 0 ? 8 : capacity_ * 2));
    for (size_t i = 0; i < numberOfElements; ++i)
    {
      temp.unsafePushBack(*beginIterator);
      ++beginIterator;
    }
    swap(temp);
  } else
  {
    for (size_t i = 0; i < numberOfElements; ++i)
    {
      unsafePushBack(*beginIterator);
      ++beginIterator;
    }
  }
}


template< class T >
void topit::Vector< T >::pushBackCount(size_t numberOfElements, const T& value)
{
  if (size_ + numberOfElements >= capacity_)
  {
    Vector< T > temp(*this);
    size_t newSize = temp.size_ + numberOfElements;

    temp.reserve(std::max(newSize, temp.capacity_ == 0 ? 8 : temp.capacity_ * 2));
    for (size_t i = 0; i < numberOfElements; ++i)
    {
      temp.unsafePushBack(value);
    }
    swap(temp);
  } else
  {
    for (size_t i = 0; i < numberOfElements; ++i)
    {
      unsafePushBack(value);
    }
  }
}

template< class T >
void topit::Vector< T >::unsafePushBack(const T& value)
{
  assert(size_ < capacity_);
  new (data_ + size_) T(value);
  ++size_;
}

template< class T >
void topit::Vector< T >::popBack()
{
  if (!size_)
  {
    return;
  }
  data_[size_ - 1].~T();
  --size_;
}

template< class T >
topit::VecIter< T > topit::Vector< T >::insert(VecIter< T > position, const T& value)
{
  size_t pos = position - begin();
  insert(pos, value);
  return position + 1;
}

template< class T >
topit::VecIter< T > topit::Vector< T >::insert(VecIter< T > position, size_t numberOfElements, const T& value)
{
  if (position < begin() || position > end())
  {
    throw std::out_of_range("Insert: Iterator is out of range");
  }

  Vector< T > temp(*this);
  size_t startPosition = position - begin();
  size_t newSize = temp.size_ + numberOfElements;
  if (temp.size_ + numberOfElements >= temp.capacity_)
  {
    temp.reserve(std::max(newSize, temp.capacity_ == 0 ? 8 : temp.capacity_ * 2));
  }
  for (size_t i = temp.size_; i > startPosition; --i)
  {
    new (temp.data_ + i + numberOfElements - 1) T(std::move(temp.data_[i - 1]));
    temp.data_[i - 1].~T();
  }
  for (size_t i = 0; i < numberOfElements; ++i)
  {
    new (&temp.data_[startPosition++]) T(value);
  }
  temp.size_ += numberOfElements;
  swap(temp);
  return position + numberOfElements;
}

template< class T >
topit::VecIter< T > topit::Vector< T >::insert(VecIter< T > position, VecIter< T > startIterator, size_t numberOfElements)
{
  if (position < begin() || position > end())
  {
    throw std::out_of_range("Insert: Iterator is out of range");
  }

  Vector< T > temp(*this);
  size_t startPosition = position - begin();
  size_t newSize = temp.size_ + numberOfElements;
  if (temp.size_ + numberOfElements >= temp.capacity_)
  {
    temp.reserve(std::max(newSize, temp.capacity_ == 0 ? 8 : temp.capacity_ * 2));
  }
  for (size_t i = temp.size_; i > startPosition; --i)
  {
    new (temp.data_ + i + numberOfElements - 1) T(std::move(temp.data_[i - 1]));
    temp.data_[i - 1].~T();
  }
  for (size_t i = 0; i < numberOfElements; ++i, ++startIterator)
  {
    new (&temp.data_[startPosition++]) T(*startIterator);
  }
  temp.size_ += numberOfElements;
  swap(temp);
  return position + numberOfElements;
}

template< class T >
void topit::Vector< T >::insert(size_t position, const T& value)
{
  if (position > size_)
  {
    throw std::out_of_range("Insert: Position is out of range");
  }

  Vector< T > temp(*this);
  if (temp.size_ >= temp.capacity_)
  {
    temp.reserve(temp.capacity_ == 0 ? 8 : temp.capacity_ * 2);
  }
  for (size_t i = temp.size_; i > position; --i)
  {
    new (temp.data_ + i) T(std::move(temp.data_[i - 1]));
    temp.data_[i - 1].~T();
  }
  new (&temp.data_[position]) T(value);
  ++temp.size_;
  swap(temp);
}

template< class T >
void topit::Vector< T >::insert(size_t position, const Vector< T >& anotherVector, size_t start, size_t end)
{
  if (start > end)
  {
    throw std::invalid_argument("Insert: start position must be less than end position");
  }
  if (end > anotherVector.size_)
  {
    throw std::out_of_range("Insert: end position is out of range");
  }
  if (position > size_)
  {
    throw std::out_of_range("Insert: Position is out of range");
  }

  size_t numberOfElements = end - start;
  if (numberOfElements == 0)
  {
    return;
  }

  Vector< T > temp(*this);

  size_t newSize = temp.size_ + numberOfElements;
  if (temp.size_ + numberOfElements >= temp.capacity_)
  {
    temp.reserve(std::max(newSize, temp.capacity_ == 0 ? 8 : temp.capacity_ * 2));
  }
  for (size_t i = temp.size_; i > position; --i)
  {
    new (temp.data_ + i + numberOfElements - 1) T(std::move(temp.data_[i - 1]));
    temp.data_[i - 1].~T();
  }
  for (size_t i = start; i < end; ++i)
  {
    new (&temp.data_[position++]) T(anotherVector[i]);
  }
  temp.size_ += numberOfElements;
  swap(temp);
}

template< class T >
void topit::Vector< T >::erase(size_t position)
{
  if (position >= size_)
  {
    throw std::out_of_range("Erase: Position is out of range");
  }

  Vector< T > temp(*this);
  for (size_t i = position; i < temp.size_; ++i)
  {
    temp.data_[i] = std::move(temp.data_[i + 1]);
  }
  temp.data_[temp.size_ - 1].~T();
  --temp.size_;
  swap(temp);
}

template< class T >
void topit::Vector< T >::erase(size_t start, size_t end)
{
  if (start > end)
  {
    throw std::invalid_argument("Erase: start position must be less than end position");
  } else if (end > size_)
  {
    throw std::out_of_range("Erase: end position is out of range");
  }

  size_t numberOfElements = end - start;
  if (numberOfElements == 0)
  {
    return;
  }
  Vector<T> temp(*this);
  for (size_t i = end; i < temp.size_; ++i)
  {
    temp.data_[i - numberOfElements] = std::move(temp.data_[i]);
  }

  for (size_t i = temp.size_ - numberOfElements; i < temp.size_; ++i)
  {
    temp.data_[i].~T();
  }

  temp.size_ -= numberOfElements;
  swap(temp);
}

template< class T >
void topit::Vector< T >::erase(VecIter< T > position)
{
  size_t pos = position - begin();
  erase(pos);
}


template< class T >
void topit::Vector< T >::erase(VecIter< T > position, size_t s)
{
  size_t pos = position - begin();
  erase(pos, pos + s);
}

template< class T >
void topit::Vector< T >::erase(VecIter< T > beginIterator, VecIter< T > endIterator)
{
  if (beginIterator > endIterator)
  {
    throw std::invalid_argument("Erase: beginIterator must be less than endIterator");
  } else if (beginIterator < begin() || endIterator > end())
  {
    throw std::out_of_range("Erase: Iterator is out of range");
  }

  size_t b = beginIterator - begin();
  size_t e = endIterator - begin();
  size_t count = e - b;
  erase(b, b + count);
}


template< class T >
void topit::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
bool topit::operator==(const Vector< T >& rhs, const Vector< T >& lhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

#endif
