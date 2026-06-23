#ifndef CUCKOO_HASH_CONST_ITER_HPP
#define CUCKOO_HASH_CONST_ITER_HPP

#include <cstddef>
#include <memory>
#include <vector/top-it-vector.hpp>
#include "cuckoo-hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class CuckooHashTable;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashConstIter
  {
    public:
      using value_type = typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::value_type;

      HashConstIter() noexcept;

      const value_type & operator*() const noexcept;
      const value_type * operator->() const noexcept;

      HashConstIter & operator++() noexcept;
      HashConstIter operator++(int) noexcept;

      bool operator==(const HashConstIter & other) const noexcept;
      bool operator!=(const HashConstIter & other) const noexcept;

    private:
      friend class CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >;

      using Table = CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >;
      using Node = detail::CuckooHashNode< Key, Value >;

      const Table * table_;
      size_t tableIndex_;
      size_t nodeIndex_;

      HashConstIter(const Table * table, size_t tableIndex, size_t nodeIndex) noexcept;
      void advance() noexcept;
  };

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::HashConstIter() noexcept:
    table_(nullptr),
    tableIndex_(2),
    nodeIndex_(0)
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::HashConstIter(
    const Table * table,
    size_t tableIndex,
    size_t nodeIndex) noexcept:
    table_(table),
    tableIndex_(tableIndex),
    nodeIndex_(nodeIndex)
  {
    advance();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const typename HashConstIter< Key, Value, PrimHash, SecHash, Equal >::value_type &
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator*() const noexcept
  {
    if (tableIndex_ == 0)
    {
      return table_->firstTable_[nodeIndex_].data_;
    }
    return table_->secondTable_[nodeIndex_].data_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const typename HashConstIter< Key, Value, PrimHash, SecHash, Equal >::value_type *
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator->() const noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal > &
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator++() noexcept
  {
    ++nodeIndex_;
    advance();
    return *this;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator++(int) noexcept
  {
    HashConstIter copy(*this);
    ++(*this);
    return copy;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator==(
    const HashConstIter & other) const noexcept
  {
    return table_ == other.table_
      && tableIndex_ == other.tableIndex_
      && nodeIndex_ == other.nodeIndex_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator!=(
    const HashConstIter & other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void HashConstIter< Key, Value, PrimHash, SecHash, Equal >::advance() noexcept
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
