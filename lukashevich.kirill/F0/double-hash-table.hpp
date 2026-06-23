#ifndef DOUBLE_HASH_TABLE_HPP
#define DOUBLE_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "hash-table-utils.hpp"
#include "vector/vector.hpp"

namespace lukashevich {
  template< class Key, class Value, class FirstHash, class SecondHash >
  class HashTable
  {
  public:
    class Iterator;
    class ConstIterator;

    HashTable();

    void insert(const Key & key, const Value & value);
    void erase(const Key & key);
    bool contains(const Key & key) const;

    Value & at(const Key & key);
    const Value & at(const Key & key) const;

    size_t getSize() const noexcept;
    bool isEmpty() const noexcept;

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

  private:
    struct Cell
    {
      Key key;
      Value value;
      bool isBusy;
      bool isDeleted;

      Cell();
    };

    size_t getIndex(const Key & key, size_t probe) const;
    size_t findIndex(const Key & key) const;
    size_t findPlace(const Key & key) const;
    void rehash(size_t capacity);
    void growIfNeeded();

    Vector< Cell > data_;
    size_t size_;
    size_t used_;
    FirstHash firstHash_;
    SecondHash secondHash_;
  };

  template< class Key, class Value, class FirstHash, class SecondHash >
  class HashTable< Key, Value, FirstHash, SecondHash >::Iterator
  {
  public:
    Iterator();

    Iterator & operator++() noexcept;
    Iterator operator++(int) noexcept;
    bool operator==(const Iterator & rhs) const noexcept;
    bool operator!=(const Iterator & rhs) const noexcept;

    Value & operator*() const noexcept;
    Value * operator->() const noexcept;
    const Key & getKey() const noexcept;
    Value & getValue() const noexcept;

  private:
    Iterator(Cell * data, size_t capacity, size_t index) noexcept;
    void skipEmpty() noexcept;

    Cell * data_;
    size_t capacity_;
    size_t index_;

    friend class HashTable< Key, Value, FirstHash, SecondHash >;
  };

  template< class Key, class Value, class FirstHash, class SecondHash >
  class HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator
  {
  public:
    ConstIterator();
    ConstIterator(const Iterator & rhs) noexcept;

    ConstIterator & operator++() noexcept;
    ConstIterator operator++(int) noexcept;
    bool operator==(const ConstIterator & rhs) const noexcept;
    bool operator!=(const ConstIterator & rhs) const noexcept;

    const Value & operator*() const noexcept;
    const Value * operator->() const noexcept;
    const Key & getKey() const noexcept;
    const Value & getValue() const noexcept;

  private:
    ConstIterator(const Cell * data, size_t capacity, size_t index) noexcept;
    void skipEmpty() noexcept;

    const Cell * data_;
    size_t capacity_;
    size_t index_;

    friend class HashTable< Key, Value, FirstHash, SecondHash >;
  };

  template< class Key, class Value, class FirstHash, class SecondHash >
  HashTable< Key, Value, FirstHash, SecondHash >::Cell::Cell():
    key(),
    value(),
    isBusy(false),
    isDeleted(false)
  {}

  template< class Key, class Value, class FirstHash, class SecondHash >
  HashTable< Key, Value, FirstHash, SecondHash >::HashTable():
    data_(detail::getNextPrimeCapacity(0)),
    size_(0),
    used_(0),
    firstHash_(),
    secondHash_()
  {}

