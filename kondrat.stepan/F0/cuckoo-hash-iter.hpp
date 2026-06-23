#ifndef CUCKOO_HASH_ITER_HPP
#define CUCKOO_HASH_ITER_HPP

#include <cstddef>
#include <memory>
#include <vector/top-it-vector.hpp>
#include "cuckoo-hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class CuckooHashTable;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashIter
  {
    public:
      using value_type = typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::value_type;

      HashIter() noexcept;

      value_type & operator*() noexcept;
      value_type * operator->() noexcept;

      HashIter & operator++() noexcept;
      HashIter operator++(int) noexcept;

      bool operator==(const HashIter & other) const noexcept;
      bool operator!=(const HashIter & other) const noexcept;

    private:
      friend class CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >;

      using Table = CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >;
      using Node = detail::CuckooHashNode< Key, Value >;

      Table * table_;
      size_t tableIndex_;
      size_t nodeIndex_;

      HashIter(Table * table, size_t tableIndex, size_t nodeIndex) noexcept;
      void advance() noexcept;
  };

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >::HashIter() noexcept:
    table_(nullptr),
    tableIndex_(2),
    nodeIndex_(0)
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >::HashIter(
    Table * table,
    size_t tableIndex,
    size_t nodeIndex) noexcept:
    table_(table),
    tableIndex_(tableIndex),
    nodeIndex_(nodeIndex)
  {
    advance();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename HashIter< Key, Value, PrimHash, SecHash, Equal >::value_type &
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator*() noexcept
  {
    if (tableIndex_ == 0)
    {
      return table_->firstTable_[nodeIndex_].data_;
    }
    return table_->secondTable_[nodeIndex_].data_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename HashIter< Key, Value, PrimHash, SecHash, Equal >::value_type *
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator->() noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal > &
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator++() noexcept
  {
    ++nodeIndex_;
    advance();
    return *this;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator++(int) noexcept
  {
    HashIter copy(*this);
    ++(*this);
    return copy;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashIter< Key, Value, PrimHash, SecHash, Equal >::operator==(
    const HashIter & other) const noexcept
  {
    return table_ == other.table_
      && tableIndex_ == other.tableIndex_
      && nodeIndex_ == other.nodeIndex_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashIter< Key, Value, PrimHash, SecHash, Equal >::operator!=(
    const HashIter & other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void HashIter< Key, Value, PrimHash, SecHash, Equal >::advance() noexcept
  {
    if (table_ == nullptr || tableIndex_ == 2)
    {
      return;
    }

    while (tableIndex_ < 2)
    {
      const kondrat::Vector< Node > & table =
        tableIndex_ == 0 ? table_->firstTable_ : table_->secondTable_;
      while (nodeIndex_ < table.getSize())
      {
        if (table[nodeIndex_].occupied_)
        {
          return;
        }
        ++nodeIndex_;
      }
      ++tableIndex_;
      nodeIndex_ = 0;
    }
  }
}

#endif
