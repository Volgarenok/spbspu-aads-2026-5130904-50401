#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <functional>
#include <stdexcept>
#include <memory>
#include "bidir_list.hpp"
#include "vector.hpp"
#include "hmac_hash.hpp"

namespace tarasenko
{
  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableForwardIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstForwardIterator;

  template< class Key, class Value, class Hash, class Equal >
  using Table = HashTable< Key, Value, Hash, Equal >;

  template< class Key, class Value, class Hash, class Equal >
  using Iterator = HashTableForwardIterator< Key, Value, Hash, Equal >;

  template< class Key, class Value, class Hash, class Equal >
  using ConstIterator = HashTableConstForwardIterator< Key, Value, Hash, Equal >;

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    using Bucket = BidirList< std::pair< Key, Value > >;

    HashTable(size_t slots = 64, const Hash& hash = Hash(), const Equal& equal = Equal());

    void add(const Key& key, const Value& val);
    void add(const Key& key, Value&& val);
    size_t drop(const Key& key);
    const Value& at(const Key& key) const;
    Value& at(const Key& key);
    Value& operator[](const Key& key);
    Value& operator[](Key&& key);
    bool has(const Key& key) const;
    void rehash(size_t slots);

    size_t size() const;
    size_t capacity() const;
    bool empty() const;

    void swap(Table< Key, Value, Hash, Equal >& rhs) noexcept;

    Iterator< Key, Value, Hash, Equal > begin();
    Iterator< Key, Value, Hash, Equal > end();

    ConstIterator< Key, Value, Hash, Equal > begin() const;
    ConstIterator< Key, Value, Hash, Equal > end() const;

    ConstIterator< Key, Value, Hash, Equal > cbegin() const;
    ConstIterator< Key, Value, Hash, Equal > cend() const;

  private:
    friend class HashTableForwardIterator< Key, Value, Hash, Equal >;
    friend class HashTableConstForwardIterator< Key, Value, Hash, Equal >;

