#ifndef MY_CITER_HPP
#define MY_CITER_HPP

#include "../common/c-iter.hpp"
#include "my_node_hashtable.hpp"
#include <iostream>
namespace zubarev
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class CIterHashTable
  {
    friend class HashTable< Key, Value, Hash, Equal >;

  private:
    using Node = NodeHashTable< Key, Value >;
    using OverflowCIter = LCIter< Node >;
    using Table = HashTable< Key, Value, Hash, Equal >;

    size_t element_index_;
    size_t bucket_index_;
    OverflowCIter overflow_el_;
    const Table* table_;

    bool is_in_overflow() const noexcept
    {
      return bucket_index_ >= table_->bucket_count_;
    }

  public:
    CIterHashTable() = default;
    CIterHashTable(size_t, size_t, OverflowCIter, const Table*);

    const Node& operator*() const;
    const Node* operator->() const noexcept;
    CIterHashTable& operator++();
    bool operator!=(const CIterHashTable&) const;
    bool operator==(const CIterHashTable&) const;
  };

  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal >::CIterHashTable(size_t el_idx,
                                                            size_t buc_idx,
                                                            OverflowCIter over_it,
                                                            const Table* table):
    element_index_(el_idx),
    bucket_index_(buc_idx),
    overflow_el_(over_it),
    table_(table)
  {}

  template < class Key, class Value, class Hash, class Equal >
  const NodeHashTable< Key, Value >& CIterHashTable< Key, Value, Hash, Equal >::operator*() const
  {
    if (is_in_overflow()) {
      if (overflow_el_ != table_->overflow_bucket_.end()) {
        return *overflow_el_;
      } else {
        throw std::out_of_range("Dereferencing end() iterator");
      }
    } else {
      size_t idx = bucket_index_ * table_->bucket_capacity_ + element_index_;
      return table_->data_[idx];
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  const NodeHashTable< Key, Value >* CIterHashTable< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return &operator*();
  }

  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal >& CIterHashTable< Key, Value, Hash, Equal >::operator++()
  {
    if (is_in_overflow()) {
      if (overflow_el_ != table_->overflow_bucket_.end()) {
        ++overflow_el_;
      }
    } else {
      do {
        element_index_++;
        if (element_index_ >= table_->bucket_capacity_) {
          bucket_index_++;
          element_index_ = 0;
          while (bucket_index_ < table_->bucket_count_ && table_->sizes_[bucket_index_] == 0) {
            bucket_index_++;
            element_index_ = 0;
          }
          if (bucket_index_ >= table_->bucket_count_) {
            if (!table_->overflow_bucket_.empty()) {
              overflow_el_ = table_->overflow_bucket_.begin();
            } else {
              bucket_index_ = table_->bucket_count_ + 1;
            }
            return *this;
          }
        }
      } while (!table_->data_[bucket_index_ * table_->bucket_capacity_ + element_index_].is_val_);
    }
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  bool CIterHashTable< Key, Value, Hash, Equal >::operator!=(const CIterHashTable& rhs) const
  {
    return !(*this == rhs);
  }

  template < class Key, class Value, class Hash, class Equal >
  bool CIterHashTable< Key, Value, Hash, Equal >::operator==(const CIterHashTable& rhs) const
  {
    if (table_ != rhs.table_) {
      return false;
    }
    if (is_in_overflow() != rhs.is_in_overflow()) {
      return false;
    }
    if (is_in_overflow()) {
      return overflow_el_ == rhs.overflow_el_;
    }
    return bucket_index_ == rhs.bucket_index_ && element_index_ == rhs.element_index_;
  }

}

#endif
