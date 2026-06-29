#ifndef MY_HASHTABLE_HPP
#define MY_HASHTABLE_HPP

#include <stdexcept>
#include <utility>

#include "../common/list.hpp"
#include "my_citer_hashtable.hpp"
#include "my_iter_hashtable.hpp"
#include "my_node_hashtable.hpp"

namespace zubarev
{

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {

  public:
    using Node = detail::NodeHashTable< Key, Value >;
    using OverflowList = List< Node >;
    using OverflowIter = LIter< Node >;
    using Iter = IterHashTable< Key, Value, Hash, Equal >;
    using CIter = CIterHashTable< Key, Value, Hash, Equal >;

    using Table = HashTable< Key, Value, Hash, Equal >;
    HashTable();
    HashTable(size_t, size_t, Node*, size_t*, OverflowList, Hash, Equal);
    ~HashTable() noexcept;
    HashTable(const HashTable& table);
    HashTable(HashTable&& table) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    Value& operator[](const Key& k);
    Value& at(const Key& id);
    const Value& at(const Key& id) const;

    void swap(Table& rhs) noexcept;

    void insert(const Key& k, const Value& v);
    size_t erase(const Key& k);
    bool contains(const Key& k) const;
    void rehash(size_t slots);

    Iter begin();
    Iter end();

    CIter cbegin() const;
    CIter cend() const;

    CIter begin() const;
    CIter end() const;

  private:
    friend class IterHashTable< Key, Value, Hash, Equal >;
    friend class CIterHashTable< Key, Value, Hash, Equal >;

    size_t bucket_count_;
    size_t bucket_capacity_;

    Node* data_;
    size_t* sizes_;
    OverflowList overflow_bucket_;
    Hash hasher_;
    Equal equaler_;

    size_t getBucketIndex(const Key& k) const
    {
      return static_cast< size_t >(hasher_(k)) % bucket_count_;
    }

    Node* find_el(const Key& k) noexcept
    {
      return const_cast< Node* >(static_cast< const HashTable* >(this)->find_el(k));
    }

    const Node* find_el(const Key& k) const noexcept
    {
      for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
        if (equaler_(k, (*it).key) && (*it).is_val) {
          return std::addressof(*it);
        }
      }
      size_t buc_idx = getBucketIndex(k);
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t idx = bucket_capacity_ * buc_idx + i;
        if (equaler_(data_[idx].key, k) && data_[idx].is_val) {

          return std::addressof(data_[idx]);
        }
      }
      return nullptr;
    }
  };
  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    bucket_count_(8),
    bucket_capacity_(8),
    data_(nullptr),
    sizes_(nullptr),
    overflow_bucket_(),
    hasher_(),
    equaler_()
  {
    data_ = new Node[bucket_count_ * bucket_capacity_]();

    try {
      sizes_ = new size_t[bucket_count_]();
    } catch (...) {
      delete[] data_;
      throw;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucket_count,
                                                  size_t bucket_capacity,
                                                  Node* data,
                                                  size_t* sizes,
                                                  OverflowList overflow_bucket,
                                                  Hash hasher,
                                                  Equal equaler):
    bucket_count_(bucket_count),
    bucket_capacity_(bucket_capacity),
    data_(nullptr),
    sizes_(nullptr),
    overflow_bucket_(overflow_bucket),
    hasher_(hasher),
    equaler_(equaler)
  {
    if (data) {
      data_ = data;
    } else {
      data_ = new Node[bucket_count_ * bucket_capacity_]();
    }
    if (sizes) {
      sizes_ = sizes;
    } else {
      try {
        sizes_ = new size_t[bucket_count_]();
      } catch (...) {
        delete[] data_;
        throw;
      }
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable() noexcept
  {
    delete[] data_;
    delete[] sizes_;
  }
  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& table):
    bucket_count_(table.bucket_count_),
    bucket_capacity_(table.bucket_capacity_),
    data_(),
    sizes_(),
    overflow_bucket_(),
    hasher_(table.hasher_),
    equaler_(table.equaler_)
  {
    data_ = new Node[table.bucket_count_ * table.bucket_capacity_]();
    try {
      sizes_ = new size_t[bucket_count_]();
    } catch (...) {
      delete[] data_;
      throw;
    }
    for (auto it = table.begin(); it != table.end(); ++it) {
      insert(it->key, it->val);
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& table) noexcept:

    bucket_count_(std::exchange(table.bucket_count_, 0)),
    bucket_capacity_(std::exchange(table.bucket_capacity_, 0)),
    data_(std::exchange(table.data_, nullptr)),
    sizes_(std::exchange(table.sizes_, nullptr)),
    overflow_bucket_(std::move(table.overflow_bucket_)),
    hasher_(std::move(table.hasher_)),
    equaler_(std::move(table.equaler_))
  {}
  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& rhs)
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    Table cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& rhs) noexcept
  {
    if (this != &rhs) {
      swap(rhs);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
  {
    Node* el = find_el(k);
    if (el) {
      return el->val;
    } else {
      insert(k, Value{});
      return find_el(k)->val;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k)
  {
    Node* el = find_el(k);
    if (el) {
      return el->val;
    } else {
      throw std::out_of_range("HashTable: index out of range");
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
  {
    const Node* el = find_el(k);
    if (el) {
      return el->val;
    } else {
      throw std::out_of_range("HashTable: index out of range");
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(Table& rhs) noexcept
  {
    std::swap(bucket_count_, rhs.bucket_count_);
    std::swap(bucket_capacity_, rhs.bucket_capacity_);
    std::swap(data_, rhs.data_);
    std::swap(sizes_, rhs.sizes_);
    std::swap(overflow_bucket_, rhs.overflow_bucket_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(equaler_, rhs.equaler_);
  }
  template< class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    Iter it(0, 0, overflow_bucket_.begin(), this);

    while (it.bucket_index_ < bucket_count_ && it.element_index_ < bucket_capacity_ &&
           !data_[it.bucket_index_ * bucket_capacity_ + it.element_index_].is_val) {
      ++it;
      if (it.is_in_overflow())
        break;
    }

    if (it.is_in_overflow() && it.overflow_el_ == overflow_bucket_.end()) {
      return end();
    }

    return it;
  }
  template< class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template< class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    auto* non_const_this = const_cast< HashTable* >(this);

    return non_const_this->begin();
  }
  template< class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return CIter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template< class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin() const
  {
    return cbegin();
  }
  template< class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end() const
  {
    return CIter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::insert(const Key& k, const Value& v)
  {
    if (contains(k)) {
      return;
    }
    size_t buc_idx = getBucketIndex(k);
    for (size_t i = 0; i < bucket_capacity_; ++i) {
      if (!data_[bucket_capacity_ * buc_idx + i].is_val) {
        data_[bucket_capacity_ * buc_idx + i] = Node(k, v, true);
        sizes_[buc_idx]++;
        return;
      }
    }
    overflow_bucket_.push_back(Node(k, v, true));
  }
  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& k)
  {
    size_t buc_idx = getBucketIndex(k);

    auto prev = overflow_bucket_.before_begin();

    for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it, ++prev) {
      if (equaler_(k, (*it).key) && (*it).is_val) {
        overflow_bucket_.erase_after(prev);
        return 1;
      }
    }

    for (size_t i = 0; i < bucket_capacity_; ++i) {
      size_t idx = bucket_capacity_ * buc_idx + i;
      if (equaler_(data_[idx].key, k) && data_[idx].is_val) {
        data_[idx] = Node{};
        sizes_[buc_idx]--;
        return 1;
      }
    }
    return 0;
  }
  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::contains(const Key& k) const
  {

    size_t buc_idx = getBucketIndex(k);

    for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
      if (equaler_(k, (*it).key) && (*it).is_val) {
        return true;
      }
    }

    for (size_t i = 0; i < bucket_capacity_; ++i) {
      size_t idx = bucket_capacity_ * buc_idx + i;
      if (equaler_(data_[idx].key, k) && data_[idx].is_val) {

        return true;
      }
    }

    return false;
  }
  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    Table tmp(slots, bucket_capacity_, nullptr, nullptr, OverflowList(), hasher_, equaler_);
    for (size_t i = 0; i < bucket_count_; ++i) {
      for (size_t j = 0; j < bucket_capacity_; ++j) {
        size_t idx = bucket_capacity_ * i + j;
        if (data_[idx].is_val) {
          tmp.insert(data_[idx].key, data_[idx].val);
        }
      }
    }

    for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
      if ((*it).is_val) {
        tmp.insert((*it).key, (*it).val);
      }
    }
    swap(tmp);
  }

}

#endif
