#ifndef HTITER_HPP
#define HTITER_HPP

#include <cstddef>
#include <utility>

namespace chernov {
  template< bool B, class T, class F >
  struct conditional {
    using type = T;
  };

  template< class T, class F >
  struct conditional< false, T, F > {
    using type = F;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal, bool IsConst >
  class HTIter {
  public:
    using pair_type = std::pair< const Key, Value >;
    using reference = typename conditional< IsConst, const pair_type &, pair_type & >::type;
    using point = typename conditional< IsConst, const pair_type *, pair_type * >::type;

    HTIter();
    HTIter(const HashTable< Key, Value, Hash, Equal > * ht, size_t index);

    reference operator*() const;
    point operator->() const;

    HTIter & operator++();
    HTIter operator++(int);

    template< bool OtherConst >
    bool operator==(const HTIter< Key, Value, Hash, Equal, OtherConst > & other) const noexcept;

    template< bool OtherConst >
    bool operator!=(const HTIter< Key, Value, Hash, Equal, OtherConst > & other) const noexcept;
  private:
    const HashTable< Key, Value, Hash, Equal > * ht_;
    size_t index_;
  };
}

template< class Key, class Value, class Hash, class Equal, bool IsConst >
chernov::HTIter< Key, Value, Hash, Equal, IsConst >::HTIter():
  ht_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal, bool IsConst >
chernov::HTIter< Key, Value, Hash, Equal, IsConst >::
HTIter(const HashTable< Key, Value, Hash, Equal > * ht, size_t index):
  ht_(ht),
  index_(index)
{}

template< class Key, class Value, class Hash, class Equal, bool IsConst >
typename chernov::HTIter< Key, Value, Hash, Equal, IsConst>::reference
chernov::HTIter< Key, Value, Hash, Equal, IsConst >::operator*() const
{
  return ht_->data_[index_];
}

template< class Key, class Value, class Hash, class Equal, bool IsConst >
typename chernov::HTIter< Key, Value, Hash, Equal, IsConst>::point
chernov::HTIter< Key, Value, Hash, Equal, IsConst >::operator->() const
{
  return ht_->data_ + index_;
}

template< class Key, class Value, class Hash, class Equal, bool IsConst >
chernov::HTIter< Key, Value, Hash, Equal, IsConst > & chernov::HTIter< Key, Value, Hash, Equal, IsConst >::operator++()
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
    if (index_ + 1 < ht_->num_buckets_ * ht_->bucket_cap_ + ht_->overflow_size_) {
      ++index_;
      return *this;
    }
  }
  index_ = ht_->num_buckets_ * ht_->bucket_cap_ + ht_->overflow_cap_;
  return *this;
}

template< class Key, class Value, class Hash, class Equal, bool IsConst >
chernov::HTIter< Key, Value, Hash, Equal, IsConst >
chernov::HTIter< Key, Value, Hash, Equal, IsConst >::operator++(int)
{
  HTIter old = *this;
  ++(*this);
  return old;
}

template< class Key, class Value, class Hash, class Equal, bool IsConst >
template< bool OtherConst >
bool chernov::HTIter< Key, Value, Hash, Equal, IsConst >::
operator==(const HTIter< Key, Value, Hash, Equal, OtherConst > & other) const noexcept
{
  return ht_ == other.ht_ && index_ == other.index_;
}


template< class Key, class Value, class Hash, class Equal, bool IsConst >
template< bool OtherConst >
bool chernov::HTIter< Key, Value, Hash, Equal, IsConst >::
operator!=(const HTIter< Key, Value, Hash, Equal, OtherConst > & other) const noexcept
{
  return !(*this == other);
}

#endif
