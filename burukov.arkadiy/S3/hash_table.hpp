#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <vector.hpp>
#include <list.hpp>
#include "hash_iterator.hpp"
#include "siphash.hpp"
#include <functional>
#include <stdexcept>
#include <utility>
#include <algorithm>

namespace burukov
{
  template<typename KeyType, typename ValueType,
           typename HashType = SipHash<KeyType>,
           typename EqualType = std::equal_to<KeyType>>
  class HashTable
  {
    friend class HashIterator<KeyType, ValueType, HashType, EqualType>;
    friend class ConstHashIterator<KeyType, ValueType, HashType, EqualType>;
    using EntryType = std::pair<const KeyType, ValueType>;

  public:
    using Iterator = HashIterator<KeyType, ValueType, HashType, EqualType>;
    using ConstIterator = ConstHashIterator<KeyType, ValueType, HashType, EqualType>;

    explicit HashTable(size_t bucket_count = 16)
      : size_(0), hasher_(), equal_()
    {
      if (bucket_count == 0)
        bucket_count = 16;
      allocateBuckets(bucket_count);
    }

    HashTable(const HashTable &other)
      : size_(0), hasher_(other.hasher_), equal_(other.equal_)
    {
      allocateBuckets(other.buckets_.getSize());
      for (size_t i = 0; i < other.buckets_.getSize(); ++i)
      {
        for (auto it = other.buckets_[i].cbegin(); it != other.buckets_[i].cend(); ++it)
        {
          buckets_[i].pushBack(*it);
          ++size_;
        }
      }
    }

    HashTable(HashTable &&other) noexcept
      : buckets_(std::move(other.buckets_)), size_(other.size_),
        hasher_(std::move(other.hasher_)), equal_(std::move(other.equal_))
    {
      other.size_ = 0;
    }

    HashTable &operator=(HashTable other)
    {
      swap(other);
      return *this;
    }

    ~HashTable() = default;

    void swap(HashTable &other) noexcept
    {
      buckets_.swap(other.buckets_);
      std::swap(size_, other.size_);
      std::swap(hasher_, other.hasher_);
      std::swap(equal_, other.equal_);
    }

    void add(const KeyType &key, const ValueType &value)
    {
      size_t index = hasher_(key) % buckets_.getSize();
      List<EntryType> copy = buckets_[index];
      bool found = false;
      for (auto it = copy.begin(); it != copy.end(); ++it)
      {
        if (equal_(it->first, key))
        {
          it->second = value;
          found = true;
          break;
        }
      }
      if (!found)
      {
        copy.pushBack(EntryType(key, value));
        ++size_;
      }
      buckets_[index] = std::move(copy);
    }

    void erase(const KeyType &key)
    {
      size_t index = hasher_(key) % buckets_.getSize();
      List<EntryType> rebuilt;
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
        return;
      buckets_[index] = std::move(rebuilt);
      --size_;
    }

    ValueType &at(const KeyType &key)
    {
      size_t index = hasher_(key) % buckets_.getSize();
      for (auto it = buckets_[index].begin(); it != buckets_[index].end(); ++it)
      {
        if (equal_(it->first, key))
          return it->second;
      }
      throw std::out_of_range("key not found");
    }

    const ValueType &at(const KeyType &key) const
    {
      size_t index = hasher_(key) % buckets_.getSize();
      for (auto it = buckets_[index].cbegin(); it != buckets_[index].cend(); ++it)
      {
        if (equal_(it->first, key))
          return it->second;
      }
      throw std::out_of_range("key not found");
    }

    bool contains(const KeyType &key) const noexcept
    {
      size_t index = hasher_(key) % buckets_.getSize();
      for (auto it = buckets_[index].cbegin(); it != buckets_[index].cend(); ++it)
      {
        if (equal_(it->first, key))
          return true;
      }
      return false;
    }

    void rehash(size_t new_bucket_count)
    {
      if (new_bucket_count <= buckets_.getSize())
        return;
      HashTable fresh(new_bucket_count);
      for (size_t i = 0; i < buckets_.getSize(); ++i)
      {
        for (auto it = buckets_[i].begin(); it != buckets_[i].end(); ++it)
          fresh.add(it->first, it->second);
      }
      swap(fresh);
    }

    void clear() noexcept
    {
      for (size_t i = 0; i < buckets_.getSize(); ++i)
        buckets_[i].clear();
      size_ = 0;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    Iterator begin()
    {
      return Iterator(&buckets_, buckets_.getSize(), 0);
    }

    Iterator end()
    {
      return Iterator();
    }

    ConstIterator begin() const
    {
      return ConstIterator(&buckets_, buckets_.getSize(), 0);
    }

    ConstIterator end() const
    {
      return ConstIterator();
    }

    ConstIterator cbegin() const
    {
      return begin();
    }

    ConstIterator cend() const
    {
      return end();
    }

  private:
    void allocateBuckets(size_t count)
    {
      for (size_t i = 0; i < count; ++i)
        buckets_.pushBack(List<EntryType>());
    }

    Vector<List<EntryType>> buckets_;
    size_t size_;
    HashType hasher_;
    EqualType equal_;
  };
}

#endif
