#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>

#include "top_it_vector.hpp"

namespace ulanova
{
  template <class Key, class Value, class Hash, class Equal>
  class HashTable
  {
  public:
    class iterator;
    class const_iterator;

    explicit HashTable(size_t slots = 8);
    HashTable(size_t slots, const Hash& hash, const Equal& equal);
    HashTable(const HashTable& rhs);
    HashTable(HashTable&& rhs) noexcept;

    HashTable& operator=(const HashTable& rhs);
    HashTable& operator=(HashTable&& rhs) noexcept;

    void swap(HashTable& rhs) noexcept;

    bool isEmpty() const noexcept;
    size_t getsize() const noexcept;
    size_t getcapacity() const noexcept;

    void add(const Key& key, const Value& value);
    Value drop(const Key& key);
    bool has(const Key& key) const;

    Value* find(const Key& key);
    const Value* find (const Key& key) const;

    void rehash(size_t slots);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

  private:
    enum class BucketState
    {
      Empty,
      Occupied,
      Deleted
    };

    struct Bucket
    {
      BucketState state;
      Key key;
      Value value;

      Bucket():
        state(BucketState::Empty),
        key(),
        value()
      {}
    };
    Vector<Bucket> buckets_;
    size_t size_;
    Hash hash_;
    Equal equal_;

    static const size_t npos = static_cast<size_t>(-1);

    size_t findIndex(const Key& key) const;
    size_t findPlace(const Key& key, bool& found) const;
    size_t nextIndex(size_t index) const noexcept;
  };

  template <class Key, class Value, class Hash, class Equal>
  class HashTable<Key,Value,Hash,Equal>::iterator
  {
  public:
    iterator();
    iterator(HashTable* table, size_t index);

    iterator& operator++();
    iterator operator++(int);

    bool operator==(const iterator& rhs) const noexcept;
    bool operator!=(const iterator& rhs) const noexcept;

    const Key& key() const;
    Value& value() const;
  private:
    HashTable* table_;
    size_t index_;
    void skipEmpty();
  };

  template <class Key, class Value, class Hash, class Equal>
  class HashTable<Key, Value, Hash, Equal>::const_iterator
  {
  public:
    const_iterator();
    const_iterator(const HashTable* table, size_t index);

    const_iterator& operator++();
    const_iterator operator++(int);

    bool operator==(const const_iterator& rhs) const noexcept;
    bool operator!=(const const_iterator& rhs) const noexcept;

    const Key& key() const;
    const Value& value() const;

  private:
    const HashTable* table_;
    size_t index_;

    void skipEmpty();
  };
}
template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::HashTable(size_t slots):
  buckets_(slots, Bucket()),
  size_(0),
  hash_(),
  equal_()
{}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::HashTable(size_t slots, const Hash& hash, const Equal& equal):
  buckets_(slots, Bucket()),
  size_(0),
  hash_(hash),
  equal_(equal)
{}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable& rhs):
  buckets_(rhs.buckets_),
  size_(rhs.size_),
  hash_(rhs.hash_),
  equal_(rhs.equal_)
{}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::HashTable(HashTable&& rhs) noexcept:
  buckets_(std::move(rhs.buckets_)),
  size_(rhs.size_),
  hash_(std::move(rhs.hash_)),
  equal_(std::move(rhs.equal_))
{
  rhs.size_ = 0;
}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>&
ulanova::HashTable<Key, Value, Hash, Equal>::operator=(const HashTable& rhs)
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }

  HashTable cpy(rhs);
  swap(cpy);
  return *this;
}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>&
ulanova::HashTable<Key, Value, Hash, Equal>::operator=(HashTable&& rhs) noexcept
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }

  HashTable cpy(std::move(rhs));
  swap(cpy);
  return *this;
}


template <class Key, class Value, class Hash, class Equal>
void ulanova::HashTable<Key, Value, Hash, Equal>::swap(HashTable& rhs) noexcept
{
  buckets_.swap(rhs.buckets_);
  std::swap(size_, rhs.size_);
  std::swap(hash_, rhs.hash_);
  std::swap(equal_, rhs.equal_);
}

