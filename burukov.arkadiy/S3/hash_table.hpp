#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <memory>

#include <vector.hpp>
#include <list.hpp>

#include "hash_iterator.hpp"
#include "siphash.hpp"

namespace burukov
{
  namespace detail
  {
    struct DoubleSlots
    {
      size_t operator()(size_t before) const
      {
        return (before < 10) ? 20 : before * 2;
      }
    };
  }

  template< class KeyType, class ValueType, class HashType = SipHash< KeyType >,
            class EqualType = std::equal_to< KeyType > >
  class HashTable
  {
    friend class HashIterator< KeyType, ValueType, HashType, EqualType >;
    friend class ConstHashIterator< KeyType, ValueType, HashType, EqualType >;

  public:
    using EntryType = std::pair< const KeyType, ValueType >;
    using Iterator = HashIterator< KeyType, ValueType, HashType, EqualType >;
    using ConstIterator = ConstHashIterator< KeyType, ValueType, HashType, EqualType >;
    using SlotPolicy = std::function< size_t(size_t) >;

    HashTable();
    HashTable(const HashTable &other) = default;
    HashTable(HashTable &&other) noexcept = default;
    explicit HashTable(size_t bucketCount);
    ~HashTable() = default;

    HashTable &operator=(const HashTable &other) = default;
    HashTable &operator=(HashTable &&other) noexcept = default;

    void swap(HashTable &other) noexcept;

    void add(const KeyType &key, const ValueType &value);
    void add(const KeyType &key, ValueType &&value);

    void erase(const KeyType &key);

    ValueType &at(const KeyType &key);
    const ValueType &at(const KeyType &key) const;

    bool contains(const KeyType &key) const;

    void rehash(size_t newBucketCount);
    void clear() noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    double loadFactor() const noexcept;
    size_t longestChain() const noexcept;

    double maxLoadFactor() const noexcept;
    void maxLoadFactor(double value);
    size_t maxChainLength() const noexcept;
    void maxChainLength(size_t value);

    void slotPolicy(SlotPolicy policy);

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

  private:
    Vector< List< EntryType > > buckets_;
    size_t size_;
    HashType hasher_;
    EqualType equal_;
    double maxLoadFactor_;
    size_t maxChainLength_;
    SlotPolicy slotPolicy_;

    void allocateBuckets(size_t count);
    bool exceedsLimits(size_t chainLength) const noexcept;

    template< class V >
    void addImpl(const KeyType &key, V &&value);
  };

  template< class KeyType, class ValueType, class HashType, class EqualType >
  HashTable< KeyType, ValueType, HashType, EqualType >::HashTable():
    HashTable(16)
  {}

