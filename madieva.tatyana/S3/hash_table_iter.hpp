#ifndef HASH_TABLE_ITER_HPP
#define HASH_TABLE_ITER_HPP

#include <list.hpp>
#include "hash_table.hpp"

namespace madieva {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTIter {
  public:
    using ht = HashTable< Key, Value, Hash, Equal >;
    using pair = std::pair< Key, Value >;
    HTIter & operator++();
    HTIter operator++(int);
    pair & operator*();
    pair * operator->();
    bool operator==(const HTIter & other) const;
    bool operator!=(const HTIter & other) const;
  private:
    HTIter(size_t idx, LIter< pair > it,
      const Vector< List< pair > > * buckets);
    friend class HashTable< Key, Value, Hash, Equal >;
    size_t bucket_index_;
    LIter< pair > node_iter_;
    const Vector< List< pair > > * buckets_;
    void next();
  };

  template< class Key, class Value, class Hash, class Equal >
  void HTIter< Key, Value, Hash, Equal >::next()
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
      node_iter_ = LIter< pair >(nullptr, nullptr);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value, Hash, Equal >::HTIter(size_t idx, LIter< pair > it,
    const Vector< List< pair > > * buckets):
    bucket_index_(idx),
    node_iter_(it),
    buckets_(buckets)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value, Hash, Equal > & HTIter< Key, Value, Hash, Equal >::operator++()
  {
    next();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value, Hash, Equal > HTIter< Key, Value, Hash, Equal >::operator++(int)
  {
    HTIter old = *this;
    next(); 
    return old;
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value, Hash, Equal >::pair & HTIter< Key, Value, Hash, Equal >::operator*()
  {
    return *node_iter_;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HTIter< Key, Value, Hash, Equal >::pair *
  HTIter< Key, Value, Hash, Equal >::operator->()
  {
    return &(*node_iter_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HTIter< Key, Value, Hash, Equal >::operator==(const HTIter& other) const
  {
    return bucket_index_ == other.bucket_index_ &&
      node_iter_ == other.node_iter_ &&
      buckets_ == other.buckets_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HTIter< Key, Value, Hash, Equal >::operator!=(const HTIter & other) const
  {
    return !(*this == other);
  }
}

#endif
