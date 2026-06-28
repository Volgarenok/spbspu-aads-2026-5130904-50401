#ifndef MY_HASHTABLE_HPP
#define MY_HASHTABLE_HPP

#include "../common/my_list/list.hpp"
#include "my_citer_hashtable.hpp"
#include "my_iter_hashtable.hpp"

#include "my_node_hashtable.hpp"

#include <stdexcept>
#include <utility>
namespace zubarev
{

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    friend class IterHashTable< Key, Value, Hash, Equal >;
    friend class CIterHashTable< Key, Value, Hash, Equal >;
  private:
    using Node = NodeHashTable< Key, Value >;
    using OverflowList = List< Node >;
    using OverflowIter = LIter< Node >;
    using Iter = IterHashTable< Key, Value, Hash, Equal >;
    using CIter = CIterHashTable< Key, Value, Hash, Equal >;

    using Table = HashTable< Key, Value, Hash, Equal >;

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
      for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
        if (equaler_(k, (*it).key_) && (*it).is_val_) {
          return std::addressof(*it);
        }
      }
      size_t buc_idx = getBucketIndex(k);
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t idx = bucket_capacity_ * buc_idx + i;
        if (equaler_(data_[idx].key_, k) && data_[idx].is_val_) {

          return &data_[idx];
        }
      }
      return nullptr;
    }

    const Node* find_el(const Key& k) const noexcept
    {
      for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
        if (equaler_(k, (*it).key_) && (*it).is_val_) {
          return std::addressof(*it);
        }
      }
      size_t buc_idx = getBucketIndex(k);
      for (size_t i = 0; i < bucket_capacity_; ++i) {
        size_t idx = bucket_capacity_ * buc_idx + i;
        if (equaler_(data_[idx].key_, k) && data_[idx].is_val_) {

          return &data_[idx];
        }
      }
      return nullptr;
    }

  public:
    HashTable();
    HashTable(size_t, size_t, Node*, size_t*, OverflowList, Hash, Equal);
    ~HashTable();
    HashTable(const HashTable& table);
    HashTable(HashTable&& table) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    Value& operator[](Key k) noexcept;
    const Value& operator[](Key id) const noexcept;
    Value& at(Key id);
    const Value& at(Key id) const;

    void swap(Table& rhs) noexcept;

    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k) const;
    void rehash(size_t slots);

    Iter begin();
    Iter end();

    CIter cbegin() const;
    CIter cend() const;

    CIter begin() const;
    CIter end() const;
  };
  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    bucket_count_(8),
    bucket_capacity_(8),
    data_(new Node[8 * 8]()),
    sizes_(new size_t[8]()),
    overflow_bucket_(),
    hasher_(),
    equaler_()
  {}

  template < class Key, class Value, class Hash, class Equal >
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
      sizes_ = new size_t[bucket_count_]();
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    delete[] data_;
    delete[] sizes_;
  }
  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& table):
    bucket_count_(table.bucket_count_),
    bucket_capacity_(table.bucket_capacity_),
    data_(),
    sizes_(),
    overflow_bucket_(),
    hasher_(table.hasher_),
    equaler_(table.equaler_)
  {
    sizes_ = new size_t[bucket_count_];
    data_ = new Node[bucket_capacity_ * bucket_count_];
    for (size_t i = 0; i < bucket_count_; ++i) {
      sizes_[i] = table.sizes_[i];
      for (size_t j = 0; j < sizes_[i]; ++j) {
        data_[i * bucket_capacity_ + j] = table.data_[i * bucket_capacity_ + j];
      }
    }
    overflow_bucket_ = table.overflow_bucket_;

  }
  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& table) noexcept:
    bucket_count_(table.bucket_count_),
    bucket_capacity_(table.bucket_capacity_),
    data_(table.data_),
    sizes_(table.sizes_),
    overflow_bucket_(table.overflow_bucket_),
    hasher_(table.hasher_),
    equaler_(table.equaler_)
  {
    table.bucket_count_ = 0;
    table.bucket_capacity_ = 0;
    table.data_ = nullptr;
    table.sizes_ = nullptr;
  }
  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& rhs)
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    Table cpy(rhs);
    swap(cpy);
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& rhs) noexcept
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    Table cpy(std::move(rhs));
    swap(cpy);
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](Key k) noexcept
  {
    Node* el = find_el(k);
    if (el) {
      return el->val_;
    } else {
      add(k, Value{});
      return find_el(k)->val_;
    }
  }
  template < class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::operator[](Key id) const noexcept
  {
    return at(id);
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(Key id)
  {
    Node* el = find_el(id);
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("HashTable: index out of range");
    }
  }
  template < class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(Key id) const
  {
    const Node* el = find_el(id);
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("HashTable: index out of range");
    }
  }

  template < class Key, class Value, class Hash, class Equal >
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
  template < class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    Iter it(0, 0, overflow_bucket_.begin(), this);

    while (it.bucket_index_ < bucket_count_ && it.element_index_ < bucket_capacity_ &&
           !data_[it.bucket_index_ * bucket_capacity_ + it.element_index_].is_val_) {
      ++it;
      if (it.is_in_overflow())
        break;
    }

    if (it.is_in_overflow() && it.overflow_el_ == overflow_bucket_.end()) {
      return end();
    }

    return it;
  }
  template < class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    CIter it(0, 0, overflow_bucket_.begin(), this);

    while (it.bucket_index_ < bucket_count_ && it.element_index_ < bucket_capacity_ &&
           !data_[it.bucket_index_ * bucket_capacity_ + it.element_index_].is_val_) {
      ++it;
      if (it.is_in_overflow())
        break;
    }

    if (it.is_in_overflow() && it.overflow_el_ == overflow_bucket_.end()) {
      return cend();
    }

    return it;
  }
  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return CIter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin() const
  {
    CIter it(0, 0, overflow_bucket_.begin(), this);

    while (it.bucket_index_ < bucket_count_ && it.element_index_ < bucket_capacity_ &&
           !data_[it.bucket_index_ * bucket_capacity_ + it.element_index_].is_val_) {
      ++it;
      if (it.is_in_overflow())
        break;
    }

    if (it.is_in_overflow() && it.overflow_el_ == overflow_bucket_.end()) {
      return cend();
    }

    return it;
  }
  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end() const
  {
    return CIter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
  {
    Table tmp(*this);
    size_t buc_idx = getBucketIndex(k);
    size_t is_over = true;
    for (size_t i = 0; i < bucket_capacity_; ++i) {
      if (!tmp.data_[bucket_capacity_ * buc_idx + i].is_val_) {
        tmp.data_[bucket_capacity_ * buc_idx + i] = Node(k, v, true);
        tmp.sizes_[buc_idx]++;
        is_over = false;
        break;
      }
    }
    if (is_over) {
      for (auto it = tmp.overflow_bucket_.begin(); it != tmp.overflow_bucket_.end(); ++it) {
        if ((*it).is_val_ && equaler_((*it).key_, k)) {
          (*it).val_ = v;
          is_over = false;
          break;
        }
      }
    }
    if (is_over) {
      tmp.overflow_bucket_.push_back(Node(k, v, true));
    }
    swap(tmp);
  }
  template < class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(Key k)
  {
    Table tmp(*this);
    Value val;
    size_t buc_idx = getBucketIndex(k);

    auto prev = overflow_bucket_.before_begin();

    for (auto it = tmp.overflow_bucket_.begin(); it != tmp.overflow_bucket_.end(); ++it, ++prev) {
      if (equaler_(k, (*it).key_) && (*it).is_val_) {
        Value val = (*it).val_;

        tmp.overflow_bucket_.erase_after(prev);
        swap(tmp);
        return val;
      }
    }

    for (size_t i = 0; i < tmp.bucket_capacity_; ++i) {
      size_t idx = tmp.bucket_capacity_ * buc_idx + i;
      if (equaler_(tmp.data_[idx].key_, k) && tmp.data_[idx].is_val_) {

        tmp.data_[idx].is_val_ = false;
        tmp.sizes_[buc_idx]--;
        val = tmp.data_[bucket_capacity_ * buc_idx + i].val_;
        swap(tmp);
        return val;
      }
    }
    throw std::out_of_range("Key not found in drop()");
  }
  template < class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(Key k) const
  {

    size_t buc_idx = getBucketIndex(k);

    for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
      if (equaler_(k, (*it).key_) && (*it).is_val_) {
        return true;
      }
    }

    for (size_t i = 0; i < bucket_capacity_; ++i) {
      size_t idx = bucket_capacity_ * buc_idx + i;
      if (equaler_(data_[idx].key_, k) && data_[idx].is_val_) {

        return true;
      }
    }

    return false;
  }
  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    Table tmp(slots,
              bucket_capacity_,
              new Node[slots * bucket_capacity_],
              new size_t[slots],
              OverflowList(),
              hasher_,
              equaler_);
    for (size_t i = 0; i < bucket_count_; ++i) {
      for (size_t j = 0; j < bucket_capacity_; ++j) {
        size_t idx = bucket_capacity_ * i + j;
        if (data_[idx].is_val_) {
          tmp.add(data_[idx].key_, data_[idx].val_);
        }
      }
    }

    for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
      if ((*it).is_val_) {
        tmp.add((*it).key_, (*it).val_);
      }
    }
    swap(tmp);
  }

}

#endif
