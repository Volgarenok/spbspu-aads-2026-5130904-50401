#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP
#include <iostream>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector/vector.hpp>
#include "cuckoo_hash_table_node.hpp"
#include "cuckoo_iterator.hpp"
#include "cuckoo_const_iterator.hpp"


namespace malashenko
{


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class Iterator;
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class ConstIterator;
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  struct CuckooHashTable {
  public:

    using chTable_t = CuckooHashTable< Key, Value, Hash1, Hash2, Equal >;
    using iter_t = Iterator< Key, Value, Hash1, Hash2, Equal >;
    using cIter_t = ConstIterator< Key, Value, Hash1, Hash2, Equal >;


    CuckooHashTable();
    CuckooHashTable(const size_t& size);

    CuckooHashTable(const chTable_t& rhs);
    CuckooHashTable(chTable_t&& rhs) noexcept;

    chTable_t& operator=(const chTable_t& rhs);
    chTable_t& operator=(chTable_t&& rhs) noexcept;

    ~CuckooHashTable() = default;

    void insert(const Key& key, const Value& value);
    void insert(const Key& key, Value&& value);

    std::pair< Value, bool > erase(const Key& key);

    bool contains(const Key& key) const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const ;

    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    iter_t begin() noexcept;
    cIter_t begin() const noexcept;
    cIter_t cbegin() const noexcept;

    iter_t end() noexcept;
    cIter_t end() const noexcept;
    cIter_t cend() const noexcept;

    void rehash(const size_t& newSize);

    size_t size() const noexcept;
    size_t max_size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;

    void swap(chTable_t& rhs) noexcept;

  private:
    friend class Iterator< Key, Value, Hash1, Hash2, Equal >;
    friend class ConstIterator< Key, Value, Hash1, Hash2, Equal >;

    using node_t = detail::HashTableNode< Key, Value >;
    using vec_t = Vector< node_t >;
    using pair_t = std::pair< bool, node_t >;


    vec_t table_[2];

    size_t size_;
    size_t capacity_;
    size_t max_steps_;

    Hash1 hasher1_;
    Hash2 hasher2_;

    template< class V >
    pair_t insertInTable(const Key& key, V&& value);
    std::pair< size_t, size_t> hasInTable(const Key& key) const noexcept;
  };

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable():
    table_{ vec_t(8, node_t{}), vec_t(8, node_t{}) },
    size_(0),
    capacity_(8),
    max_steps_(2 * std::log2(capacity_)),
    hasher1_(),
    hasher2_()
  {}

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(const size_t& size):
    table_{ vec_t(size, node_t{}), vec_t(size, node_t{}) },
    size_(0),
    capacity_(size),
    max_steps_(2 * std::log2(capacity_)),
    hasher1_(),
    hasher2_()
  {}

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(const chTable_t& rhs):
    table_{rhs.table_[0], rhs.table_[1]},
    size_(rhs.size_),
    capacity_(rhs.capacity_),
    max_steps_(rhs.max_steps_),
    hasher1_(rhs.hasher1_),
    hasher2_(rhs.hasher2_)
  {}

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(chTable_t&& rhs) noexcept:
    table_{std::move(rhs.table_[0]), std::move(rhs.table_[1])},
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0)),
    max_steps_(std::exchange(rhs.max_steps_, 0)),
    hasher1_(std::move(rhs.hasher1_)),
    hasher2_(std::move(rhs.hasher2_))
  {}

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(const chTable_t& rhs)
  {
    assert (this != std::addressof(rhs));
    chTable_t tmp(rhs);
    swap(tmp);
    return *this;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(chTable_t&& rhs) noexcept
  {
    assert (this != std::addressof(rhs));
    chTable_t tmp(std::move(rhs));
    swap(tmp);
    return *this;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  std::pair< size_t, size_t>
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::hasInTable(const Key& key) const noexcept
  {
    Equal eq;
    size_t pos1 = hasher1_(key) % capacity_;
    if (table_[0][pos1].isOccupied && eq(table_[0][pos1].key, key))
    {
      return {0, pos1};
    }

    size_t pos2 = hasher2_(key) % capacity_;
    if (table_[1][pos2].isOccupied && eq(table_[1][pos2].key, key))
    {
      return {1, pos2};
    }

    return {2, 0};
  }


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  template< class V >
  std::pair< bool, detail::HashTableNode< Key, Value > >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insertInTable(const Key& key, V&& value)
  {
    std::pair< size_t, size_t > keyInTableIndex = hasInTable(key);

    if (keyInTableIndex.first != 2)
    {
      table_[keyInTableIndex.first][keyInTableIndex.second].value = std::forward< V >(value);
      return {true, node_t{}};
    }

    node_t cur{true, key, std::forward< V >(value)};
    size_t tableInd = 0;

    for (size_t step = 0; step < max_steps_; ++step)
    {
      size_t pos = (tableInd == 0 ? hasher1_(cur.key) : hasher2_(cur.key)) % capacity_;
      if (!table_[tableInd][pos].isOccupied)
      {
        table_[tableInd][pos] = cur;
        ++size_;
        return {true, node_t{}};
      }

      std::swap(cur, table_[tableInd][pos]);
      tableInd = 1 - tableInd;
    }

    return {false, cur};
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insert(const Key& key, const Value& value)
  {
    pair_t tmp = insertInTable(key, value);

    if (!tmp.first)
    {
      rehash(capacity_ * 2);
      insert(tmp.second.key, tmp.second.value);
    }
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insert(const Key& key, Value&& value)
  {
    pair_t tmp = insertInTable(key, std::move(value));
    if (!tmp.first)
    {
      rehash(capacity_ * 2);
      insert(tmp.second.key, tmp.second.value);
    }
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const CuckooHashTable& >(*this).at(key));
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  const Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::at(const Key& key) const
  {

    std::pair< size_t, size_t > pos = hasInTable(key);
    if (pos.first == 2)
    {
      throw std::out_of_range("Unknown key");
    }
    return table_[pos.first][pos.second].value;
  }


  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  std::pair< Value, bool > CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::erase(const Key& key)
  {
    Equal eq;
    size_t pos1 = hasher1_(key) % capacity_;
    if (table_[0][pos1].isOccupied && eq(table_[0][pos1].key, key))
    {
      Value result = table_[0][pos1].value;
      table_[0][pos1].isOccupied = false;
      --size_;
      return {result, true};
    }

    size_t pos2 = hasher2_(key) % capacity_;
    if (table_[1][pos2].isOccupied && eq(table_[1][pos2].key, key))
    {
      Value result = table_[1][pos2].value;
      table_[1][pos2].isOccupied = false;
      --size_;
      return {result, true};
    }

    return {Value(), false};
  }


  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::contains(const Key& key) const noexcept
  {
    return hasInTable(key).first != 2;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
  {
    return size_;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::capacity() const noexcept
  {
    return capacity_;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::swap(chTable_t& rhs) noexcept
  {
    using std::swap;
    swap(table_[0], rhs.table_[0]);
    swap(table_[1], rhs.table_[1]);
    swap(size_, rhs.size_);
    swap(capacity_, rhs.capacity_);
    swap(max_steps_, rhs.max_steps_);
    swap(hasher1_, rhs.hasher1_);
    swap(hasher2_, rhs.hasher2_);
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  Iterator< Key, Value, Hash1, Hash2, Equal > CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::begin() noexcept
  {
    iter_t it(this, 0, 0);
    it.skipEmpty();
    return it;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  ConstIterator< Key, Value, Hash1, Hash2, Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::begin() const noexcept
  {
    cIter_t it(this, 0, 0);
    it.skipEmpty();
    return it;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  ConstIterator< Key, Value, Hash1, Hash2, Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::cbegin() const noexcept
  {
    cIter_t it(this, 0, 0);
    it.skipEmpty();
    return it;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  Iterator< Key, Value, Hash1, Hash2, Equal > CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::end() noexcept
  {
    return {this, 1, capacity_};
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  ConstIterator< Key, Value, Hash1, Hash2, Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::end() const noexcept
  {
    return {this, 1, capacity_};
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  ConstIterator< Key, Value, Hash1, Hash2, Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::cend() const noexcept
  {
    return {this, 1, capacity_};
  }


  template<class Key, class Value, class Hash1, class Hash2, class Equal >

  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator[](const Key& key)
  {
    if (!contains(key))
    {
      insert(key, Value());
    }
    return at(key);
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator[](Key&& key)
  {
    if (!contains(std::move(key)))
    {
      insert(std::move(key), Value());
    }
    return at(std::move(key));
  }



  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::rehash(const size_t& newSize)
  {
    chTable_t newTable(newSize);
    newTable.max_steps_ = 2 * std::log2(newSize);
    for (iter_t beg = begin(); beg != end(); ++beg)
    {
      newTable.insert(beg->key, beg->value);
    }

    swap(newTable);
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::clear() noexcept
  {
    table_[0] = vec_t(capacity_, node_t{});
    table_[1] = vec_t(capacity_, node_t{});
    size_ = 0;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::max_size() const noexcept
  {
    return capacity_ * 2;
  }

}





#endif
