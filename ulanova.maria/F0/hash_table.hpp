#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "../common/vector.hpp"

#include <cstddef>
#include  <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace ulanova
{
  template < class Value >
  class HashTable
  {
  public:

    HashTable();

    void add(const std::string& key, const Value& value);
    void drop(const std::string& key);
    bool has(const std::string& key) const;
    Value* find(const std::string& key);
    const Value* find(const std::string& key) const;

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
  };
}

template< class Value >
ulanova::HashTable< Value >::HashTable():
  buckets_(17, Bucket{State::empty, "", Value{}}),
  size_(0)
{}

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
    rehash(buckets_.getsize() * 2);
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

template<class Value >
bool ulanova::HashTable< Value >::has(const std::string& key) const
{
  return find_index(key) != buckets_.getsize();
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

  for (size_t i = 0; i < buckets_.size(); ++i)
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
  return (size_ + 1) * 2 >= buckets_.getsize();
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

  throw std::logic_error("hash table is full");
}

#endif
