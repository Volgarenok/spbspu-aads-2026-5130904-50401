#ifndef MADIEVA_HASH_TABLE_ITER_HPP
#define MADIEVA_HASH_TABLE_ITER_HPP


#include "../common/list.hpp"
#include "hash_table.hpp"

namespace madieva {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTIter {
    using ht = HashTable< Key, Value, Hash, Equal >;
    using Pair = std::pair<Key, Value>;
    size_t bucket_index_;
    LIter< Pair > node_iter_;
    const Vector< List< Pair > > * buckets_;
    void next();
  public:
    HTIter(size_t idx, LIter< Pair > it,
      const Vector< List< Pair > >* buckets);
    HTIter & operator++();
    std::pair<const Key&, Value&> operator*();
    bool operator==(const HTIter& other) const;
    bool operator!=(const HTIter& other) const;
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
      node_iter_ = LIter<Pair>(nullptr);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value, Hash, Equal >::HTIter(size_t idx, LIter< Pair > it,
    const Vector< List< Pair > > * buckets) :
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

  template<class Key, class Value, class Hash, class Equal>
  std::pair<const Key&, Value&> HTIter<Key, Value, Hash, Equal>::operator*()
  {
    Pair & pair = *node_iter_;
    return {pair.first, pair.second};
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HTIter< Key, Value, Hash, Equal >::operator==(const HTIter& other) const
  {
    return bucket_index_ == other.bucket_index_ &&
      node_iter_ == other.node_iter_ &&
      buckets_ == other.buckets_;
  }

  template<class Key, class Value, class Hash, class Equal>
  bool HTIter<Key, Value, Hash, Equal>::operator!=(const HTIter& other) const
  {
    return !(*this == other);
  }
}


#endif
