#ifndef HASH_ITERS_HPP
#define HASH_ITERS_HPP
#include <vector.hpp>
#include <list.hpp>
#include <cstddef>
#include <utility>

namespace sedov
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class HashConstIter;

  template < class Key, class Value, class Hash, class Equal >
  class HashIter
  {
  public:
    using valType = std::pair< Key, Value >;

    HashIter();

    HashIter & operator++();
    HashIter operator++(int);
    bool operator==(const HashIter & other) const;
    bool operator!=(const HashIter & other) const;
    valType & operator*();
    valType *operator->();

  private:
    Vector< List< valType > > * data_;
    size_t capacity_;
    size_t idx_;
    LIter< valType > listIt_;
    LIter< valType > listEnd_;

    HashIter(Vector< List< valType > > * data, size_t capacity, size_t startIdx);

    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;

    void findValid();
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashConstIter
  {
  public:
    using valType = std::pair< Key, Value >;

    HashConstIter();

    HashConstIter & operator++();
    HashConstIter operator++(int);
    bool operator==(const HashConstIter & other) const;
    bool operator!=(const HashConstIter & other) const;
    const valType & operator*() const;
    const valType *operator->() const;

  private:
    const Vector< List< valType > > * data_;
    size_t capacity_;
    size_t idx_;
    LCIter< valType > listIt_;
    LCIter< valType > listEnd_;

    HashConstIter(const Vector< List< valType > > * data, size_t capacity, size_t startIdx);

    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIter< Key, Value, Hash, Equal >;

    void findValid();
  };
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashIter< Key, Value, Hash, Equal >::HashIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_(nullptr),
  listEnd_(nullptr)
{}

template < class Key, class Value, class Hash, class Equal >
sedov::HashIter< Key, Value, Hash, Equal >::HashIter(Vector< List< valType > > * data,
    size_t capacity, size_t startIdx):
  data_(data),
  capacity_(capacity),
  idx_(startIdx),
  listIt_(nullptr),
  listEnd_(nullptr)
{
  findValid();
}

template < class Key, class Value, class Hash, class Equal >
void sedov::HashIter< Key, Value, Hash, Equal >::findValid()
{
  while (idx_ < capacity_)
  {
    listIt_ = (*data_)[idx_].begin();
    listEnd_ = (*data_)[idx_].end();
    if (listIt_ != listEnd_)
    {
      return;
    }
    ++idx_;
  }
  data_ = nullptr;
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashIter< Key, Value, Hash, Equal > & sedov::HashIter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  if (listIt_ != listEnd_)
  {
    return *this;
  }
  ++idx_;
  findValid();
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashIter< Key, Value, Hash, Equal > sedov::HashIter< Key, Value, Hash, Equal >::operator++(int)
{
  HashIter temp = *this;
  ++(*this);
  return temp;
}

template < class Key, class Value, class Hash, class Equal >
bool sedov::HashIter< Key, Value, Hash, Equal >::operator==(const HashIter & other) const
{
  if (data_ == nullptr && other.data_ == nullptr)
  {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr)
  {
    return false;
  }
  if (idx_ >= capacity_ && other.idx_ >= other.capacity_)
  {
    return true;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template < class Key, class Value, class Hash, class Equal >
bool sedov::HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter & other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashIter< Key, Value, Hash, Equal >::valType & sedov::HashIter< Key, Value, Hash, Equal >::operator*()
{
  return *listIt_;
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashIter< Key, Value, Hash, Equal >::valType * sedov::HashIter< Key, Value, Hash, Equal >::operator->()
{
  return &(*listIt_);
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_(nullptr),
  listEnd_(nullptr)
{}

template < class Key, class Value, class Hash, class Equal >
sedov::HashConstIter< Key, Value, Hash, Equal >::HashConstIter(const Vector< List< valType > > * data,
    size_t capacity, size_t startIdx):
  data_(data),
  capacity_(capacity),
  idx_(startIdx),
  listIt_(nullptr),
  listEnd_(nullptr)
{
  findValid();
}

template < class Key, class Value, class Hash, class Equal >
void sedov::HashConstIter< Key, Value, Hash, Equal >::findValid()
{
  while (idx_ < capacity_)
  {
    listIt_ = (*data_)[idx_].cbegin();
    listEnd_ = (*data_)[idx_].cend();
    if (listIt_ != listEnd_)
    {
      return;
    }
    ++idx_;
  }
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashConstIter< Key, Value, Hash, Equal > & sedov::HashConstIter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  if (listIt_ != listEnd_)
  {
    return *this;
  }
  ++idx_;
  findValid();
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashConstIter< Key, Value, Hash, Equal > sedov::HashConstIter< Key, Value, Hash, Equal >::operator++(int)
{
  HashConstIter temp = *this;
  ++(*this);
  return temp;
}

template < class Key, class Value, class Hash, class Equal >
bool sedov::HashConstIter< Key, Value, Hash, Equal >::operator==(const HashConstIter & other) const
{
  if (data_ == nullptr && other.data_ == nullptr)
  {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr)
  {
    return false;
  }
  if (idx_ >= capacity_ && other.idx_ >= other.capacity_)
  {
    return true;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template < class Key, class Value, class Hash, class Equal >
bool sedov::HashConstIter< Key, Value, Hash, Equal >::operator!=(const HashConstIter & other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
const typename sedov::HashConstIter< Key, Value, Hash, Equal >::valType &
  sedov::HashConstIter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

template < class Key, class Value, class Hash, class Equal >
const typename sedov::HashConstIter< Key, Value, Hash, Equal >::valType *
  sedov::HashConstIter< Key, Value, Hash, Equal >::operator->() const
{
  return &(*listIt_);
}

#endif
