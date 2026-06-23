#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector/top-it-vector.hpp>
#include "cuckoo-hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class PrimHash, class SecHash, class Equal = std::equal_to< Key > >
  class CuckooHashTable;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashIter;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashConstIter;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class CuckooHashTable
  {
    public:
      using value_type = std::pair< Key, Value >;
      using Iterator = HashIter< Key, Value, PrimHash, SecHash, Equal >;
      using ConstIterator = HashConstIter< Key, Value, PrimHash, SecHash, Equal >;

      CuckooHashTable();
      CuckooHashTable(const CuckooHashTable & other) = default;
      explicit CuckooHashTable(size_t capacity);

      CuckooHashTable & operator=(const CuckooHashTable & other);

      void insert(const Key & key, const Value & value);
      void insert(const Key & key, Value && value);
      size_t erase(const Key & key);

      bool contains(const Key & key) const;
      Value & at(const Key & key);
      const Value & at(const Key & key) const;
      Value & operator[](const Key & key);

      void clear();
      void rehash(size_t newCapacity);
      void swap(CuckooHashTable & other) noexcept;

      size_t size() const noexcept;
      size_t capacity() const noexcept;
      bool empty() const noexcept;

      Iterator begin() noexcept;
      Iterator end() noexcept;
      ConstIterator begin() const noexcept;
      ConstIterator end() const noexcept;
      ConstIterator cbegin() const noexcept;
      ConstIterator cend() const noexcept;

    private:
      friend class HashIter< Key, Value, PrimHash, SecHash, Equal >;
      friend class HashConstIter< Key, Value, PrimHash, SecHash, Equal >;

      using Node = detail::CuckooHashNode< Key, Value >;

      static const size_t minSubtableCapacity_ = 8;
      static const size_t maxLoadFactorMultiplier_ = 2;
      static const size_t maxRehashCount_ = 32;

      kondrat::Vector< Node > firstTable_;
      kondrat::Vector< Node > secondTable_;
      size_t size_;
      PrimHash primaryHash_;
      SecHash secondaryHash_;
      Equal equal_;

      size_t firstIndex(const Key & key) const;
      size_t secondIndex(const Key & key) const;
      size_t subtableCapacity() const noexcept;
      size_t normalizedSubtableCapacity(size_t capacity) const;

      Node * findNode(const Key & key);
      const Node * findNode(const Key & key) const;

      bool placeWithoutRehash(const Node & node);
      void rehashAndPlace(const Node & node);

      template< class V >
      void insertImpl(const Key & key, V && value);
  };

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::CuckooHashTable():
    CuckooHashTable(minSubtableCapacity_ * maxLoadFactorMultiplier_)
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::CuckooHashTable(size_t capacity):
    firstTable_(normalizedSubtableCapacity(capacity), Node()),
    secondTable_(normalizedSubtableCapacity(capacity), Node()),
    size_(0),
    primaryHash_(),
    secondaryHash_(),
    equal_()
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal > &
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::operator=(
    const CuckooHashTable & other)
  {
    if (this != std::addressof(other))
    {
      CuckooHashTable copy(other);
      swap(copy);
    }

    return *this;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::insert(
    const Key & key,
    const Value & value)
  {
    insertImpl(key, value);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::insert(
    const Key & key,
    Value && value)
  {
    insertImpl(key, std::move(value));
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::erase(const Key & key)
  {
    CuckooHashTable copy(*this);
    Node * const node = copy.findNode(key);
    if (node == nullptr)
    {
      return 0;
    }

    *node = Node();
    --copy.size_;

    swap(copy);
    return 1;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::contains(const Key & key) const
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  Value & CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::at(const Key & key)
  {
    Node * const node = findNode(key);
    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const Value & CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::at(const Key & key) const
  {
    const Node * const node = findNode(key);
    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  Value & CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::operator[](const Key & key)
  {
    Node * const node = findNode(key);
    if (node != nullptr)
    {
      return node->data_.second;
    }

    insert(key, Value{});

    return findNode(key)->data_.second;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::clear()
  {
    CuckooHashTable copy(*this);

    for (size_t i = 0; i < copy.firstTable_.getSize(); ++i)
    {
      copy.firstTable_[i] = Node();
      copy.secondTable_[i] = Node();
    }
    copy.size_ = 0;

    swap(copy);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::rehash(size_t newCapacity)
  {
    if (newCapacity == 0)
    {
      throw std::logic_error("invalid capacity");
    }

    CuckooHashTable fresh(newCapacity);

    for (ConstIterator it = cbegin(); it != cend(); ++it)
    {
      fresh.insert(it->first, it->second);
    }

    swap(fresh);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::swap(CuckooHashTable & other) noexcept
  {
    firstTable_.swap(other.firstTable_);
    secondTable_.swap(other.secondTable_);
    std::swap(size_, other.size_);
    std::swap(primaryHash_, other.primaryHash_);
    std::swap(secondaryHash_, other.secondaryHash_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::capacity() const noexcept
  {
    return firstTable_.getSize() + secondTable_.getSize();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Iterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::begin() noexcept
  {
    return Iterator(this, 0, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Iterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::end() noexcept
  {
    return Iterator(this, 2, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::begin() const noexcept
  {
    return ConstIterator(this, 0, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::end() const noexcept
  {
    return ConstIterator(this, 2, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::cbegin() const noexcept
  {
    return begin();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::cend() const noexcept
  {
    return end();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::firstIndex(const Key & key) const
  {
    return primaryHash_(key) % subtableCapacity();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::secondIndex(const Key & key) const
  {
    return secondaryHash_(key) % subtableCapacity();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::subtableCapacity() const noexcept
  {
    return firstTable_.getSize();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::normalizedSubtableCapacity(size_t capacity) const
  {
    if (capacity == 0)
    {
      throw std::logic_error("invalid capacity");
    }

    const size_t halfCapacity = (capacity + 1) / 2;

    return halfCapacity < minSubtableCapacity_ ? minSubtableCapacity_ : halfCapacity;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Node *
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::findNode(const Key & key)
  {
    const size_t firstPosition = firstIndex(key);
    if (firstTable_[firstPosition].occupied_ && equal_(firstTable_[firstPosition].data_.first, key))
    {
      return std::addressof(firstTable_[firstPosition]);
    }

    const size_t secondPosition = secondIndex(key);
    if (secondTable_[secondPosition].occupied_ && equal_(secondTable_[secondPosition].data_.first, key))
    {
      return std::addressof(secondTable_[secondPosition]);
    }

    return nullptr;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Node *
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::findNode(const Key & key) const
  {
    const size_t firstPosition = firstIndex(key);
    if (firstTable_[firstPosition].occupied_ && equal_(firstTable_[firstPosition].data_.first, key))
    {
      return std::addressof(firstTable_[firstPosition]);
    }

    const size_t secondPosition = secondIndex(key);
    if (secondTable_[secondPosition].occupied_ && equal_(secondTable_[secondPosition].data_.first, key))
    {
      return std::addressof(secondTable_[secondPosition]);
    }

    return nullptr;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::placeWithoutRehash(const Node & node)
  {
    kondrat::Vector< Node > firstBackup = firstTable_;
    kondrat::Vector< Node > secondBackup = secondTable_;
    Node current = node;
    size_t tableIndex = 0;
    const size_t maxKickCount = capacity();

    for (size_t kickCount = 0; kickCount < maxKickCount; ++kickCount)
    {
      kondrat::Vector< Node > & table = tableIndex == 0 ? firstTable_ : secondTable_;
      const size_t position =
        tableIndex == 0 ? firstIndex(current.data_.first) : secondIndex(current.data_.first);

      if (!table[position].occupied_)
      {
        table[position] = current;
        return true;
      }

      std::swap(current, table[position]);
      tableIndex = 1 - tableIndex;
    }

    firstTable_.swap(firstBackup);
    secondTable_.swap(secondBackup);
    return false;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::rehashAndPlace(const Node & node)
  {
    size_t newCapacity = capacity();

    for (size_t rehashCount = 0; rehashCount < maxRehashCount_; ++rehashCount)
    {
      if (placeWithoutRehash(node))
      {
        return;
      }

      newCapacity *= maxLoadFactorMultiplier_;
      rehash(newCapacity);
    }

    throw std::logic_error("rehash limit exceeded");
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  template< class V >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::insertImpl(
    const Key & key,
    V && value)
  {
    CuckooHashTable copy(*this);

    Node * const existing = copy.findNode(key);
    if (existing != nullptr)
    {
      existing->data_.second = std::forward< V >(value);
    }
    else
    {
      if ((copy.size_ + 1) * maxLoadFactorMultiplier_ > copy.capacity())
      {
        copy.rehash(copy.capacity() * maxLoadFactorMultiplier_);
      }

      copy.rehashAndPlace(Node(key, std::forward< V >(value)));
      ++copy.size_;
    }

    swap(copy);
  }
}

#include "cuckoo-hash-iter.hpp"
#include "cuckoo-hash-const-iter.hpp"

#endif
