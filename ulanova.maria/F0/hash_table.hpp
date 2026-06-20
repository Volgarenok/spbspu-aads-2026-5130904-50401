#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "../common/vector.hpp"

#include <cstddef>
#include  <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <memory>

namespace ulanova
{
  template < class Value >
  class HashTable
  {
  public:

    class iterator;
    class const_iterator;

    HashTable();
    HashTable(size_t capacity);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    ~HashTable() = default;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;
    void swap(HashTable& other) noexcept;
    Value& at(const std::string& key);
    const Value& at(const std::string& key) const;
    size_t capacity() const noexcept;

    void add(const std::string& key, const Value& value);
    void drop(const std::string& key);
    void clear() noexcept;
    bool has(const std::string& key) const;
    size_t size() const noexcept;
    bool empty() const noexcept;
    Value* find(const std::string& key);
    const Value* find(const std::string& key) const;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

  private:

    enum class State
    {
      empty,
      filled,
      deleted
    };

    struct Bucket
    {
      State state;
      std::string key;
      Value value;
    };

    ulanova::Vector< Bucket > buckets_;
    size_t size_;

    size_t get_first_hash(const std::string& key) const;
    size_t get_second_hash(const std::string& key) const;
    size_t find_index(const std::string& key) const;
    bool need_rehash() const;
    void rehash(size_t new_capacity);
    void add_without_rehash(const std::string& key, const Value& value);
    static bool is_prime(size_t n)
    {
      if (n < 2) return false;
      for (size_t i = 2; i * i <= n; ++i)
      {
        if (n % i == 0) return false;
      }
      return true;
    }

    static size_t next_prime(size_t n)
    {
      while (!is_prime(n))
      {
        ++n;
      }
      return n;
    }
  };
}

template< class Value >
class ulanova::HashTable< Value >::iterator
{
public:
  iterator(HashTable< Value >* table, size_t index) noexcept:
    table_(table),
    index_(index)
  {
    skip_empty();
  }

  Value& operator*() const
  {
    return table_->buckets_[index_].value;
  }

  Value* operator->() const
  {
    return std::addressof(table_->buckets_[index_].value);
  }

  iterator& operator++() noexcept
  {
    ++index_;
    skip_empty();
    return *this;
  }

  bool operator!=(const iterator& rhs) const noexcept
  {
    return (table_ != rhs.table_) || (index_ != rhs.index_);
  }

  bool operator==(const iterator& rhs) const noexcept
  {
    return !(*this != rhs);
  }

private:
  HashTable< Value >* table_;
  size_t index_;

  void skip_empty() noexcept
  {
    while ((index_ < table_->buckets_.getsize()) && (table_->buckets_[index_].state != State::filled))
    {
      ++index_;
    }
  }
};

template< class Value >
class ulanova::HashTable< Value >::const_iterator
{
public:
  const_iterator(const HashTable< Value >* table, size_t index) noexcept:
    table_(table),
    index_(index)
  {
    skip_empty();
  }

  const Value& operator*() const
  {
    return table_->buckets_[index_].value;
  }

  const Value* operator->() const
  {
    return std::addressof(table_->buckets_[index_].value);
  }

  const_iterator& operator++() noexcept
  {
    ++index_;
    skip_empty();
    return *this;
  }

  bool operator!=(const const_iterator& rhs) const noexcept
  {
    return (table_ != rhs.table_) || ( index_ != rhs.index_);
  }

  bool operator==(const const_iterator& rhs) const noexcept
  {
    return !(*this != rhs);
  }

private:
  const HashTable< Value >* table_;
  size_t index_;

  void skip_empty() noexcept
  {
    while ((index_ < table_->buckets_.getsize()) && (table_->buckets_[index_].state != State::filled))
    {
      ++index_;
    }
  }
};


template< class Value >
ulanova::HashTable< Value >::HashTable():
  buckets_(17, Bucket{State::empty, "", Value{}}),
  size_(0)
{}

template< class Value >
ulanova::HashTable< Value >::HashTable(size_t capacity):
  buckets_(next_prime(capacity), Bucket{State::empty, "", Value{}}),
  size_(0)
{}

template< class Value >
ulanova::HashTable< Value >::HashTable(const HashTable& other):
  buckets_(other.buckets_),
  size_(other.size_)
{}

template< class Value >
ulanova::HashTable< Value >::HashTable(HashTable&& other) noexcept:
  buckets_(std::move(other.buckets_)),
  size_(other.size_)
{
  other.size_ = 0;
}

