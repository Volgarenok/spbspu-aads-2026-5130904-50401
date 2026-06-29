#ifndef MY_ITER_HPP
#define MY_ITER_HPP

#include <stdexcept>
#include "../common/iter.hpp"
#include "my_node_hashtable.hpp"

namespace zubarev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;
  template< class Key, class Value, class Hash, class Equal >
  class CIterHashTable;

  template< class Key, class Value, class Hash, class Equal >
  class IterHashTable
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class CIterHashTable< Key, Value, Hash, Equal >;

  public:
    IterHashTable() = default;
    IterHashTable(size_t, size_t, LIter< detail::NodeHashTable< Key, Value > >, HashTable< Key, Value, Hash, Equal >*);

    detail::NodeHashTable< Key, Value >& operator*();
    detail::NodeHashTable< Key, Value >* operator->() noexcept;
    IterHashTable& operator++();
    IterHashTable operator++(int);
    bool operator!=(const IterHashTable&) const;
    bool operator==(const IterHashTable&) const;

  private:
    size_t element_index_;
    size_t bucket_index_;
    LIter< detail::NodeHashTable< Key, Value > > overflow_el_;
    HashTable< Key, Value, Hash, Equal >* table_;

    bool is_in_overflow() const noexcept;
  };
  template< class Key, class Value, class Hash, class Equal >
  bool IterHashTable< Key, Value, Hash, Equal >::is_in_overflow() const noexcept
  {
    return bucket_index_ >= table_->bucket_count_;
  }

  template< class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal >::IterHashTable(size_t el_idx,
                                                          size_t buc_idx,
                                                          LIter< detail::NodeHashTable< Key, Value > > it,
                                                          HashTable< Key, Value, Hash, Equal >* table):
    element_index_(el_idx),
    bucket_index_(buc_idx),
    overflow_el_(it),
    table_(table)
  {}

  template< class Key, class Value, class Hash, class Equal >
  detail::NodeHashTable< Key, Value >& IterHashTable< Key, Value, Hash, Equal >::operator*()
  {

    if (is_in_overflow()) {
      if (overflow_el_ != table_->overflow_bucket_.end()) {
        if (!(*overflow_el_).is_val) {
          throw std::out_of_range("Invalid node in overflow");
        }
        return *overflow_el_;
      } else {
        throw std::out_of_range("Dereferencing end() iterator");
      }

    } else {
      size_t idx = bucket_index_ * table_->bucket_capacity_ + element_index_;

      return table_->data_[idx];
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  detail::NodeHashTable< Key, Value >* IterHashTable< Key, Value, Hash, Equal >::operator->() noexcept
  {
    return std::addressof(operator*());
  }
  template< class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal >& IterHashTable< Key, Value, Hash, Equal >::operator++()
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
      } while (!table_->data_[bucket_index_ * table_->bucket_capacity_ + element_index_].is_val);
    }
    return *this;
  }
  template< class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal > IterHashTable< Key, Value, Hash, Equal >::operator++(int)
  {
    IterHashTable< Key, Value, Hash, Equal > tmp = this;
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
      } while (!table_->data_[bucket_index_ * table_->bucket_capacity_ + element_index_].is_val);
    }
    return tmp;
  }
  template< class Key, class Value, class Hash, class Equal >
  bool IterHashTable< Key, Value, Hash, Equal >::operator!=(const IterHashTable& rhs) const
  {
    return !(*this == rhs);
  }
  template< class Key, class Value, class Hash, class Equal >
  bool IterHashTable< Key, Value, Hash, Equal >::operator==(const IterHashTable& rhs) const
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
