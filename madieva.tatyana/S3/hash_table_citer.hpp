#ifndef HASH_TABLE_CITER_HPP
#define HASH_TABLE_CITER_HPP

#include <list.hpp>
#include "hash_table.hpp"

namespace madieva
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTCIter
  {
  public:
    using ht = HashTable< Key, Value, Hash, Equal >;
    using pair = std::pair< Key, Value >;
    HTCIter & operator++();
    HTCIter operator++(int);
    const pair & operator*() const;
    const pair* operator->() const;
    bool operator==(const HTCIter & other) const;
    bool operator!=(const HTCIter & other) const;
  private:
    HTCIter(size_t idx, LCIter< pair > it,
      const Vector< List< pair > > * buckets);
    friend class HashTable< Key, Value, Hash, Equal >;
    size_t bucket_index_;
    LCIter< pair > node_iter_;
    const Vector< List< pair > >* buckets_;
    void next();
  };

  template< class Key, class Value, class Hash, class Equal >
  void HTCIter< Key, Value, Hash, Equal >::next()
  {
    ++node_iter_;
    if (node_iter_ == (*buckets_)[bucket_index_].begin()) {
      ++bucket_index_;
      while (bucket_index_ < buckets_->getSize()) {
        if ((*buckets_)[bucket_index_].size() > 0) {
          node_iter_ = (*buckets_)[bucket_index_].begin();
          return;
        }
        ++bucket_index_;
      }
      node_iter_ = LCIter< pair >(nullptr, nullptr);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value, Hash, Equal >::HTCIter(size_t idx, LCIter< pair > it,
    const Vector< List< pair > >* buckets):
    bucket_index_(idx),
    node_iter_(it),
    buckets_(buckets)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value, Hash, Equal > & HTCIter< Key, Value, Hash, Equal >::operator++()
  {
    next();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value, Hash, Equal > HTCIter< Key, Value, Hash, Equal >::operator++(int)
  {
    HTCIter old = *this;
    next();
    return old;
  }

  template< class Key, class Value, class Hash, class Equal >
  const HTCIter< Key, Value, Hash, Equal >::pair &
    HTCIter< Key, Value, Hash, Equal >::operator*() const
  {
    return *node_iter_;
  }

  template< class Key, class Value, class Hash, class Equal >
  const typename HTCIter< Key, Value, Hash, Equal >::pair *
    HTCIter< Key, Value, Hash, Equal >::operator->() const
  {
    return &(*node_iter_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HTCIter< Key, Value, Hash, Equal >::operator==(const HTCIter & other) const
  {
    return bucket_index_ == other.bucket_index_ &&
      node_iter_ == other.node_iter_ &&
      buckets_ == other.buckets_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HTCIter< Key, Value, Hash, Equal >::operator!=(const HTCIter & other) const
  {
    return !(*this == other);
  }
}

#endif
