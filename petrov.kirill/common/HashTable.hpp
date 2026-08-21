#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>

namespace petrov
{
  template <typename Key, typename Value, typename HashFunc = std::hash<Key>, typename Equal = std::equal_to<Key>>
  class HashTable
  {
  private:
    struct Bucket
    {
      std::pair<Key, Value> kv;
      size_t psl;
      bool occupied;

      Bucket() : psl(0), occupied(false) {}
      Bucket(const Key &k, const Value &v, size_t p) : kv(k, v), psl(p), occupied(true) {}
    };

    Bucket *buckets_;
    size_t bucketCount_;
    size_t size_;
    HashFunc hasher_;
    Equal equal_;
    double maxLoadFactor_;

  public:
    class Iterator;
    class ConstIterator;

    HashTable(size_t slots = 16):
      buckets_(new Bucket[slots]()),
      bucketCount_(slots),
      size_(0),
      hasher_(HashFunc()),
      equal_(Equal()),
      maxLoadFactor_(0.75)
    {
    }

    ~HashTable()
    {
      delete[] buckets_;
    }

    HashTable(const HashTable &other):
      buckets_(new Bucket[other.bucketCount_]()),
      bucketCount_(other.bucketCount_),
      size_(other.size_),
      hasher_(other.hasher_),
      equal_(other.equal_),
      maxLoadFactor_(other.maxLoadFactor_)
    {
      for (size_t i = 0; i < bucketCount_; ++i)
      {
        buckets_[i] = other.buckets_[i];
      }
    }

    HashTable &operator=(const HashTable &other)
    {
      if (this != &other)
      {
        HashTable temp(other);
        std::swap(buckets_, temp.buckets_);
        std::swap(bucketCount_, temp.bucketCount_);
        std::swap(size_, temp.size_);
        std::swap(hasher_, temp.hasher_);
        std::swap(equal_, temp.equal_);
        std::swap(maxLoadFactor_, temp.maxLoadFactor_);
      }
      return *this;
    }

    void setMaxLoadFactor(double maxLoad)
    {
      maxLoadFactor_ = maxLoad;
    }

    void add(const Key &k, const Value &v)
    {
      if (has(k)) return;
      checkRehash();

      Bucket curr(k, v, 0);
      size_t idx = hasher_(k) % bucketCount_;

      while (true)
      {
        if (!buckets_[idx].occupied)
        {
          buckets_[idx] = curr;
          ++size_;
          return;
        }
        if (curr.psl > buckets_[idx].psl)
        {
          std::swap(curr, buckets_[idx]);
        }
        curr.psl++;
        idx = (idx + 1) % bucketCount_;
      }
    }

    bool drop(const Key &key)
    {
      size_t idx = hasher_(key) % bucketCount_;
      size_t dist = 0;

      while (buckets_[idx].occupied)
      {
        if (equal_(buckets_[idx].kv.first, key))
        {
          buckets_[idx].occupied = false;
          --size_;

          size_t currIdx = idx;
          size_t nextIdx = (currIdx + 1) % bucketCount_;

          while (buckets_[nextIdx].occupied && buckets_[nextIdx].psl > 0)
          {
            buckets_[currIdx] = buckets_[nextIdx];
            buckets_[currIdx].psl--;
            buckets_[nextIdx].occupied = false;

            currIdx = nextIdx;
            nextIdx = (currIdx + 1) % bucketCount_;
          }
          return true;
        }
        if (dist > buckets_[idx].psl)
        {
          break;
        }
        dist++;
        idx = (idx + 1) % bucketCount_;
      }
      return false;
    }

    bool has(const Key &k) const
    {
      size_t idx = hasher_(k) % bucketCount_;
      size_t dist = 0;

      while (buckets_[idx].occupied)
      {
        if (equal_(buckets_[idx].kv.first, k)) return true;
        if (dist > buckets_[idx].psl) break;
        dist++;
        idx = (idx + 1) % bucketCount_;
      }
      return false;
    }

    bool isEmpty() const
    {
      return size_ == 0;
    }

    size_t getCapacity() const
    {
      return bucketCount_;
    }

