#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP
#include <iostream>
#include <cstddef>
#include <utility>
#include <common/vector/vector.hpp>
#include <hash_table_node.hpp>

namespace malashenko
{
  template<class Key, class Value, class Hash1, class Hash2, class Equal >
  struct CuckooHashTable {
  public:
    using cht_t = CuckooHashTable< Key, Value, Hash1, Hash2, Equal >;

    CuckooHashTable();
    CuckooHashTable(const size_t& size);

    CuckooHashTable(const cht_t& rhs);
    CuckooHashTable(cht_t&& rhs) noexcept;

    cht_t& operator=(const cht_t& rhs);
    cht_t& operator=(cht_t&& rhs) noexcept;

    ~CuckooHashTable() = default;

    void add(const Key& key, const Value& value);
    Value drop(const Key& key);

    bool has(const Key& key) const;

    Value& get(const Key& key);
    const Value& get(const Key& key) const;

    Value& operator[](const Key& key);

    // htIter_t begin();
    // htIter_t end();
    // htConstIter_t cbegin() const;
    // htConstIter_t cend() const;

    void rehash(const size_t& newSize);

    size_t size() const noexcept;

    void swap(cht_t& rhs);

  private:
    using vec_t = Vector< detail::HashTableNode< Key, Value > >;
    Vector< detail::HashTableNode< Key, Value > > table_[2];

    size_t size_;
    size_t capacity_;
    size_t max_steps_ = 16;

    Hash1 hasher1_;
    Hash2 hasher2_;

    bool insertInTable(const Key& key, const Value& value, size_t tableInd, size_t cnt);
    std::pair< size_t, size_t> hasInTable(const Key& key) const noexcept;
  };
}


template<class Key, class Value, class Hash1, class Hash2, class Equal >
malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable():
  table_(),
  size_(0),
  capacity_(8),
  hasher1_(),
  hasher2_()
{}

template<class Key, class Value, class Hash1, class Hash2, class Equal >
malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(const size_t& size):
  table_(),
  size_(0),
  capacity_(8),
  hasher1_(),
  hasher2_()
{}

template<class Key, class Value, class Hash1, class Hash2, class Equal >
malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(const cht_t& rhs):
  table_(rhs.table_),
  size_(rhs.size_),
  capacity_(rhs.capacity_),
  max_steps_(rhs.max_steps_),
  hasher1_(rhs.hasher1_),
  hasher2_(rhs.hasher2_)
{}

template<class Key, class Value, class Hash1, class Hash2, class Equal >
malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(cht_t&& rhs) noexcept:
  table_(std::move(rhs.table_)),
  size_(std::exchange(rhs.size_, 0)),
  capacity_(std::exchange(rhs.size_, 0)),
  max_steps_(std::exchange(rhs.max_steps_, 0)),
  hasher1_(std::move(rhs.hasher1_)),
  hasher2_(std::move(rhs.hasher2_))
{}

template<class Key, class Value, class Hash1, class Hash2, class Equal >
malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >& malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(const cht_t& rhs)
{
  assert (this == std::addressof(rhs));
  cht_t tmp(rhs);
  swap(tmp);
  return *this;
}

template<class Key, class Value, class Hash1, class Hash2, class Equal >
malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >& malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(cht_t&& rhs) noexcept
{
  assert (this == std::addressof(rhs));
  cht_t tmp(std::move(rhs));
  swap(tmp);
  return *this;
}

template<class Key, class Value, class Hash1, class Hash2, class Equal >
std::pair< size_t, size_t> malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::hasInTable(const Key& key) const noexcept
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


template<class Key, class Value, class Hash1, class Hash2, class Equal >
bool malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insertInTable(const Key& key, const Value& value, size_t tableInd, size_t cnt)
{
  if (cnt >= max_steps_)
  {
    return false;
  }

  std::pair< size_t, size_t > keyInTableIndex = hasInTable(key);

  if (keyInTableIndex.first != 2)
  {
    table_[keyInTableIndex.first][keyInTableIndex.second].value = value;
    return true;
  }

  detail::HashTableNode< Key, Value > insertNode = {true, key, value};
  size_t hashedKey = (tableInd == 0) ? hasher1_(key) : hasher2_(key);
  size_t pos = hashedKey % capacity_;
  if (!table_[tableInd][pos].isOccupied)
  {
    table_[tableInd][pos] = insertNode;
    return true;
  }
  else
  {
    Key tmpKey = table_[tableInd][pos].key;
    Value tmpValue = table_[tableInd][pos].value;
    table_[tableInd][pos] = insertNode;
    return insertInTable(tmpKey, tmpValue, 1 - tableInd, cnt + 1);
  }
}

template<class Key, class Value, class Hash1, class Hash2, class Equal >
void malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::add(const Key& key, const Value& value)
{
  // Hash1 hasher1;
  // detail::HashTableNode< Key, Value > insertNode = {true, key, value};
  // size_t ind1 = hasher1(key) % capacity_;
  // if (!table1_[ind1].isOccupied)
  // {
  //   table1_[ind1] = insertNode;
  //   return;
  // }
  // else
  // {
  //   detail::HashTableNode< Key, Value > tmpNode = table1_[ind1];
  //   table1_[ind1] = insertNode;
  //   Hash2 hasher2;
  //   size_t counter = 0;
  //   size_t ind2 = hasher2(key) % capacity_;


  // }


}



template<class Key, class Value, class Hash1, class Hash2, class Equal >
void malashenko::CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::swap(cht_t& rhs)
{
  using std::swap;
  swap(table1_, rhs.table1_);
  swap(table2_, rhs.table2_);
  swap(size_, rhs.size_);
  swap(max_steps_, rhs.max_steps_);
  swap(hasher1_, rhs.hasher1_);
  swap(hasher2_, rhs.hasher2_);
}







#endif