template< class Value >
ulanova::HashTable< Value >& ulanova::HashTable< Value >::operator=(const HashTable& other)
{
  if (this != std::addressof(other))
  {
    HashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template< class Value >
ulanova::HashTable< Value >& ulanova::HashTable< Value >::operator=(HashTable&& other) noexcept
{
  if (this != std::addressof(other))
  {
    swap(other);
  }
  return *this;
}

template< class Value >
void ulanova::HashTable< Value >::swap(HashTable& other) noexcept
{
  std::swap(buckets_, other.buckets_);
  std::swap(size_, other.size_);
}

template< class Value >
Value& ulanova::HashTable< Value >::at(const std::string& key)
{
  const size_t index = find_index(key);
  if (index == buckets_.getsize())
  {
    throw std::logic_error("key not found");
  }
  return buckets_[index].value;
}

template< class Value >
const Value& ulanova::HashTable< Value >::at(const std::string& key) const
{
  const size_t index = find_index(key);
  if (index == buckets_.getsize())
  {
    throw std::logic_error("key not found");
  }
  return buckets_[index].value;
}

template< class Value >
size_t ulanova::HashTable< Value >::capacity() const noexcept
{
  return buckets_.getsize();
}

template< class Value >
void ulanova::HashTable< Value >::add(const std::string& key,
  const Value& value)
{
  if (has(key))
  {
    throw std::logic_error("key already exists");
  }

  if (need_rehash())
  {
    rehash(next_prime(buckets_.getsize() * 2 + 1));
  }

  add_without_rehash(key, value);
}

template< class Value >
void ulanova::HashTable< Value >::drop(const std::string& key)
{
  const size_t index = find_index(key);

  if (index == buckets_.getsize())
  {
    throw std::logic_error("key not found");
  }

  buckets_[index].state = State::deleted;
  --size_;
}

template< class Value >
void ulanova::HashTable< Value >::clear() noexcept
{
  for (size_t i = 0; i < buckets_.getsize(); ++i)
  {
    buckets_[i].state = State::empty;
  }
  size_ = 0;
}

template<class Value >
bool ulanova::HashTable< Value >::has(const std::string& key) const
{
  return find_index(key) != buckets_.getsize();
}

template< class Value >
size_t ulanova::HashTable< Value >::size() const noexcept
{
  return size_;
}

template< class Value >
bool ulanova::HashTable< Value >::empty() const noexcept
{
  return size() == 0;
}

template< class Value >
Value* ulanova::HashTable< Value >::find(const std::string& key)
{
  const size_t index = find_index(key);

  if (index == buckets_.getsize())
  {
    return nullptr;
  }

  return std::addressof(buckets_[index].value);
}

template< class Value >
const Value* ulanova::HashTable< Value >::find(const std::string& key) const
{
  const size_t index = find_index(key);

  if (index == buckets_.getsize())
  {
    return nullptr;
  }

  return std::addressof(buckets_[index].value);
}

template< class Value >
size_t ulanova::HashTable< Value >::get_first_hash(const std::string& key) const
{
  return std::hash< std::string >{}(key) % buckets_.getsize();
}

template< class Value >
size_t ulanova::HashTable< Value >::get_second_hash( const std::string& key ) const
{
  const size_t hash = std::hash< std::string >{}(key);
  return 1 + (hash % (buckets_.getsize() - 1));
}

template< class Value >
size_t ulanova::HashTable< Value >::find_index( const std::string& key) const
{
  const size_t first_hash = get_first_hash(key);
  const size_t second_hash = get_second_hash(key);

  for (size_t i = 0; i < buckets_.getsize(); ++i)
  {
    const size_t index = (first_hash + i * second_hash) % buckets_.getsize();

    if (buckets_[index].state == State::empty)
    {
      return buckets_.getsize();
    }

    if ((buckets_[index].state == State::filled) && (buckets_[index].key == key))
    {
      return index;
    }
  }
  return buckets_.getsize();
}

template< class Value >
bool ulanova::HashTable< Value >::need_rehash() const
{
  return size_ * 10 >= buckets_.getsize() * 7;
}

template< class Value >
void ulanova::HashTable< Value >::rehash(size_t new_capacity)
{
  const ulanova::Vector< Bucket > old_buckets = buckets_;

  buckets_ = ulanova::Vector< Bucket >(new_capacity, Bucket{State::empty, "", Value{}});
  size_ = 0;

  for (size_t i = 0; i < old_buckets.getsize(); ++i)
  {
    if (old_buckets[i].state == State::filled)
    {
      add_without_rehash(old_buckets[i].key, old_buckets[i].value);
    }
  }
}

template< class Value >
void ulanova::HashTable< Value >::add_without_rehash(
  const std::string& key,
  const Value& value)
{
  const size_t first_hash = get_first_hash(key);
  const size_t second_hash = get_second_hash(key);
  size_t deleted_index = buckets_.getsize();

  for (size_t i = 0; i < buckets_.getsize(); ++i)
  {
    const size_t index = (first_hash + i * second_hash) % buckets_.getsize();

    if (buckets_[index].state == State::deleted)
    {
      if (deleted_index == buckets_.getsize())
      {
        deleted_index = index;
      }
    }
    else if (buckets_[index].state == State::empty)
    {
      const size_t target = (deleted_index == buckets_.getsize()) ? index : deleted_index;

      buckets_[target].state = State::filled;
      buckets_[target].key = key;
      buckets_[target].value = value;
      ++size_;
      return;
    }
  }

  if (deleted_index != buckets_.getsize())
  {
    buckets_[deleted_index].state = State::filled;
    buckets_[deleted_index].key = key;
    buckets_[deleted_index].value = value;
    ++size_;
    return;
  }

  throw std::logic_error("hash table is full");
}

template< class Value >
typename ulanova::HashTable< Value >::iterator
ulanova::HashTable< Value >::begin() noexcept
{
  return iterator(this,0);
}

template< class Value >
typename ulanova::HashTable< Value >::iterator
ulanova::HashTable< Value >::end() noexcept
{
  return iterator(this, buckets_.getsize());
}

template< class Value >
typename ulanova::HashTable< Value >::const_iterator
ulanova::HashTable< Value >::begin() const noexcept
{
  return const_iterator(this, 0);
}

template< class Value >
typename ulanova::HashTable< Value >::const_iterator
ulanova::HashTable< Value >::end() const noexcept
{
  return const_iterator(this, buckets_.getsize());
}

template< class Value >
typename ulanova::HashTable< Value >::const_iterator
ulanova::HashTable< Value >::cbegin() const noexcept
{
  return begin();
}

template< class Value >
typename ulanova::HashTable< Value >::const_iterator
ulanova::HashTable< Value >::cend() const noexcept
{
  return end();
}


#endif
