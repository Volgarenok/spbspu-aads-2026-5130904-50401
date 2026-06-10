#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace zhuravleva
{
  template< class Key, class Value >
  class CuckooHashTable
  {
  public:
    CuckooHashTable();

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

  private:
    struct Cell
    {
      bool occupied;
      std::pair< Key, Value > data;

      Cell():
        occupied(false),
        data()
      {}
    };

    std::vector< Cell > table1_;
    std::vector< Cell > table2_;
    size_t size_;
    std::hash< Key > hash_;
  };

  template< class Key, class Value >
  CuckooHashTable< Key, Value >::CuckooHashTable():
    table1_(8), table2_(8),
    size_(0), hash_()
  {}

  template< class Key, class Value >
  bool CuckooHashTable< Key, Value >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value >
  size_t CuckooHashTable< Key, Value >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value >
  size_t CuckooHashTable< Key, Value >::capacity() const noexcept
  {
    return table1_.size() + table2_.size();
  }
}

#endif