#ifndef CUCKOO_CONST_ITERATOR_HPP
#define CUCKOO_CONST_ITERATOR_HPP

#include <utility>
#include <cstddef>
#include "cuckoo_hash_table.hpp"

namespace malashenko
{
  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  struct CuckooHashTable;

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class ConstIterator {
  public:
    using cht_t = CuckooHashTable< Key, Value, Hash1, Hash2, Equal >;
    using chtCIter_t = ConstIterator< Key, Value, Hash1, Hash2, Equal >;

    chtCIter_t& operator++();
    chtCIter_t operator++(int);

    const detail::HashTableNode< Key, Value >& operator*();
    const detail::HashTableNode< Key, Value >* operator->();

    bool operator!=(const chtCIter_t& other) const;
    bool operator==(const chtCIter_t& other) const;
  private:
    friend struct CuckooHashTable< Key, Value, Hash1, Hash2, Equal >;

    const cht_t* hashTable_;
    size_t tableNum_;
    size_t pos_;

    ConstIterator(const cht_t* hashTable, size_t tableNum, size_t pos);
    void skipEmpty();
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  ConstIterator< Key, Value, Hash1, Hash2, Equal >::ConstIterator(const cht_t* hashTable, size_t tableNum, size_t pos):
    hashTable_(hashTable),
    tableNum_(tableNum),
    pos_(pos)
  {}

  template<class Key, class Value, class Hash1, class Hash2, class Equal>
  void ConstIterator<Key, Value, Hash1, Hash2, Equal>::skipEmpty()
  {
    while (tableNum_ < 2)
    {
      while (pos_ < hashTable_->capacity_ && !hashTable_->table_[tableNum_][pos_].isOccupied)
      {
        ++pos_;
      }

      if (pos_ < hashTable_->capacity_)
      {
        return;
      }

      ++tableNum_;
      pos_ = 0;
    }

    tableNum_ = 1;
    pos_ = hashTable_->capacity_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  ConstIterator< Key, Value, Hash1, Hash2, Equal >& ConstIterator< Key, Value, Hash1, Hash2, Equal >::operator++()
  {
    ++pos_;
    skipEmpty();

    return *this;
  }


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  ConstIterator< Key, Value, Hash1, Hash2, Equal > ConstIterator< Key, Value, Hash1, Hash2, Equal >::operator++(int)
  {

    ConstIterator< Key, Value, Hash1, Hash2, Equal > tmp(*this);
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const detail::HashTableNode< Key, Value >& ConstIterator< Key, Value, Hash1, Hash2, Equal >::operator*()
  {
    return hashTable_->table_[tableNum_][pos_];
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const detail::HashTableNode< Key, Value >* ConstIterator< Key, Value, Hash1, Hash2, Equal >::operator->()
  {
    return std::addressof(hashTable_->table_[tableNum_][pos_]);
  }


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool ConstIterator< Key, Value, Hash1, Hash2, Equal >::operator==(const chtCIter_t& other) const
  {
    return (hashTable_ == other.hashTable_ && tableNum_ == other.tableNum_ && pos_ == other.pos_);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool ConstIterator< Key, Value, Hash1, Hash2, Equal >::operator!=(const chtCIter_t& other) const
  {
    return !(*this == other);
  }
}

#endif