  template< class Key, class Value, class FirstHash, class SecondHash >
  void HashTable< Key, Value, FirstHash, SecondHash >::insert(const Key & key,
      const Value & value)
  {
    growIfNeeded();

    size_t found = findIndex(key);
    if (found != data_.getSize()) {
      data_[found].value = value;
      return;
    }

    size_t index = findPlace(key);
    if (!data_[index].isBusy && !data_[index].isDeleted) {
      ++used_;
    }

    data_[index].key = key;
    data_[index].value = value;
    data_[index].isBusy = true;
    data_[index].isDeleted = false;
    ++size_;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  void HashTable< Key, Value, FirstHash, SecondHash >::erase(const Key & key)
  {
    size_t index = findIndex(key);
    if (index == data_.getSize()) {
      throw std::out_of_range("hash table key not found");
    }

    data_[index].isBusy = false;
    data_[index].isDeleted = true;
    --size_;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  bool HashTable< Key, Value, FirstHash, SecondHash >::contains(const Key & key) const
  {
    return findIndex(key) != data_.getSize();
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  Value & HashTable< Key, Value, FirstHash, SecondHash >::at(const Key & key)
  {
    size_t index = findIndex(key);
    if (index == data_.getSize()) {
      throw std::out_of_range("hash table key not found");
    }

    return data_[index].value;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  const Value & HashTable< Key, Value, FirstHash, SecondHash >::at(const Key & key) const
  {
    size_t index = findIndex(key);
    if (index == data_.getSize()) {
      throw std::out_of_range("hash table key not found");
    }

    return data_[index].value;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  size_t HashTable< Key, Value, FirstHash, SecondHash >::getSize() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  bool HashTable< Key, Value, FirstHash, SecondHash >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::Iterator
      HashTable< Key, Value, FirstHash, SecondHash >::begin() noexcept
  {
    return Iterator(data_.begin(), data_.getSize(), 0);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::Iterator
      HashTable< Key, Value, FirstHash, SecondHash >::end() noexcept
  {
    return Iterator(data_.begin(), data_.getSize(), data_.getSize());
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator
      HashTable< Key, Value, FirstHash, SecondHash >::begin() const noexcept
  {
    return ConstIterator(data_.begin(), data_.getSize(), 0);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator
      HashTable< Key, Value, FirstHash, SecondHash >::end() const noexcept
  {
    return ConstIterator(data_.begin(), data_.getSize(), data_.getSize());
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator
      HashTable< Key, Value, FirstHash, SecondHash >::cbegin() const noexcept
  {
    return begin();
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator
      HashTable< Key, Value, FirstHash, SecondHash >::cend() const noexcept
  {
    return end();
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  size_t HashTable< Key, Value, FirstHash, SecondHash >::getIndex(const Key & key,
      size_t probe) const
  {
    const size_t capacity = data_.getSize();
    const size_t first = firstHash_(key) % capacity;
    const size_t step = (secondHash_(key) % (capacity - 1)) + 1;
    return (first + (probe * step)) % capacity;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  size_t HashTable< Key, Value, FirstHash, SecondHash >::findIndex(const Key & key) const
  {
    const size_t capacity = data_.getSize();

    for (size_t probe = 0; probe < capacity; ++probe) {
      const size_t index = getIndex(key, probe);
      if (!data_[index].isBusy && !data_[index].isDeleted) {
        return capacity;
      }
      if (data_[index].isBusy && (data_[index].key == key)) {
        return index;
      }
    }

    return capacity;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  size_t HashTable< Key, Value, FirstHash, SecondHash >::findPlace(const Key & key) const
  {
    const size_t capacity = data_.getSize();
    size_t deletedIndex = capacity;

    for (size_t probe = 0; probe < capacity; ++probe) {
      const size_t index = getIndex(key, probe);
      if (!data_[index].isBusy && data_[index].isDeleted) {
        if (deletedIndex == capacity) {
          deletedIndex = index;
        }
      } else if (!data_[index].isBusy && !data_[index].isDeleted) {
        if (deletedIndex != capacity) {
          return deletedIndex;
        }
        return index;
      }
    }

    if (deletedIndex != capacity) {
      return deletedIndex;
    }

    throw std::length_error("hash table is full");
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  void HashTable< Key, Value, FirstHash, SecondHash >::rehash(size_t capacity)
  {
    HashTable temp;
    temp.data_ = Vector< Cell >(capacity);
    temp.size_ = 0;
    temp.used_ = 0;

    for (size_t i = 0; i < data_.getSize(); ++i) {
      if (data_[i].isBusy) {
        temp.insert(data_[i].key, data_[i].value);
      }
    }

    data_.swap(temp.data_);
    size_ = temp.size_;
    used_ = temp.used_;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  void HashTable< Key, Value, FirstHash, SecondHash >::growIfNeeded()
  {
    const size_t capacity = data_.getSize();
    if (((used_ + 1) * 2) >= capacity) {
      rehash(detail::getNextPrimeCapacity(capacity));
    }
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  HashTable< Key, Value, FirstHash, SecondHash >::Iterator::Iterator():
    data_(nullptr),
    capacity_(0),
    index_(0)
  {}

  template< class Key, class Value, class FirstHash, class SecondHash >
  HashTable< Key, Value, FirstHash, SecondHash >::Iterator::Iterator(Cell * data,
      size_t capacity,
      size_t index) noexcept:
    data_(data),
    capacity_(capacity),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::Iterator &
      HashTable< Key, Value, FirstHash, SecondHash >::Iterator::operator++() noexcept
  {
    if (index_ < capacity_) {
      ++index_;
      skipEmpty();
    }
    return *this;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::Iterator
      HashTable< Key, Value, FirstHash, SecondHash >::Iterator::operator++(int) noexcept
  {
    Iterator temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  bool HashTable< Key, Value, FirstHash, SecondHash >::Iterator::operator==(
      const Iterator & rhs) const noexcept
  {
    return (data_ == rhs.data_) && (index_ == rhs.index_);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  bool HashTable< Key, Value, FirstHash, SecondHash >::Iterator::operator!=(
      const Iterator & rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  Value & HashTable< Key, Value, FirstHash, SecondHash >::Iterator::operator*()
      const noexcept
  {
    return data_[index_].value;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  Value * HashTable< Key, Value, FirstHash, SecondHash >::Iterator::operator->()
      const noexcept
  {
    return &(data_[index_].value);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  const Key & HashTable< Key, Value, FirstHash, SecondHash >::Iterator::getKey()
      const noexcept
  {
    return data_[index_].key;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  Value & HashTable< Key, Value, FirstHash, SecondHash >::Iterator::getValue()
      const noexcept
  {
    return data_[index_].value;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  void HashTable< Key, Value, FirstHash, SecondHash >::Iterator::skipEmpty() noexcept
  {
    while ((index_ < capacity_) && !data_[index_].isBusy) {
      ++index_;
    }
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::ConstIterator():
    data_(nullptr),
    capacity_(0),
    index_(0)
  {}

  template< class Key, class Value, class FirstHash, class SecondHash >
  HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::ConstIterator(
      const Iterator & rhs) noexcept:
    data_(rhs.data_),
    capacity_(rhs.capacity_),
    index_(rhs.index_)
  {}

  template< class Key, class Value, class FirstHash, class SecondHash >
  HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::ConstIterator(
      const Cell * data,
      size_t capacity,
      size_t index) noexcept:
    data_(data),
    capacity_(capacity),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator &
      HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::operator++()
      noexcept
  {
    if (index_ < capacity_) {
      ++index_;
      skipEmpty();
    }
    return *this;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  typename HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator
      HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::operator++(int)
      noexcept
  {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  bool HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::operator==(
      const ConstIterator & rhs) const noexcept
  {
    return (data_ == rhs.data_) && (index_ == rhs.index_);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  bool HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::operator!=(
      const ConstIterator & rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  const Value & HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::operator*()
      const noexcept
  {
    return data_[index_].value;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  const Value * HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::operator->()
      const noexcept
  {
    return &(data_[index_].value);
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  const Key & HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::getKey()
      const noexcept
  {
    return data_[index_].key;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  const Value & HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::getValue()
      const noexcept
  {
    return data_[index_].value;
  }

  template< class Key, class Value, class FirstHash, class SecondHash >
  void HashTable< Key, Value, FirstHash, SecondHash >::ConstIterator::skipEmpty() noexcept
  {
    while ((index_ < capacity_) && !data_[index_].isBusy) {
      ++index_;
    }
  }
}

#endif
