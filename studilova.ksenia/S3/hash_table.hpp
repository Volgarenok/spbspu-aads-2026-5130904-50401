#ifndef HASH_TABLE
#define HASH_TABLE

#include "../common/vector.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace studilova
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    enum class State
    {
      EMPTY,
      OCCUPIED,
      TOMBSTONE
    };

    struct Entry
    {
      Key key;
      Value value;
      State state;

      Entry();
    };

    public:
      explicit HashTable
      (
        size_t capacity = 16,
        Hash hash = Hash{},
        Equal equal = Equal{}
      );

      size_t size() const noexcept;
      size_t capacity() const noexcept;
      bool isEmpty() const noexcept;

    private:
      topit::Vector< Entry > table_;
      size_t size_;
      Hash hash_;
      Equal equal_;

      static constexpr size_t npos = static_cast< size_t >(-1);

      size_t probeIndex(const Key& key, size_t attempt) const;
      size_t findIndex(const Key& key, size_t& outIndex) const;
  };
}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::Entry::Entry() :
  key(),
  value(),
  state(State::EMPTY)
{}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::HashTable(
  size_t capacity,
  Hash hash,
  Equal equal
):
  table_(capacity),
  size_(0),
  hash_(std::move(hash)),
  equal_(std::move(equal))
{
  if (capacity == 0)
  {
    throw std::invalid_argument("HashTable capacity must be positive");
  }
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::probeIndex(const Key& key, size_t attempt) const
{
  return(hash_(key) + attempt * attempt) % table_.getSize();
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::findIndex(const Key& key, size_t& outIndex) const
{
  for (size_t attempt = 0; attempt < table_.getSize(); ++attempt)
  {
    size_t index = probeIndex(key, attempt);
    if (table_[index].state == State::EMPTY)
    {
      return false;
    }
    if (table_[index].state == State::OCCUPIED && equal_(table_[index].key, key))
    {
      outIndex = index;
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return table_.getSize();
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::isEmpty() const noexcept
{
  return size_ == 0;
}

#endif
