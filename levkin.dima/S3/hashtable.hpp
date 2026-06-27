#ifndef HASH
#define HASH

#include "hasher.hpp"
#include "list.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

namespace levkin {
  template < class T > struct Equal {
    bool operator()(const T& a, const T& b) const { return a == b; }
  };

  template < class Key, class Value > struct NodeHashTable {
    Key key_;
    Value value_;
    bool is_valid_;

    NodeHashTable() : is_valid_(false) {}
    NodeHashTable(const Key& k, const Value& v, bool valid)
        : key_(k), value_(v), is_valid_(valid)
    {
    }
    NodeHashTable(Key&& k, Value&& v, bool valid)
        : key_(std::move(k)), value_(std::move(v)), is_valid_(valid)
    {
    }
  };

  template <
      class Key,
      class Value,
      class Hash = Sha1Hasher< Key >,
      class EqualTo = Equal< Key > >
  class HashTable;

  template < class Key, class Value, class Hash, class EqualTo, bool IsConst >
  class HashTableIterator
  {
    template < class K, class V, class H, class E > friend class HashTable;
    template < class K, class V, class H, class E, bool C >
    friend class HashTableIterator;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = NodeHashTable< Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::
        conditional< IsConst, const value_type*, value_type* >::type;
    using reference = typename std::
        conditional< IsConst, const value_type&, value_type& >::type;
    using list_iterator = typename std::conditional<
        IsConst,
        LCIter< NodeHashTable< Key, Value > >,
        LIter< NodeHashTable< Key, Value > > >::type;

    HashTableIterator() noexcept
        : table_(nullptr), index_(0), overflow_it_(nullptr)
    {
    }

    HashTableIterator(
        const HashTable< Key, Value, Hash, EqualTo >* table,
        size_t index,
        list_iterator overflow_it = nullptr) noexcept
        : table_(const_cast< HashTable< Key, Value, Hash, EqualTo >* >(table)),
          index_(index), overflow_it_(overflow_it)
    {
    }

    template < bool OtherConst >
    HashTableIterator(
        const HashTableIterator< Key, Value, Hash, EqualTo, OtherConst >& other,
        typename std::enable_if< IsConst && !OtherConst >::type* =
            nullptr) noexcept
        : table_(other.table_), index_(other.index_),
          overflow_it_(other.overflow_it_)
    {
    }

    typename std::conditional< IsConst, const Key&, Key& >::type
    key() const noexcept
    {
      if (index_ < table_->pool_.getSize()) {
        return table_->pool_[index_].key_;
      }
      return overflow_it_->key_;
    }

    typename std::conditional< IsConst, const Value&, Value& >::type
    value() const noexcept
    {
      if (index_ < table_->pool_.getSize()) {
        return table_->pool_[index_].value_;
      }
      return overflow_it_->value_;
    }

    reference operator*() const noexcept
    {
      if (index_ < table_->pool_.getSize()) {
        return table_->pool_[index_];
      }
      return *overflow_it_;
    }

    pointer operator->() const noexcept
    {
      if (index_ < table_->pool_.getSize()) {
        return &(table_->pool_[index_]);
      }
      return &(*overflow_it_);
    }

    HashTableIterator& operator++()
    {
      if (!table_)
        return *this;

      if (index_ < table_->pool_.getSize()) {
        index_++;
        while (index_ < table_->pool_.getSize() &&
               !table_->pool_[index_].is_valid_) {
          index_++;
        }
        if (index_ == table_->pool_.getSize()) {
          overflow_it_ = table_->overflow_.begin();
          while (overflow_it_ != table_->overflow_.end() &&
                 !overflow_it_->is_valid_) {
            ++overflow_it_;
          }
        }
      } else if (overflow_it_ != table_->overflow_.end()) {
        ++overflow_it_;
        while (overflow_it_ != table_->overflow_.end() &&
               !overflow_it_->is_valid_) {
          ++overflow_it_;
        }
      }
      return *this;
    }

    bool operator==(const HashTableIterator& rhs) const noexcept
    {
      if (table_ != rhs.table_)
        return false;
      if (index_ != rhs.index_)
        return false;

      if (index_ == table_->pool_.getSize()) {
        return overflow_it_ == rhs.overflow_it_;
      }
      return true;
    }

    HashTableIterator operator++(int)
    {
      HashTableIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    HashTableIterator next() const
    {
      HashTableIterator tmp = *this;
      return ++tmp;
    }

    bool hasNext() const noexcept { return *this != table_->end(); }

    bool operator!=(const HashTableIterator& rhs) const noexcept
    {
      return !(*this == rhs);
    }

  private:
    HashTable< Key, Value, Hash, EqualTo >* table_;
    size_t index_;
    list_iterator overflow_it_;
  };

