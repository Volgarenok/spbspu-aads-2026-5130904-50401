#ifndef MADIEVA_HASH_TABLE_CITER_HPP
#define MADIEVA_HASH_TABLE_CITER_HPP


#include "../common/list.hpp"
#include "hash_table.hpp"

namespace madieva {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTCIter {
    using ht = HashTable<Key, Value, Hash, Equal>;
    using Pair = std::pair<Key, Value>;
    size_t bucket_index_;
    LCIter<Pair> node_iter_;
    const Vector< List<Pair> >* buckets_;
    void next();
  public:
    HTCIter(size_t idx, LCIter<Pair> it,
      const Vector< List<Pair> >* buckets);
    HTCIter& operator++();
    std::pair<const Key&, const Value&> operator*() const;
    bool operator==(const HTCIter& other) const;
    bool operator!=(const HTCIter& other) const;
  };

  template<class Key, class Value, class Hash, class Equal>
  void HTCIter<Key, Value, Hash, Equal>::next()
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
      node_iter_ = LCIter<Pair>(nullptr);
    }
  }

  template<class Key, class Value, class Hash, class Equal>
  HTCIter<Key, Value, Hash, Equal>::HTCIter(size_t idx, LCIter<Pair> it,
    const Vector< List<Pair> >* buckets) :
    bucket_index_(idx),
    node_iter_(it),
    buckets_(buckets)
  {}

  template<class Key, class Value, class Hash, class Equal>
  HTCIter<Key, Value, Hash, Equal>& HTCIter<Key, Value, Hash, Equal>::operator++()
  {
    next();
    return *this;
  }

  template<class Key, class Value, class Hash, class Equal>
  std::pair<const Key&, const Value&> HTCIter<Key, Value, Hash, Equal>::operator*() const
  {
    const Pair & pair = *node_iter_;
    return {pair.first, pair.second};
  }

  template<class Key, class Value, class Hash, class Equal>
  bool HTCIter<Key, Value, Hash, Equal>::operator==(const HTCIter& other) const
  {
    return bucket_index_ == other.bucket_index_ &&
      node_iter_ == other.node_iter_ &&
      buckets_ == other.buckets_;
  }

  template<class Key, class Value, class Hash, class Equal>
  bool HTCIter<Key, Value, Hash, Equal>::operator!=(const HTCIter& other) const
  {
    return !(*this == other);
  }
}


#endif
