#ifndef HASH_TABLE_CONST_ITER
#define HASH_TABLE_CONST_ITER
#include <utility>
#include <list/list.hpp>
#include <list/iterator.hpp>
#include "hash_table.hpp"
namespace malashenko
{

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;
  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIter {
  public:
    using ht_t = HashTable< Key, Value, Hash, Equal >;
    using htIter_t = HashTableConstIter< Key, Value, Hash, Equal >;
    using lIter_t = LIter< std::pair<Key, Value> >;
    htIter_t& operator++() noexcept;
    htIter_t operator++(int) noexcept;

    const std::pair<Key, Value>& operator*() noexcept;
    const std::pair<Key, Value>* operator->() noexcept;

    bool operator!=(const htIter_t& other) const noexcept;
    bool operator==(const htIter_t& other) const noexcept;
  private:
    const ht_t* hashTable_;
    size_t ind_;
    lIter_t it_;
    friend class HashTable< Key, Value, Hash, Equal >;

    HashTableConstIter(ht_t* hashTable, size_t ind, lIter_t it) noexcept;
  };


  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIter< Key, Value, Hash, Equal >::HashTableConstIter(ht_t* hashTable, size_t ind, lIter_t it) noexcept:
    hashTable_(hashTable),
    ind_(ind),
    it_(it)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIter< Key, Value, Hash, Equal >& HashTableConstIter< Key, Value, Hash, Equal >::operator++() noexcept
  {

    if (ind_ >= hashTable_->size_)
    {
      ind_ = hashTable_->size_;
      return *this;
    }

    if (it_ == --(hashTable_->slots_[ind_]->end()))
    {
      ++ind_;
      while (ind_ < hashTable_->size_ && (!hashTable_->slots_[ind_] || hashTable_->slots_[ind_]->empty()))
      {
        ++ind_;
      }
      if (ind_ < hashTable_->size_ && hashTable_->slots_[ind_] && !hashTable_->slots_[ind_]->empty())
      {
        it_ = lIter_t(hashTable_->slots_[ind_]->begin());
      }
    } else
    {
      ++it_;
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIter< Key, Value, Hash, Equal > HashTableConstIter< Key, Value, Hash, Equal >::operator++(int) noexcept
  {
    HashTableConstIter< Key, Value, Hash, Equal > tmp(*this);
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair<Key, Value>& HashTableConstIter< Key, Value, Hash, Equal >::operator*() noexcept
  {
    return *it_;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair<Key, Value>* HashTableConstIter< Key, Value, Hash, Equal >::operator->() noexcept
  {
    return std::addressof(*it_);
  }


  template< class Key, class Value, class Hash, class Equal >
  bool HashTableConstIter< Key, Value, Hash, Equal >::operator==(const htIter_t& other) const noexcept
  {
    if (ind_ == hashTable_->size_ && other.ind_ == other.hashTable_->size_)
    {
      return true;
    }
    bool res = hashTable_ == other.hashTable_;
    res = res && ind_ == other.ind_;
    res = res && it_ == other.it_;
    return res;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTableConstIter< Key, Value, Hash, Equal >::operator!=(const htIter_t& other) const noexcept
  {
    return !(*this == other);
  }

}

#endif