  template < class Key, class Value, class Hash, class EqualTo > class HashTable
  {
    template < class K, class V, class H, class E, bool C >
    friend class HashTableIterator;

  public:
    using iterator = HashTableIterator< Key, Value, Hash, EqualTo, false >;
    using const_iterator = HashTableIterator< Key, Value, Hash, EqualTo, true >;

    HashTable() : HashTable(11, 4) {}

    HashTable(size_t num_buckets, size_t bucket_capacity = 4)
        : num_buckets_(num_buckets == 0 ? 11 : num_buckets),
          bucket_capacity_(bucket_capacity == 0 ? 4 : bucket_capacity),
          count_valid_(0)
    {
      size_t total_slots = num_buckets_ * bucket_capacity_;
      for (size_t i = 0; i < total_slots; ++i) {
        pool_.pushBack(NodeHashTable< Key, Value >{});
      }
    }

    HashTable(const HashTable& other)
        : pool_(other.pool_), overflow_(other.overflow_),
          num_buckets_(other.num_buckets_),
          bucket_capacity_(other.bucket_capacity_),
          count_valid_(other.count_valid_)
    {
    }

    HashTable(HashTable&& other) noexcept
        : num_buckets_(0), bucket_capacity_(0), count_valid_(0)
    {
      swap(other);
    }

    HashTable& operator=(const HashTable& other)
    {
      if (this != &other) {
        HashTable tmp(other);
        swap(tmp);
      }
      return *this;
    }

    HashTable& operator=(HashTable&& other) noexcept
    {
      if (this != &other) {
        HashTable tmp(std::move(other));
        swap(tmp);
      }
      return *this;
    }

    void swap(HashTable& other) noexcept
    {
      std::swap(pool_, other.pool_);
      std::swap(overflow_, other.overflow_);
      std::swap(num_buckets_, other.num_buckets_);
      std::swap(bucket_capacity_, other.bucket_capacity_);
      std::swap(count_valid_, other.count_valid_);
    }
    Value& at(const Key& key)
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_ && equal(pool_[curr].key_, key)) {
          return pool_[curr].value_;
        }
      }
      for (auto it = overflow_.begin(); it != overflow_.end(); ++it) {
        if (it->is_valid_ && equal(it->key_, key)) {
          return it->value_;
        }
      }
      throw std::out_of_range("key not found");
    }

    const Value& at(const Key& key) const
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_ && equal(pool_[curr].key_, key)) {
          return pool_[curr].value_;
        }
      }
      for (auto it = overflow_.cbegin(); it != overflow_.cend(); ++it) {
        if (it->is_valid_ && equal(it->key_, key)) {
          return it->value_;
        }
      }
      throw std::out_of_range("key not found");
    }
    void add(const Key& key, const Value& value)
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      size_t first_free_in_pool = pool_.getSize();

      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_) {
          if (equal(pool_[curr].key_, key)) {
            pool_[curr].value_ = value; // Update instead of throwing!
            return;
          }
        } else if (first_free_in_pool == pool_.getSize()) {
          first_free_in_pool = curr;
        }
      }

      auto it = overflow_.begin();
      auto first_free_in_list = overflow_.end();
      while (it != overflow_.end()) {
        if (it->is_valid_) {
          if (equal(it->key_, key)) {
            it->value_ = value; // Update instead of throwing!
            return;
          }
        } else if (first_free_in_list == overflow_.end()) {
          first_free_in_list = it;
        }
        ++it;
      }

      if (first_free_in_pool != pool_.getSize()) {
        pool_[first_free_in_pool].key_ = key;
        pool_[first_free_in_pool].value_ = value;
        pool_[first_free_in_pool].is_valid_ = true;
        count_valid_++;
      } else if (first_free_in_list != overflow_.end()) {
        first_free_in_list->key_ = key;
        first_free_in_list->value_ = value;
        first_free_in_list->is_valid_ = true;
        count_valid_++;
      } else {
        overflow_.pushBack(NodeHashTable< Key, Value >{key, value, true});
        count_valid_++;
      }
    }

    void add(const Key& key, Value&& value)
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      size_t first_free_in_pool = pool_.getSize();

      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_) {
          if (equal(pool_[curr].key_, key)) {
            pool_[curr].value_ =
                std::move(value); // Update instead of throwing!
            return;
          }
        } else if (first_free_in_pool == pool_.getSize()) {
          first_free_in_pool = curr;
        }
      }

      auto it = overflow_.begin();
      auto first_free_in_list = overflow_.end();
      while (it != overflow_.end()) {
        if (it->is_valid_) {
          if (equal(it->key_, key)) {
            it->value_ = std::move(value); // Update instead of throwing!
            return;
          }
        } else if (first_free_in_list == overflow_.end()) {
          first_free_in_list = it;
        }
        ++it;
      }

      if (first_free_in_pool != pool_.getSize()) {
        pool_[first_free_in_pool].key_ = key;
        pool_[first_free_in_pool].value_ = std::move(value);
        pool_[first_free_in_pool].is_valid_ = true;
        count_valid_++;
      } else if (first_free_in_list != overflow_.end()) {
        first_free_in_list->key_ = key;
        first_free_in_list->value_ = std::move(value);
        first_free_in_list->is_valid_ = true;
        count_valid_++;
      } else {
        overflow_.pushBack(
            NodeHashTable< Key, Value >{key, std::move(value), true});
        count_valid_++;
      }
    }

    Value drop(const Key& key)
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_ && equal(pool_[curr].key_, key)) {
          Value removed_value = std::move(pool_[curr].value_);
          pool_[curr].is_valid_ = false;
          count_valid_--;
          return removed_value;
        }
      }
      for (auto it = overflow_.begin(); it != overflow_.end(); ++it) {
        if (it->is_valid_ && equal(it->key_, key)) {
          Value removed_value = std::move(it->value_);
          it->is_valid_ = false;
          count_valid_--;
          return removed_value;
        }
      }
      throw std::runtime_error("No such key");
    }

    bool has(const Key& key) const noexcept
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_ && equal(pool_[curr].key_, key)) {
          return true;
        }
      }
      for (auto it = overflow_.cbegin(); it != overflow_.cend(); ++it) {
        if (it->is_valid_ && equal(it->key_, key)) {
          return true;
        }
      }
      return false;
    }

    Value& get(const Key& key)
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_ && equal(pool_[curr].key_, key)) {
          return pool_[curr].value_;
        }
      }
      for (auto it = overflow_.begin(); it != overflow_.end(); ++it) {
        if (it->is_valid_ && equal(it->key_, key)) {
          return it->value_;
        }
      }
      throw std::logic_error("No such key");
    }

    const Value& get(const Key& key) const
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_ && equal(pool_[curr].key_, key)) {
          return pool_[curr].value_;
        }
      }
      for (auto it = overflow_.cbegin(); it != overflow_.cend(); ++it) {
        if (it->is_valid_ && equal(it->key_, key)) {
          return it->value_;
        }
      }
      throw std::logic_error("No such key");
    }

    Value& operator[](const Key& key)
    {
      Hash hasher;
      EqualTo equal;
      size_t bucket_idx = hasher(key) % num_buckets_;
      size_t home_start = bucket_idx * bucket_capacity_;
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t curr = home_start + i;
        if (pool_[curr].is_valid_ && equal(pool_[curr].key_, key)) {
          return pool_[curr].value_;
        }
      }
      for (auto it = overflow_.begin(); it != overflow_.end(); ++it) {
        if (it->is_valid_ && equal(it->key_, key)) {
          return it->value_;
        }
      }
      add(key, Value{});
      return get(key);
    }

    const Value& operator[](const Key& key) const { return get(key); }

    void rehash(size_t new_num_buckets, size_t new_bucket_capacity)
    {
      HashTable< Key, Value, Hash, EqualTo > new_table(
          new_num_buckets, new_bucket_capacity);

      for (size_t i = 0; i < pool_.getSize(); ++i) {
        if (pool_[i].is_valid_) {
          new_table.add(std::move(pool_[i].key_), std::move(pool_[i].value_));
        }
      }

      for (auto it = overflow_.begin(); it != overflow_.end(); ++it) {
        if (it->is_valid_) {
          new_table.add(std::move(it->key_), std::move(it->value_));
        }
      }

      this->swap(new_table);
    }

    void clear() noexcept
    {
      for (size_t i = 0; i < pool_.getSize(); ++i) {
        pool_[i].is_valid_ = false;
      }
      overflow_.clear();
      count_valid_ = 0;
    }

    size_t size() const noexcept { return num_buckets_ * bucket_capacity_; }
    size_t count() const noexcept { return num_buckets_; }
    size_t countValid() const noexcept { return count_valid_; }
    size_t capacity() const noexcept { return bucket_capacity_; }

    iterator begin() noexcept
    {
      size_t idx = findNextValidInPool(0);
      if (idx < pool_.getSize()) {
        return iterator(this, idx, overflow_.end());
      }
      auto it = overflow_.begin();
      while (it != overflow_.end() && !it->is_valid_) {
        ++it;
      }
      return iterator(this, pool_.getSize(), it);
    }

    iterator end() noexcept
    {
      return iterator(this, pool_.getSize(), overflow_.end());
    }

    const_iterator begin() const noexcept
    {
      size_t idx = findNextValidInPool(0);
      if (idx < pool_.getSize()) {
        return const_iterator(this, idx, overflow_.cend());
      }
      auto it = overflow_.cbegin();
      while (it != overflow_.cend() && !it->is_valid_) {
        ++it;
      }
      return const_iterator(this, pool_.getSize(), it);
    }

    const_iterator end() const noexcept
    {
      return const_iterator(this, pool_.getSize(), overflow_.cend());
    }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

  private:
    size_t findNextValidInPool(size_t index) const noexcept
    {
      while (index < pool_.getSize() && !pool_[index].is_valid_) {
        index++;
      }
      return index;
    }

    stuff::Vector< NodeHashTable< Key, Value > > pool_;
    levkin::List< NodeHashTable< Key, Value > > overflow_;
    size_t num_buckets_;
    size_t bucket_capacity_;
    size_t count_valid_;
  };

}

#endif