    Vector< Bucket > table_;
    size_t size_ = 0;
    Hash hash_;
    Equal equal_;
  };

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const Table< Key, Value, Hash, Equal >& lhs,
    const Table< Key, Value, Hash, Equal >& rhs);

  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const Table< Key, Value, Hash, Equal >& lhs,
    const Table< Key, Value, Hash, Equal >& rhs);

  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  class HashTableForwardIterator
  {
  public:
    using Bucket = BidirList< std::pair< Key, Value > >;
    using Pair = std::pair< Key, Value >;

    HashTableForwardIterator();

    Pair& operator*();
    Pair* operator->();

    Iterator< Key, Value, Hash, Equal >& operator++();
    Iterator< Key, Value, Hash, Equal > operator++(int);

    bool operator==(const Iterator< Key, Value, Hash, Equal >& rhs) const;
    bool operator!=(const Iterator< Key, Value, Hash, Equal >& rhs) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;

    Table< Key, Value, Hash, Equal >* owner_;
    VecIt< Bucket > bucketIt_;
    ListIter< Pair > listIt_;

    HashTableForwardIterator(Table< Key, Value, Hash, Equal >* owner,
      VecIt< Bucket > bucketIt,
      ListIter< Pair > listIt);
  };

  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  class HashTableConstForwardIterator
  {
  public:
    using Bucket = BidirList< std::pair< Key, Value > >;
    using Pair = std::pair< Key, Value >;

    HashTableConstForwardIterator();

    const Pair& operator*() const;
    const Pair* operator->() const;

    ConstIterator< Key, Value, Hash, Equal >& operator++();
    ConstIterator< Key, Value, Hash, Equal > operator++(int);

    bool operator==(const ConstIterator< Key, Value, Hash, Equal >& rhs) const;
    bool operator!=(const ConstIterator< Key, Value, Hash, Equal >& rhs) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;

    const Table< Key, Value, Hash, Equal >* owner_;
    VecConstIt< Bucket > bucketIt_;
    ListConstIter< Pair > listIt_;

    HashTableConstForwardIterator(const Table< Key, Value, Hash, Equal >* owner,
      VecConstIt< Bucket > bucketIt,
      ListConstIter< Pair > listIt);
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots, const Hash& hash, const Equal& equal):
    table_(slots > 0 ? slots : 1, Bucket()),
    hash_(hash),
    equal_(equal)
  {}

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& val)
  {
    size_t slot = hash_(key) % table_.getSize();
    const Bucket& list = table_[slot];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return;
      }
    }
    table_[slot].push_front({key, val});
    ++size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, Value&& val)
  {
    size_t slot = hash_(key) % table_.getSize();
    const Bucket& list = table_[slot];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return;
      }
    }
    table_[slot].push_front({key, std::move(val)});
    ++size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    size_t slot = hash_(key) % table_.getSize();
    Bucket& list = table_[slot];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        list.erase(it);
        --size_;
        return 1;
      }
    }
    return 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    size_t slot = hash_(key) % table_.getSize();
    const Bucket& list = table_[slot];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }
    throw std::runtime_error("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
  {
    try
    {
      at(key);
    }
    catch (const std::runtime_error&)
    {
      return false;
    }
    return true;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::capacity() const
  {
    return table_.getSize();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    Table< Key, Value, Hash, Equal > copy(slots);
    for (auto it = begin(); it != end(); ++it)
    {
      copy.add(it->first, it->second);
    }
    swap(copy);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(Table< Key, Value, Hash, Equal >& rhs) noexcept
  {
    table_.swap(rhs.table_);
    std::swap(size_, rhs.size_);
    std::swap(hash_, rhs.hash_);
    std::swap(equal_, rhs.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const Table< Key, Value, Hash, Equal >& lhs,
    const Table< Key, Value, Hash, Equal >& rhs)
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    try
    {
      for (auto it = lhs.begin(); it != lhs.end(); ++it)
      {
        if (rhs.at(it->first) != it->second)
        {
          return false;
        }
      }
    }
    catch (...)
    {
      return false;
    }
    return true;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const Table< Key, Value, Hash, Equal >& lhs,
    const Table< Key, Value, Hash, Equal >& rhs)
  {
    return !(lhs == rhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename Iterator< Key, Value, Hash, Equal >::Pair&
    HashTableForwardIterator< Key, Value, Hash, Equal >::operator*()
  {
    return *listIt_;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename Iterator< Key, Value, Hash, Equal >::Pair*
    HashTableForwardIterator< Key, Value, Hash, Equal >::operator->()
  {
    return std::addressof(**this);
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal >& HashTableForwardIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++listIt_;
    if (listIt_ == bucketIt_->end())
    {
      ++bucketIt_;
      while (bucketIt_ != owner_->table_.end() && bucketIt_->empty())
      {
        ++bucketIt_;
      }
      if (bucketIt_ != owner_->table_.end())
      {
        listIt_ = bucketIt_->begin();
      }
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal > HashTableForwardIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    Iterator< Key, Value, Hash, Equal > copy = *this;
    ++(*this);
    return copy;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableForwardIterator< Key, Value, Hash, Equal >::operator==(
    const Iterator< Key, Value, Hash, Equal >& rhs) const
  {
    if (owner_ != rhs.owner_ || bucketIt_ != rhs.bucketIt_)
    {
      return false;
    }
    if (bucketIt_ == owner_->table_.end())
    {
      return true;
    }
    return listIt_ == rhs.listIt_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableForwardIterator< Key, Value, Hash, Equal >::operator!=(
    const Iterator< Key, Value, Hash, Equal >& rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableForwardIterator< Key, Value, Hash, Equal >::HashTableForwardIterator():
    owner_(nullptr)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTableForwardIterator< Key, Value, Hash, Equal >::HashTableForwardIterator(
    Table< Key, Value, Hash, Equal >* owner,
    VecIt< Bucket > bucketIt,
    ListIter< Pair > listIt):
    owner_(owner),
    bucketIt_(bucketIt),
    listIt_(listIt)
  {}

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    auto bucketIt = table_.begin();
    while (bucketIt != table_.end() && bucketIt->empty())
    {
      ++bucketIt;
    }
    if (bucketIt == table_.end())
    {
      return end();
    }

    return Iterator< Key, Value, Hash, Equal >(this, bucketIt, bucketIt->begin());
  }

  template< class Key, class Value, class Hash, class Equal >
  Iterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return Iterator< Key, Value, Hash, Equal >(this, table_.end(),
      ListIter< typename Iterator< Key, Value, Hash, Equal >::Pair >());
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstForwardIterator< Key, Value, Hash, Equal >::HashTableConstForwardIterator():
    owner_(nullptr)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstForwardIterator< Key, Value, Hash, Equal >::HashTableConstForwardIterator(
    const Table< Key, Value, Hash, Equal >* owner,
    VecConstIt< Bucket > bucketIt,
    ListConstIter< Pair > listIt):
    owner_(owner),
    bucketIt_(bucketIt),
    listIt_(listIt)
  {}

  template< class Key, class Value, class Hash, class Equal >
  const typename ConstIterator< Key, Value, Hash, Equal >::Pair&
    HashTableConstForwardIterator< Key, Value, Hash, Equal >::operator*() const
  {
    return *listIt_;
  }

  template< class Key, class Value, class Hash, class Equal >
  const typename ConstIterator< Key, Value, Hash, Equal >::Pair*
    HashTableConstForwardIterator< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(**this);
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal >& HashTableConstForwardIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++listIt_;
    if (listIt_ == bucketIt_->end())
    {
      ++bucketIt_;
      while (bucketIt_ != owner_->table_.end() && bucketIt_->empty())
      {
        ++bucketIt_;
      }
      if (bucketIt_ != owner_->table_.end())
      {
        listIt_ = bucketIt_->begin();
      }
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal > HashTableConstForwardIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    ConstIterator< Key, Value, Hash, Equal > copy = *this;
    ++(*this);
    return copy;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableConstForwardIterator< Key, Value, Hash, Equal >::operator==(
    const ConstIterator< Key, Value, Hash, Equal >& rhs) const
  {
    if (owner_ != rhs.owner_ || bucketIt_ != rhs.bucketIt_)
    {
      return false;
    }
    if (bucketIt_ == owner_->table_.end())
    {
      return true;
    }
    return listIt_ == rhs.listIt_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableConstForwardIterator< Key, Value, Hash, Equal >::operator!=(
    const ConstIterator< Key, Value, Hash, Equal >& rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    auto bucketIt = table_.cbegin();
    while (bucketIt != table_.cend() && bucketIt->empty())
    {
      ++bucketIt;
    }
    if (bucketIt == table_.cend())
    {
      return cend();
    }

    return ConstIterator< Key, Value, Hash, Equal >(this, bucketIt, bucketIt->cbegin());
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return ConstIterator< Key, Value, Hash, Equal >(this, table_.cend(),
      ListConstIter< typename ConstIterator< Key, Value, Hash, Equal >::Pair >());
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin() const
  {
    return cbegin();
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIterator< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end() const
  {
    return cend();
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    size_t slot = hash_(key) % table_.getSize();
    Bucket& list = table_[slot];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }
    throw std::runtime_error("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    size_t slot = hash_(key) % table_.getSize();
    Bucket& list = table_[slot];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }
    list.push_front({key, Value()});
    ++size_;
    return list.front().second;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](Key&& key)
  {
    size_t slot = hash_(key) % table_.getSize();
    Bucket& list = table_[slot];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }
    list.push_front({std::move(key), Value()});
    ++size_;
    return list.front().second;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const
  {
    return size() == 0;
  }

};

#endif
