#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <utility>
#include "HashNode.hpp"
#include "HIter.hpp"
#include "CHIter.hpp"

namespace petrov
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
    HashTable();
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable(size_t cap);
    ~HashTable();
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    void add(const Key& k, const Value& v);
    void add(const Key& k, Value&& v);
    bool drop(const Key& k);
    bool has(const Key& k) const;
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    Value& operator[](const Key& k);
    void swap(HashTable& other) noexcept;

    void rehash(size_t slots);
    void clear();

    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;

    HIter< Key, Value > begin();
    HIter< Key, Value > end();
    CHIter< Key, Value > begin() const;
    CHIter< Key, Value > end() const;

    private:
      struct Finder
      {
        size_t idx;
        bool found;
      };
      HashNode< Key, Value >* data_;
      size_t size_;
      size_t capacity_;
      Hash hash_;
      Equal equal_;

      size_t home(const Key& k) const;
      size_t probe(size_t home, size_t i) const;
      Finder find(const Key& k) const;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    data_(new HashNode< Key, Value > [8]),
    size_(0),
    capacity_(8),
    hash_(),
    equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t cap):
    data_(new HashNode< Key, Value > [cap]),
    size_(0),
    capacity_(cap),
    hash_(),
    equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    delete[] data_;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
    data_(new HashNode< Key, Value >[other.capacity_]),
    size_(other.size_),
    capacity_(other.capacity_),
    hash_(other.hash_),
    equal_(other.equal_)
  {
    try
    {
      for (size_t i = 0; i < capacity_; ++i)
      {
        data_[i] = other.data_[i];
      }
    }
    catch (...)
    {
      delete[] data_;
      throw;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
    data_(std::exchange(other.data_, nullptr)),
    size_(std::exchange(other.size_, 0)),
    capacity_(std::exchange(other.capacity_, 0)),
    hash_(std::move(other.hash_)),
    equal_(std::move(other.equal_))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    HashTable< Key, Value, Hash, Equal> cpy = other;
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal>::operator=(HashTable&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    HashTable< Key, Value, Hash, Equal> cpy(std::move(other));
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
  {
    if (size_ == capacity_)
    {
      throw std::runtime_error("No slot for add");
    }
    Finder f = find(k);
    if (f.found)
    {
      data_[f.idx].value = v;
      return;
    }
    data_[f.idx].key = k;
    data_[f.idx].value = v;
    data_[f.idx].state = OCCUPIED;
    ++size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& k, Value&& v)
  {
    if (size_ == capacity_)
    {
      throw std::runtime_error("No slot for add");
    }
    Finder f = find(k);
    if (f.found)
    {
      data_[f.idx].value = std::move(v);
      return;
    }
    data_[f.idx].key = k;
    data_[f.idx].value = std::move(v);
    data_[f.idx].state = OCCUPIED;
    ++size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::drop(const Key& k)
  {
    Finder f = find(k);
    if (!f.found)
    {
      return false;
    }
    data_[f.idx].state = TOMBSTONE;
    --size_;
    return true;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& k) const
  {
    return find(k).found;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
  {
    Finder f = find(k);
    if (!f.found)
    {
      throw std::out_of_range("Key not found");
    }
    return data_[f.idx].value;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k)
  {
    return const_cast< Value& >(static_cast< const HashTable* >(this)->at(k));
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
  {
    Finder f = find(k);
    if (f.found)
    {
      return data_[f.idx].value;
    }

    if (size_ == capacity_)
    {
      throw std::runtime_error("No slot for add");
    }

    data_[f.idx].key = k;
    data_[f.idx].value = Value{};
    data_[f.idx].state = OCCUPIED;
    ++size_;

    return data_[f.idx].value;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(hash_, other.hash_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    if (slots <= capacity_ || slots == 0)
    {
      return;
    }

    HashTable newTable(slots);
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (data_[i].state == OCCUPIED)
      {
        newTable.add(data_[i].key, data_[i].value);
      }
    }
    swap(newTable);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      data_[i].state = EMPTY;
    }
    size_ = 0;
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
   HIter< Key, Value > HashTable< Key, Value, Hash, Equal >::begin()
   {
     return HIter< Key, Value >(data_, data_ + capacity_);
   }

   template< class Key, class Value, class Hash, class Equal >
   HIter< Key, Value > HashTable< Key, Value, Hash, Equal >::end()
   {
     return HIter< Key, Value >(data_ + capacity_, data_ + capacity_);
   }

   template< class Key, class Value, class Hash, class Equal >
   CHIter< Key, Value > HashTable< Key, Value, Hash, Equal >::begin() const
   {
     return CHIter< Key, Value >(data_, data_ + capacity_);
   }

   template< class Key, class Value, class Hash, class Equal >
   CHIter< Key, Value > HashTable< Key, Value, Hash, Equal >::end() const
   {
     return CHIter< Key, Value >(data_ + capacity_, data_ + capacity_);
   }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::home(const Key& k) const
  {
    return hash_(k) % capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::probe(size_t home, size_t i) const
  {
    if (i != 0 && (SIZE_MAX / i > i))
    {
      return home + (i * i) % capacity_;
    }
    return home;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Finder HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
  {
    size_t h = home(k);
    size_t first_tmbs = capacity_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t idx = probe(h, i);
      if (data_[idx].state == OCCUPIED && (equal_(data_[idx].key, k)))
      {
        return {idx, true};
      }
      else if (data_[idx].state == TOMBSTONE && (first_tmbs == capacity_))
      {
        first_tmbs = idx;
      }
      else if (data_[idx].state == EMPTY)
      {
        return {first_tmbs == capacity_ ? idx : first_tmbs, false};
      }
    }
    return {first_tmbs == capacity_ ? h : first_tmbs, false};
  }

}
#endif
