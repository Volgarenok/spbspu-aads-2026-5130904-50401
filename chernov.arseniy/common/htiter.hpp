#ifndef HTITER_HPP
#define HTITER_HPP

#include <cstddef>

namespace chernov {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTIter {
  public:
    HTIter();
    HTIter(const HashTable< Key, Value, Hash, Equal > * ht, size_t index);
    Value & operator*() const;
    Value * operator->() const;
    HTIter & operator++();
    HTIter & operator++(int);
    bool operator==(const HTIter & other) const noexcept;
    bool operator!=(const HTIter & other) const noexcept;
  private:
    const HashTable< Key, Value, Hash, Equal > * ht_;
    size_t index_;
  };
}

template< class Key, class Value, class Hash, class Equal >
chernov::HTIter< Key, Value, Hash, Equal >::HTIter():
  ht_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
chernov::HTIter< Key, Value, Hash, Equal >::HTIter(const HashTable< Key, Value, Hash, Equal > * ht, size_t index):
  ht_(ht),
  index_(index)
{}

template< class Key, class Value, class Hash, class Equal >
Value & chernov::HTIter< Key, Value, Hash, Equal >::operator*() const
{
  return ht_->data_[index_];
}

template< class Key, class Value, class Hash, class Equal >
Value * chernov::HTIter< Key, Value, Hash, Equal >::operator->() const
{
  return ht_->data_ + index_;
}

template< class Key, class Value, class Hash, class Equal >
chernov::HTIter< Key, Value, Hash, Equal > & chernov::HTIter< Key, Value, Hash, Equal >::operator++()
{
  if (index_ < ht_->num_buckets_ * ht_->bucket_cap_) {
    size_t home_bucket = index_ / ht_->bucket_cap_;
    size_t index_in_bucket = index_ % ht_->bucket_cap_;
    if (index_in_bucket + 1 < ht_->bucket_sizes_[home_bucket]) {
      ++index_;
      return *this;
    } else {
      for (size_t i = home_bucket + 1; i < ht_->num_buckets_; ++i) {
        if (ht_->bucket_sizes_[i]) {
          index_ = i * ht_->bucket_cap_;
          return *this;
        }
      }
    }
  } else {
    if (index_ + 1 < ht_->num_buckets_ * ht_->backet_cap_ + ht_->overflow_size_) {
      ++index_;
      return *this;
    }
  }
  index_ = ht_->num_buckets_ * ht_->bucket_cap_ + ht_->overflow_cap_;
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
chernov::HTIter< Key, Value, Hash, Equal > & chernov::HTIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTIter old = *this;
  ++(*this);
  return old;
}

template< class Key, class Value, class Hash, class Equal >
bool chernov::HTIter< Key, Value, Hash, Equal >::operator==(const HTIter & other) const noexcept
{
  return ht_ == other.ht_ && index_ == other.index_;
}

template< class Key, class Value, class Hash, class Equal >
bool chernov::HTIter< Key, Value, Hash, Equal >::operator!=(const HTIter & other) const noexcept
{
  return !(*this == other);
}

#endif