  template< class KeyType, class ValueType, class HashType, class EqualType >
  HashTable< KeyType, ValueType, HashType, EqualType >::HashTable(size_t bucketCount):
    size_(0),
    hasher_(),
    equal_(),
    maxLoadFactor_(0.0),
    maxChainLength_(0),
    slotPolicy_(detail::DoubleSlots())
  {
    if (bucketCount == 0)
    {
      bucketCount = 16;
    }
    allocateBuckets(bucketCount);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::swap(HashTable &other) noexcept
  {
    buckets_.swap(other.buckets_);
    std::swap(size_, other.size_);
    std::swap(hasher_, other.hasher_);
    std::swap(equal_, other.equal_);
    std::swap(maxLoadFactor_, other.maxLoadFactor_);
    std::swap(maxChainLength_, other.maxChainLength_);
    std::swap(slotPolicy_, other.slotPolicy_);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  bool HashTable< KeyType, ValueType, HashType, EqualType >::exceedsLimits(size_t chainLength) const noexcept
  {
    if (maxChainLength_ != 0 && chainLength > maxChainLength_)
    {
      return true;
    }
    if (maxLoadFactor_ > 0.0)
    {
      const double factor = static_cast< double >(size_) / static_cast< double >(buckets_.getSize());
      if (factor > maxLoadFactor_)
      {
        return true;
      }
    }
    return false;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  template< class V >
  void HashTable< KeyType, ValueType, HashType, EqualType >::addImpl(const KeyType &key, V &&value)
  {
    const size_t index = hasher_(key) % buckets_.getSize();
    List< EntryType > chain(buckets_[index]);
    size_t chainLength = 0;
    bool found = false;
    for (auto it = chain.begin(); it != chain.end(); ++it)
    {
      ++chainLength;
      if (equal_(it->first, key))
      {
        it->second = std::forward< V >(value);
        found = true;
        break;
      }
    }
    if (found)
    {
      buckets_[index] = std::move(chain);
      return;
    }
    chain.pushBack(EntryType(key, std::forward< V >(value)));
    ++chainLength;
    buckets_[index] = std::move(chain);
    ++size_;
    if (exceedsLimits(chainLength))
    {
      rehash(slotPolicy_(buckets_.getSize()));
    }
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::add(const KeyType &key, const ValueType &value)
  {
    addImpl(key, value);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::add(const KeyType &key, ValueType &&value)
  {
    addImpl(key, std::move(value));
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::erase(const KeyType &key)
  {
    const size_t index = hasher_(key) % buckets_.getSize();
    List< EntryType > rebuilt;
    bool erased = false;
    for (auto it = buckets_[index].cbegin(); it != buckets_[index].cend(); ++it)
    {
      if (!erased && equal_(it->first, key))
      {
        erased = true;
        continue;
      }
      rebuilt.pushBack(*it);
    }
    if (!erased)
    {
      return;
    }
    buckets_[index] = std::move(rebuilt);
    --size_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  ValueType &HashTable< KeyType, ValueType, HashType, EqualType >::at(const KeyType &key)
  {
    const size_t index = hasher_(key) % buckets_.getSize();
    for (auto it = buckets_[index].begin(); it != buckets_[index].end(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }
    throw std::out_of_range("key not found");
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  const ValueType &HashTable< KeyType, ValueType, HashType, EqualType >::at(const KeyType &key) const
  {
    const size_t index = hasher_(key) % buckets_.getSize();
    for (auto it = buckets_[index].cbegin(); it != buckets_[index].cend(); ++it)
    {
      if (equal_(it->first, key))
      {
        return it->second;
      }
    }
    throw std::out_of_range("key not found");
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  bool HashTable< KeyType, ValueType, HashType, EqualType >::contains(const KeyType &key) const
  {
    const size_t index = hasher_(key) % buckets_.getSize();
    for (auto it = buckets_[index].cbegin(); it != buckets_[index].cend(); ++it)
    {
      if (equal_(it->first, key))
      {
        return true;
      }
    }
    return false;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::rehash(size_t newBucketCount)
  {
    if (newBucketCount <= buckets_.getSize())
    {
      return;
    }
    HashTable fresh(newBucketCount);
    fresh.maxLoadFactor_ = maxLoadFactor_;
    fresh.maxChainLength_ = maxChainLength_;
    fresh.slotPolicy_ = slotPolicy_;
    for (size_t i = 0; i < buckets_.getSize(); ++i)
    {
      for (auto it = buckets_[i].cbegin(); it != buckets_[i].cend(); ++it)
      {
        const size_t index = fresh.hasher_(it->first) % fresh.buckets_.getSize();
        fresh.buckets_[index].pushBack(*it);
        ++fresh.size_;
      }
    }
    swap(fresh);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::clear() noexcept
  {
    for (size_t i = 0; i < buckets_.getSize(); ++i)
    {
      buckets_[i].clear();
    }
    size_ = 0;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  size_t HashTable< KeyType, ValueType, HashType, EqualType >::size() const noexcept
  {
    return size_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  bool HashTable< KeyType, ValueType, HashType, EqualType >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  double HashTable< KeyType, ValueType, HashType, EqualType >::loadFactor() const noexcept
  {
    return static_cast< double >(size_) / static_cast< double >(buckets_.getSize());
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  size_t HashTable< KeyType, ValueType, HashType, EqualType >::longestChain() const noexcept
  {
    size_t longest = 0;
    for (size_t i = 0; i < buckets_.getSize(); ++i)
    {
      size_t current = 0;
      for (auto it = buckets_[i].cbegin(); it != buckets_[i].cend(); ++it)
      {
        ++current;
      }
      if (current > longest)
      {
        longest = current;
      }
    }
    return longest;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  double HashTable< KeyType, ValueType, HashType, EqualType >::maxLoadFactor() const noexcept
  {
    return maxLoadFactor_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::maxLoadFactor(double value)
  {
    if (value < 0.0)
    {
      throw std::invalid_argument("negative load factor");
    }
    maxLoadFactor_ = value;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  size_t HashTable< KeyType, ValueType, HashType, EqualType >::maxChainLength() const noexcept
  {
    return maxChainLength_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::maxChainLength(size_t value)
  {
    maxChainLength_ = value;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::slotPolicy(SlotPolicy policy)
  {
    if (!policy)
    {
      throw std::invalid_argument("empty slot policy");
    }
    slotPolicy_ = std::move(policy);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashTable< KeyType, ValueType, HashType, EqualType >::Iterator
  HashTable< KeyType, ValueType, HashType, EqualType >::begin() noexcept
  {
    return Iterator(std::addressof(buckets_), buckets_.getSize(), 0);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashTable< KeyType, ValueType, HashType, EqualType >::Iterator
  HashTable< KeyType, ValueType, HashType, EqualType >::end() noexcept
  {
    return Iterator();
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashTable< KeyType, ValueType, HashType, EqualType >::ConstIterator
  HashTable< KeyType, ValueType, HashType, EqualType >::begin() const noexcept
  {
    return ConstIterator(std::addressof(buckets_), buckets_.getSize(), 0);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashTable< KeyType, ValueType, HashType, EqualType >::ConstIterator
  HashTable< KeyType, ValueType, HashType, EqualType >::end() const noexcept
  {
    return ConstIterator();
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashTable< KeyType, ValueType, HashType, EqualType >::ConstIterator
  HashTable< KeyType, ValueType, HashType, EqualType >::cbegin() const noexcept
  {
    return begin();
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashTable< KeyType, ValueType, HashType, EqualType >::ConstIterator
  HashTable< KeyType, ValueType, HashType, EqualType >::cend() const noexcept
  {
    return end();
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashTable< KeyType, ValueType, HashType, EqualType >::allocateBuckets(size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      buckets_.pushBack(List< EntryType >());
    }
  }
}

#endif