template <class Key, class Value, class Hash, class Equal>
bool ulanova::HashTable<Key, Value, Hash, Equal>::isEmpty() const noexcept
{
  return size_ == 0;
}

template <class Key, class Value, class Hash, class Equal>
size_t ulanova::HashTable<Key, Value, Hash, Equal>::getsize() const noexcept
{
  return size_;
}

template <class Key, class Value, class Hash, class Equal>
size_t ulanova::HashTable<Key, Value, Hash, Equal>::getcapacity() const noexcept
{
  return buckets_.getsize();
}

template <class Key, class Value, class Hash, class Equal>
size_t ulanova::HashTable<Key, Value,Hash,Equal>::nextIndex(size_t index) const noexcept
{
  return (index + 1) % buckets_.getsize();
}

template <class Key, class Value, class Hash, class Equal>
size_t ulanova::HashTable<Key,Value, Hash, Equal>::findIndex(const Key& key) const
{
  if (buckets_.isEmpty())
  {
    return npos;
  }
  size_t index = hash_(key) % buckets_.getsize();
  for (size_t i = 0; i < buckets_.getsize(); ++i)
  {
    const Bucket& bucket = buckets_[index];
    if (bucket.state == BucketState::Empty)
    {
      return npos;
    }
    if (bucket.state == BucketState::Occupied && equal_(bucket.key, key))
    {
      return index;
    }
    index = nextIndex(index);
  }
  return npos;

}
template <class Key, class Value, class Hash, class Equal>
size_t ulanova::HashTable<Key, Value, Hash, Equal>::findPlace(const Key& key, bool& found ) const
{
  found = false;
  if (buckets_.isEmpty())
  {
    return npos;
  }
  size_t index = hash_(key) % buckets_.getsize();
  size_t firstDeleted = npos;

  for (size_t i = 0; i < buckets_.getsize(); ++i)
  {
    const Bucket& bucket = buckets_[index];
    if (bucket.state == BucketState::Occupied && equal_(bucket.key, key))
    {
      found = true;
      return index;
    }
    if (bucket.state == BucketState::Deleted && firstDeleted == npos)
    {
      firstDeleted = index;
    }
    if (bucket.state == BucketState::Empty)
    {
      return firstDeleted == npos ? index : firstDeleted;
    }
    index = nextIndex(index);
  }
  return firstDeleted;
}

template <class Key, class Value, class Hash, class Equal>
void ulanova::HashTable<Key,Value,Hash,Equal>::add(const Key& key, const Value& value)
{
  bool found = false;
  size_t index = findPlace(key,found);

  if (index == npos)
  {
    throw std::overflow_error("hash table has no free slots");
  }

  Bucket& bucket = buckets_[index];
  if (found)
  {
    bucket.value = value;
    return;
  }
  bucket.key = key;
  bucket.value = value;
  bucket.state = BucketState::Occupied;
  ++size_;
}

template <class Key, class Value, class Hash, class Equal>
Value ulanova::HashTable<Key,Value,Hash,Equal>::drop(const Key& key)
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    throw std::out_of_range("key not found");
  }
  Bucket& bucket = buckets_[index];
  Value result = bucket.value;
  bucket.state = BucketState::Deleted;
  --size_;
  return result;
}

template <class Key, class Value, class Hash, class Equal>
Value* ulanova::HashTable<Key,Value,Hash,Equal>::find(const Key& key)
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    return nullptr;
  }
  return std::addressof(buckets_[index].value);
}

template <class Key, class Value, class Hash, class Equal>
bool ulanova::HashTable<Key,Value,Hash,Equal>::has(const Key& key) const
{
  return findIndex(key) != npos;
}

template <class Key, class Value, class Hash, class Equal>
const Value* ulanova::HashTable<Key,Value,Hash,Equal>::find(const Key& key) const
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    return nullptr;
  }
  return std::addressof(buckets_[index].value);
}

