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
    struct Node
    {
      std::pair<Key, Value> kv;
      Node *next;
      Node(const Key &k, const Value &v, Node *n) : kv(k, v), next(n) {}
    };

    Node **buckets_;
    size_t bucketCount_;
    size_t size_;
    HashFunc hasher_;
    Equal equal_;
    double maxLoadFactor_;

  public:
    class Iterator;
    class ConstIterator;

    HashTable(size_t slots = 16):
      buckets_(new Node*[slots]()),
      bucketCount_(slots),
      size_(0),
      hasher_(HashFunc()),
      equal_(Equal()),
      maxLoadFactor_(0.75)
    {
      for (size_t i = 0; i < bucketCount_; ++i) buckets_[i] = nullptr;
    }

    ~HashTable()
    {
      clear();
      delete[] buckets_;
    }

    HashTable(const HashTable &other):
      buckets_(new Node*[other.bucketCount_]()),
      bucketCount_(other.bucketCount_),
      size_(0),
      hasher_(other.hasher_),
      equal_(other.equal_),
      maxLoadFactor_(other.maxLoadFactor_)
    {
      for (size_t i = 0; i < bucketCount_; ++i)
      {
        Node *curr = other.buckets_[i];
        while (curr)
        {
          add(curr->kv.first, curr->kv.second);
          curr = curr->next;
        }
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
      size_t idx = hasher_(k) % bucketCount_;
      buckets_[idx] = new Node(k, v, buckets_[idx]);
      ++size_;
    }

    bool drop(const Key &key)
    {
      size_t idx = hasher_(key) % bucketCount_;
      Node *curr = buckets_[idx];
      Node *prev = nullptr;

      while (curr)
      {
        if (equal_(curr->kv.first, key))
        {
          if (prev) prev->next = curr->next;
          else buckets_[idx] = curr->next;
          delete curr;
          --size_;
          return true;
        }
        prev = curr;
        curr = curr->next;
      }
      return false;
    }

    bool has(const Key &k) const
    {
      size_t idx = hasher_(k) % bucketCount_;
      Node *curr = buckets_[idx];
      while (curr)
      {
        if (equal_(curr->kv.first, k)) return true;
        curr = curr->next;
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
        Node *curr = buckets_[i];
        while (curr)
        {
          Node *next = curr->next;
          delete curr;
          curr = next;
        }
        buckets_[i] = nullptr;
      }
      size_ = 0;
    }

    void rehash(size_t slots)
    {
      if (slots == 0) slots = 1;
      Node **newBuckets = new Node*[slots]();
      for (size_t i = 0; i < bucketCount_; ++i)
      {
        Node *curr = buckets_[i];
        while (curr)
        {
          Node *next = curr->next;
          size_t newIdx = hasher_(curr->kv.first) % slots;
          curr->next = newBuckets[newIdx];
          newBuckets[newIdx] = curr;
          curr = next;
        }
      }
      delete[] buckets_;
      buckets_ = newBuckets;
      bucketCount_ = slots;
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
      size_t bucketIdx_;
      Node *node_;

      Iterator(HashTable *t, size_t b, Node *n) : table_(t), bucketIdx_(b), node_(n)
      {
        if (!node_ && table_) advance();
      }

    public:
      Iterator() : table_(nullptr), bucketIdx_(0), node_(nullptr) {}
      std::pair<Key, Value> &operator*() { return node_->kv; }
      std::pair<Key, Value> *operator->() { return &node_->kv; }
      bool operator!=(const Iterator &rhs) const { return node_ != rhs.node_; }
      Iterator &operator++()
      {
        if (node_->next) node_ = node_->next;
        else
        {
          bucketIdx_++;
          advance();
        }
        return *this;
      }

    private:
      void advance()
      {
        while (bucketIdx_ < table_->bucketCount_ && !table_->buckets_[bucketIdx_])
          bucketIdx_++;
        node_ = (bucketIdx_ < table_->bucketCount_) ? table_->buckets_[bucketIdx_] : nullptr;
      }
    };

    class ConstIterator
    {
      friend class HashTable;
      const HashTable *table_;
      size_t bucketIdx_;
      const Node *node_;

      ConstIterator(const HashTable *t, size_t b, const Node *n) : table_(t), bucketIdx_(b), node_(n)
      {
        if (!node_ && table_) advance();
      }

    public:
      ConstIterator() : table_(nullptr), bucketIdx_(0), node_(nullptr) {}
      const std::pair<Key, Value> &operator*() const { return node_->kv; }
      const std::pair<Key, Value> *operator->() const { return &node_->kv; }
      bool operator!=(const ConstIterator &rhs) const { return node_ != rhs.node_; }
      ConstIterator &operator++()
      {
        if (node_->next) node_ = node_->next;
        else
        {
          bucketIdx_++;
          advance();
        }
        return *this;
      }

    private:
      void advance()
      {
        while (bucketIdx_ < table_->bucketCount_ && !table_->buckets_[bucketIdx_])
          bucketIdx_++;
        node_ = (bucketIdx_ < table_->bucketCount_) ? table_->buckets_[bucketIdx_] : nullptr;
      }
    };

    Iterator begin() { return Iterator(this, 0, buckets_[0]); }
    Iterator end() { return Iterator(this, bucketCount_, nullptr); }
    ConstIterator begin() const { return ConstIterator(this, 0, buckets_[0]); }
    ConstIterator end() const { return ConstIterator(this, bucketCount_, nullptr); }
    Iterator find(const Key &k)
    {
      size_t idx = hasher_(k) % bucketCount_;
      Node *curr = buckets_[idx];
      while (curr)
      {
        if (equal_(curr->kv.first, k))
        {
          return Iterator(this, idx, curr);
        }
        curr = curr->next;
      }
      return end();
    }

    ConstIterator find(const Key &k) const
    {
      size_t idx = hasher_(k) % bucketCount_;
      const Node *curr = buckets_[idx];
      while (curr)
      {
        if (equal_(curr->kv.first, k))
        {
          return ConstIterator(this, idx, curr);
        }
        curr = curr->next;
      }
      return end();
    }
  };
}

#endif