    void swap(HashTable &rhs) noexcept
    {
      std::swap(buckets_, rhs.buckets_);
      std::swap(bucketCount_, rhs.bucketCount_);
      std::swap(size_, rhs.size_);
      std::swap(hasher_, rhs.hasher_);
      std::swap(equal_, rhs.equal_);
      std::swap(maxLoadFactor_, rhs.maxLoadFactor_);
    }

    void clear()
    {
      for (size_t i = 0; i < bucketCount_; ++i)
      {
        buckets_[i].occupied = false;
      }
      size_ = 0;
    }

    void rehash(size_t slots)
    {
      if (slots == 0) slots = 1;
      Bucket *oldBuckets = buckets_;
      size_t oldCapacity = bucketCount_;

      buckets_ = new Bucket[slots]();
      bucketCount_ = slots;
      size_ = 0;

      for (size_t i = 0; i < oldCapacity; ++i)
      {
        if (oldBuckets[i].occupied)
        {
          add(oldBuckets[i].kv.first, oldBuckets[i].kv.second);
        }
      }
      delete[] oldBuckets;
    }

    size_t getSize() const { return size_; }

  private:
    void checkRehash()
    {
      if (static_cast<double>(size_ + 1) / bucketCount_ > maxLoadFactor_)
      {
        rehash(bucketCount_ * 2);
      }
    }

  public:
    class Iterator
    {
      friend class HashTable;
      HashTable *table_;
      size_t idx_;

      Iterator(HashTable *t, size_t i) : table_(t), idx_(i)
      {
        if (table_ && idx_ < table_->bucketCount_ && !table_->buckets_[idx_].occupied)
        {
          advance();
        }
      }

    public:
      Iterator() : table_(nullptr), idx_(0) {}
      std::pair<Key, Value> &operator*() { return table_->buckets_[idx_].kv; }
      std::pair<Key, Value> *operator->() { return &table_->buckets_[idx_].kv; }
      bool operator!=(const Iterator &rhs) const { return idx_ != rhs.idx_; }
      Iterator &operator++()
      {
        idx_++;
        advance();
        return *this;
      }

    private:
      void advance()
      {
        while (idx_ < table_->bucketCount_ && !table_->buckets_[idx_].occupied)
        {
          idx_++;
        }
      }
    };

    class ConstIterator
    {
      friend class HashTable;
      const HashTable *table_;
      size_t idx_;

      ConstIterator(const HashTable *t, size_t i) : table_(t), idx_(i)
      {
        if (table_ && idx_ < table_->bucketCount_ && !table_->buckets_[idx_].occupied)
        {
          advance();
        }
      }

    public:
      ConstIterator() : table_(nullptr), idx_(0) {}
      const std::pair<Key, Value> &operator*() const { return table_->buckets_[idx_].kv; }
      const std::pair<Key, Value> *operator->() const { return &table_->buckets_[idx_].kv; }
      bool operator!=(const ConstIterator &rhs) const { return idx_ != rhs.idx_; }
      ConstIterator &operator++()
      {
        idx_++;
        advance();
        return *this;
      }

    private:
      void advance()
      {
        while (idx_ < table_->bucketCount_ && !table_->buckets_[idx_].occupied)
        {
          idx_++;
        }
      }
    };

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, bucketCount_); }
    ConstIterator begin() const { return ConstIterator(this, 0); }
    ConstIterator end() const { return ConstIterator(this, bucketCount_); }

    Iterator find(const Key &k)
    {
      size_t idx = hasher_(k) % bucketCount_;
      size_t dist = 0;

      while (buckets_[idx].occupied)
      {
        if (equal_(buckets_[idx].kv.first, k))
        {
          return Iterator(this, idx);
        }
        if (dist > buckets_[idx].psl)
        {
          break;
        }
        dist++;
        idx = (idx + 1) % bucketCount_;
      }
      return end();
    }

    ConstIterator find(const Key &k) const
    {
      size_t idx = hasher_(k) % bucketCount_;
      size_t dist = 0;

      while (buckets_[idx].occupied)
      {
        if (equal_(buckets_[idx].kv.first, k))
        {
          return ConstIterator(this, idx);
        }
        if (dist > buckets_[idx].psl)
        {
          break;
        }
        dist++;
        idx = (idx + 1) % bucketCount_;
      }
      return end();
    }
  };
}

#endif
