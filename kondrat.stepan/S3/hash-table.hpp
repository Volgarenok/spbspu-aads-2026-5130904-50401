#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include "hash-node.hpp"
#include "htiter.hpp"
#include "htciter.hpp"

namespace kondrat
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  struct HashTable
  {
    using value_type = std::pair< Key, Value >;

    HashTable();
    HashTable(const HashTable & table);
    HashTable(HashTable && table) noexcept;
    explicit HashTable(size_t capacity);
    ~HashTable() noexcept;

    HashTable & operator=(const HashTable & table);
    HashTable & operator=(HashTable && table) noexcept;

    void add(const Key & key, const Value & value);
    void add(const Key & key, Value && value);
    void erase(const Key & key);
    bool contains(const Key & key) const;
    Value & at(const Key & key);
    const Value & at(const Key & key) const;
    void rehash(size_t newCapacity);
    void clear() noexcept;
    void swap(HashTable & table) noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;
    HTIter< Key, Value > begin() noexcept;
    HTIter< Key, Value > end() noexcept;
    HTCIter< Key, Value > begin() const noexcept;
    HTCIter< Key, Value > end() const noexcept;

  private:
    static const size_t DEFAULT_CAPACITY = 17;

    detail::HashNode< Key, Value > * data_;
    size_t size_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;

    size_t getIndex(const Key & key, size_t attempt) const;
    size_t findIndex(const Key & key) const;
    size_t findSlot(const Key & key) const;

    template< class V >
    void addImpl(const Key & key, V && value);
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    data_(new detail::HashNode< Key, Value >[DEFAULT_CAPACITY]),
    size_(0),
    capacity_(DEFAULT_CAPACITY),
    hash_(),
    equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & table):
    data_(new detail::HashNode< Key, Value >[table.capacity_]),
    size_(table.size_),
    capacity_(table.capacity_),
    hash_(table.hash_),
    equal_(table.equal_)
  {
    try
    {
      for (size_t i = 0; i < capacity_; ++i)
      {
        data_[i] = table.data_[i];
      }
    }
    catch (...)
    {
      delete[] data_;
      throw;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable && table) noexcept:
    data_(std::exchange(table.data_, nullptr)),
    size_(std::exchange(table.size_, 0)),
    capacity_(std::exchange(table.capacity_, 0)),
    hash_(std::move(table.hash_)),
    equal_(std::move(table.equal_))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t capacity):
    data_(nullptr),
    size_(0),
    capacity_(capacity),
    hash_(),
    equal_()
  {
    if (capacity_ == 0)
    {
      throw std::logic_error("invalid capacity");
    }
    data_ = new detail::HashNode< Key, Value >[capacity_];
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable() noexcept
  {
    delete[] data_;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal > &
  HashTable< Key, Value, Hash, Equal >::operator=(const HashTable & table)
  {
    if (this != std::addressof(table))
    {
      HashTable< Key, Value, Hash, Equal > copy(table);
      swap(copy);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal > &
  HashTable< Key, Value, Hash, Equal >::operator=(HashTable && table) noexcept
  {
    if (this != std::addressof(table))
    {
      HashTable< Key, Value, Hash, Equal > moved(std::move(table));
      swap(moved);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key & key, const Value & value)
  {
    addImpl(key, value);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key & key, Value && value)
  {
    addImpl(key, std::move(value));
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class V >
  void HashTable< Key, Value, Hash, Equal >::addImpl(const Key & key, V && value)
  {
    const size_t index = findSlot(key);
    data_[index].data.first = key;
    data_[index].data.second = std::forward< V >(value);
    data_[index].state = detail::OCCUPIED;
    ++size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::erase(const Key & key)
  {
    const size_t index = findIndex(key);
    if (index == capacity_)
    {
      throw std::logic_error("key not found");
    }
    data_[index].state = detail::TOMBSTONE;
    --size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::contains(const Key & key) const
  {
    return findIndex(key) != capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::at(const Key & key)
  {
    const size_t index = findIndex(key);
    if (index == capacity_)
    {
      throw std::logic_error("key not found");
    }
    return data_[index].data.second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value & HashTable< Key, Value, Hash, Equal >::at(const Key & key) const
  {
    const size_t index = findIndex(key);
    if (index == capacity_)
    {
      throw std::logic_error("key not found");
    }
    return data_[index].data.second;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t newCapacity)
  {
    if (newCapacity == 0 || newCapacity < size_)
    {
      throw std::logic_error("invalid capacity");
    }

    HashTable< Key, Value, Hash, Equal > newTable(newCapacity);
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[i].state == detail::OCCUPIED)
      {
        newTable.add(data_[i].data.first, data_[i].data.second);
      }
    }
    swap(newTable);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear() noexcept
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      data_[i].state = detail::EMPTY;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable & table) noexcept
  {
    std::swap(data_, table.data_);
    std::swap(size_, table.size_);
    std::swap(capacity_, table.capacity_);
    std::swap(hash_, table.hash_);
    std::swap(equal_, table.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value > HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    return HTIter< Key, Value >(data_, data_ + capacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value > HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return HTIter< Key, Value >(data_ + capacity_, data_ + capacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value > HashTable< Key, Value, Hash, Equal >::begin() const noexcept
  {
    return HTCIter< Key, Value >(data_, data_ + capacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value > HashTable< Key, Value, Hash, Equal >::end() const noexcept
  {
    return HTCIter< Key, Value >(data_ + capacity_, data_ + capacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::getIndex(const Key & key, size_t attempt) const
  {
    return (hash_(key) + attempt * attempt) % capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndex(const Key & key) const
  {
    for (size_t attempt = 0; attempt < capacity_; ++attempt)
    {
      const size_t index = getIndex(key, attempt);
      if (data_[index].state == detail::EMPTY)
      {
        return capacity_;
      }
      if (data_[index].state == detail::OCCUPIED
          && equal_(data_[index].data.first, key))
      {
        return index;
      }
    }
    return capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findSlot(const Key & key) const
  {
    size_t firstTombstone = capacity_;
    for (size_t attempt = 0; attempt < capacity_; ++attempt)
    {
      const size_t index = getIndex(key, attempt);
      if (data_[index].state == detail::TOMBSTONE)
      {
        if (firstTombstone == capacity_)
        {
          firstTombstone = index;
        }
      }
      else if (data_[index].state == detail::EMPTY)
      {
        return firstTombstone == capacity_ ? index : firstTombstone;
      }
    }
    if (firstTombstone != capacity_)
    {
      return firstTombstone;
    }
    throw std::overflow_error("hash table is full");
  }
}

#endif