template <class Key, class Value, class Hash, class Equal>
void ulanova::HashTable<Key,Value,Hash,Equal>::rehash(size_t slots)
{
  if (slots < size_)
  {
    throw std::invalid_argument("new hash table capacity is too small");
  }
  HashTable tmp(slots,hash_,equal_);

  for(size_t i = 0; i < buckets_.getsize(); ++i)
  {
    if (buckets_[i].state == BucketState::Occupied)
    {
      tmp.add(buckets_[i].key,buckets_[i].value);
    }
  }
  swap(tmp);
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::iterator
ulanova::HashTable<Key, Value, Hash, Equal>::begin() noexcept
{
  return iterator(this,0);
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::iterator
ulanova::HashTable<Key, Value, Hash, Equal>::end() noexcept
{
  return iterator(this,buckets_.getsize());
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator
ulanova::HashTable<Key, Value, Hash, Equal>::begin() const noexcept
{
  return const_iterator(this, 0);
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator
ulanova::HashTable<Key, Value, Hash, Equal>::end() const noexcept
{
  return const_iterator(this, buckets_.getsize());
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator
ulanova::HashTable<Key, Value, Hash, Equal>::cbegin() const noexcept
{
  return begin();
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator
ulanova::HashTable<Key, Value, Hash, Equal>::cend() const noexcept
{
  return end();
}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::iterator::iterator():
  table_(nullptr),
  index_(0)
{}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::iterator::iterator(HashTable* table, size_t index):
  table_(table),
  index_(index)
{
  skipEmpty();
}

template<class Key, class Value, class Hash, class Equal>
void ulanova::HashTable<Key,Value,Hash,Equal>::iterator::skipEmpty()
{
  if (table_ == nullptr)
  {
    return;
  }

  while (index_ < table_-> buckets_.getsize() && table_->buckets_[index_].state != BucketState::Occupied)
  {
    ++index_;
  }
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::iterator&
ulanova::HashTable<Key, Value, Hash, Equal>::iterator::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::iterator
ulanova::HashTable<Key, Value, Hash, Equal>::iterator::operator++(int)
{
  iterator old(*this);
  ++(*this);
  return old;
}

template <class Key, class Value, class Hash, class Equal>
bool ulanova::HashTable<Key, Value, Hash, Equal>::iterator::operator==(const iterator& rhs) const noexcept
{
  return table_ == rhs.table_ && index_ == rhs.index_;
}

template <class Key, class Value, class Hash, class Equal>
bool ulanova::HashTable<Key, Value, Hash, Equal>::iterator::operator!=(const iterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template <class Key, class Value, class Hash, class Equal>
const Key& ulanova::HashTable<Key, Value, Hash, Equal>::iterator::key() const
{
  return table_->buckets_[index_].key;
}

template <class Key, class Value, class Hash, class Equal>
Value& ulanova::HashTable<Key, Value, Hash, Equal>::iterator::value() const
{
  return table_->buckets_[index_].value;
}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::const_iterator():
  table_(nullptr),
  index_(0)
{}

template <class Key, class Value, class Hash, class Equal>
ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::const_iterator(const HashTable* table, size_t index):
  table_(table),
  index_(index)
{
  skipEmpty();
}

template <class Key, class Value, class Hash, class Equal>
void ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::skipEmpty()
{
  if (table_ == nullptr)
  {
    return;
  }

  while (index_ < table_->buckets_.getsize() && table_->buckets_[index_].state != BucketState::Occupied)
  {
    ++index_;
  }
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator&
ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template <class Key, class Value, class Hash, class Equal>
typename ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator
ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::operator++(int)
{
  const_iterator old(*this);
  ++(*this);
  return old;
}

template <class Key, class Value, class Hash, class Equal>
bool ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::operator==(const const_iterator& rhs) const noexcept
{
   return table_ == rhs.table_ && index_ == rhs.index_;
}

template <class Key, class Value, class Hash, class Equal>
bool ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::operator!=(const const_iterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template <class Key, class Value, class Hash, class Equal>
const Key& ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::key() const
{
  return table_->buckets_[index_].key;
}

template <class Key, class Value, class Hash, class Equal>
const Value& ulanova::HashTable<Key, Value, Hash, Equal>::const_iterator::value() const
{
  return table_->buckets_[index_].value;
}

#endif
