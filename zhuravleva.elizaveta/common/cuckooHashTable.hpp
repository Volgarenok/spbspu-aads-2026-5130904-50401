#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <myVector.hpp>

namespace zhuravleva
{
  template< class Key >
  struct SecondHash
  {
    size_t operator()(const Key& key) const
    {
      std::hash< Key > hash;
      size_t value = hash(key);
      return value ^ (value >> 16);
    }
  };

  template< class Key, class Value,
    class Hash1 = std::hash< Key >,
    class Hash2 = SecondHash< Key >,
    class Equal = std::equal_to< Key > >
  class CuckooHashTable
  {
  public:
    CuckooHashTable();

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    void clear();
    void erase(const Key& key);
    void swap(CuckooHashTable& other) noexcept;
    bool contains(const Key& key) const;
    Value& get(const Key& key);
    const Value& get(const Key& key) const;
    void insert(const Key& key, const Value& value);

    class Iterator;
    class ConstIterator;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    class Iterator
    {
    public:
      Iterator();
      std::pair< Key, Value >& operator*() const;
      std::pair< Key, Value >* operator->() const;
      Iterator& operator++();
      Iterator operator++(int);
      bool operator==(const Iterator& other) const noexcept;
      bool operator!=(const Iterator& other) const noexcept;

    private:
      friend class CuckooHashTable< Key, Value, Hash1, Hash2, Equal >;
      CuckooHashTable* table_;
      bool firstTable_;
      size_t index_;
      Iterator(CuckooHashTable* table, bool firstTable, size_t index);
      void skipEmpty();
    };

    class ConstIterator
    {
    public:
      ConstIterator();
      const std::pair< Key, Value >& operator*() const;
      const std::pair< Key, Value >* operator->() const;
      ConstIterator& operator++();
      ConstIterator operator++(int);
      bool operator==(const ConstIterator& other) const noexcept;
      bool operator!=(const ConstIterator& other) const noexcept;

    private:
      friend class CuckooHashTable< Key, Value, Hash1, Hash2, Equal >;
      const CuckooHashTable* table_;
      bool firstTable_;
      size_t index_;
      ConstIterator(const CuckooHashTable* table,
          bool firstTable, size_t index);
      void skipEmpty();
    };

  private:
    static const size_t maxShiftCount_ = 32;

    struct Cell
    {
      bool occupied;
      std::pair< Key, Value > data;

      Cell():
        occupied(false),
        data()
      {}
    };

    myVector< Cell > table1_;
    myVector< Cell > table2_;
    size_t size_;
    Hash1 hash1_;
    Hash2 hash2_;
    Equal equal_;
    size_t getIndex1(const Key& key) const;
    size_t getIndex2(const Key& key) const;
    bool updateIfExists(const Key& key, const Value& value);
    bool insertToEmpty(const Key& key, const Value& value);
    bool insertWithDisplacement(const Key& key, const Value& value);
    void rehash(size_t newCapacity);
    bool insertInternal(const Key& key, const Value& value);
    void moveElementsTo(CuckooHashTable& dest) const;
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable():
    table1_(8), table2_(8),
    size_(0), hash1_(), hash2_(), equal_()
  {}

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::capacity() const noexcept
  {
    return table1_.size() + table2_.size();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::clear()
  {
    for (size_t i = 0; i < table1_.size(); ++i)
    {
      table1_[i].occupied = false;
    }
    for (size_t i = 0; i < table2_.size(); ++i)
    {
      table2_[i].occupied = false;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::erase(const Key& key)
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      table1_[index1].occupied = false;
      --size_;
      return;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      table2_[index2].occupied = false;
      --size_;
      return;
    }
    throw std::out_of_range("key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::swap(CuckooHashTable& other) noexcept
  {
    table1_.swap(other.table1_);
    table2_.swap(other.table2_);
    std::swap(size_, other.size_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::getIndex1(const Key& key) const
  {
    return hash1_(key) % table1_.size();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::getIndex2(const Key& key) const
  {
    return hash2_(key) % table2_.size();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::contains(const Key& key) const
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      return true;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      return true;
    }

    return false;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key& key)
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      return table1_[index1].data.second;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      return table2_[index2].data.second;
    }
    throw std::out_of_range("key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key& key) const
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      return table1_[index1].data.second;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      return table2_[index2].data.second;
    }
    throw std::out_of_range("key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::updateIfExists(
      const Key& key, const Value& value)
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      table1_[index1].data.second = value;
      return true;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      table2_[index2].data.second = value;
      return true;
    }
    return false;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insertToEmpty(
      const Key& key, const Value& value)
  {
    size_t index1 = getIndex1(key);
    if (!table1_[index1].occupied)
    {
      table1_[index1].data = std::make_pair(key, value);
      table1_[index1].occupied = true;
      ++size_;
      return true;
    }
    size_t index2 = getIndex2(key);
    if (!table2_[index2].occupied)
    {
      table2_[index2].data = std::make_pair(key, value);
      table2_[index2].occupied = true;
      ++size_;
      return true;
    }
    return false;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insertWithDisplacement(
      const Key& key, const Value& value)
  {
    std::pair< Key, Value > current(key, value);
    bool inFirstTable = true;
    for (size_t i = 0; i < maxShiftCount_; ++i)
    {
      if (inFirstTable)
      {
        size_t index = getIndex1(current.first);
        std::swap(current, table1_[index].data);
        table1_[index].occupied = true;
        inFirstTable = false;
        size_t index2 = getIndex2(current.first);
        if (!table2_[index2].occupied)
        {
          table2_[index2].data = current;
          table2_[index2].occupied = true;
          ++size_;
          return true;
        }
      }
      else
      {
        size_t index = getIndex2(current.first);
        std::swap(current, table2_[index].data);
        table2_[index].occupied = true;
        inFirstTable = true;
        size_t index1 = getIndex1(current.first);
        if (!table1_[index1].occupied)
        {
          table1_[index1].data = current;
          table1_[index1].occupied = true;
          ++size_;
          return true;
        }
      }
    }
    return false;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insert(
      const Key& key, const Value& value)
  {
    CuckooHashTable temp(*this);
    while (!temp.insertInternal(key, value))
    {
      temp.rehash(temp.table1_.size() * 2);
    }
    swap(temp);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::rehash(size_t newCapacity)
  {
    CuckooHashTable temp;
    temp.table1_ = myVector< Cell >(newCapacity);
    temp.table2_ = myVector< Cell >(newCapacity);
    temp.size_ = 0;
    moveElementsTo(temp);
    swap(temp);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insertInternal(
      const Key& key, const Value& value)
  {
    if (updateIfExists(key, value))
    {
      return true;
    }
    if (insertToEmpty(key, value))
    {
      return true;
    }
    return insertWithDisplacement(key, value);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::moveElementsTo(
      CuckooHashTable& dest) const
  {
    for (size_t i = 0; i < table1_.size(); ++i)
    {
      if (table1_[i].occupied)
      {
        while (!dest.insertInternal(table1_[i].data.first, table1_[i].data.second))
        {
          dest.rehash(dest.table1_.size() * 2);
        }
      }
    }

    for (size_t i = 0; i < table2_.size(); ++i)
    {
      if (table2_[i].occupied)
      {
        while (!dest.insertInternal(table2_[i].data.first, table2_[i].data.second))
        {
          dest.rehash(dest.table1_.size() * 2);
        }
      }
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::Iterator():
    table_(nullptr),
    firstTable_(true),
    index_(0)
  {}

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::Iterator(
      CuckooHashTable* table, bool firstTable, size_t index):
    table_(table),
    firstTable_(firstTable),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::skipEmpty()
  {
    if (!table_)
    {
      return;
    }
    while (firstTable_ && index_ < table_->table1_.size()
        && !table_->table1_[index_].occupied)
    {
      ++index_;
    }
    if (firstTable_ && index_ == table_->table1_.size())
    {
      firstTable_ = false;
      index_ = 0;
    }
    while (!firstTable_ && index_ < table_->table2_.size()
        && !table_->table2_[index_].occupied)
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  std::pair< Key, Value >&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::operator*() const
  {
    if (!table_)
    {
      throw std::runtime_error("invalid iterator");
    }
    if (firstTable_)
    {
      if (index_ >= table_->table1_.size())
      {
        throw std::runtime_error("invalid iterator");
      }
      return table_->table1_[index_].data;
    }
    if (index_ >= table_->table2_.size())
    {
      throw std::runtime_error("invalid iterator");
    }
    return table_->table2_[index_].data;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  std::pair< Key, Value >*
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::operator->() const
  {
    return &(**this);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::operator++()
  {
    if (!table_)
    {
      throw std::runtime_error("invalid iterator");
    }
    ++index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::operator++(int)
  {
    Iterator result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::operator==(
      const Iterator& other) const noexcept
  {
    return table_ == other.table_
        && firstTable_ == other.firstTable_
        && index_ == other.index_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator::operator!=(
      const Iterator& other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::ConstIterator():
    table_(nullptr),
    firstTable_(true),
    index_(0)
  {}

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::ConstIterator(
      const CuckooHashTable* table, bool firstTable, size_t index):
    table_(table),
    firstTable_(firstTable),
    index_(index)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::skipEmpty()
  {
    if (!table_)
    {
      return;
    }
    while (firstTable_ && index_ < table_->table1_.size()
        && !table_->table1_[index_].occupied)
    {
      ++index_;
    }
    if (firstTable_ && index_ == table_->table1_.size())
    {
      firstTable_ = false;
      index_ = 0;
    }
    while (!firstTable_ && index_ < table_->table2_.size()
        && !table_->table2_[index_].occupied)
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const std::pair< Key, Value >&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::operator*() const
  {
    if (!table_)
    {
      throw std::runtime_error("invalid iterator");
    }
    if (firstTable_)
    {
      if (index_ >= table_->table1_.size())
      {
        throw std::runtime_error("invalid iterator");
      }
      return table_->table1_[index_].data;
    }
    if (index_ >= table_->table2_.size())
    {
      throw std::runtime_error("invalid iterator");
    }
    return table_->table2_[index_].data;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const std::pair< Key, Value >*
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::operator->() const
  {
    return &(**this);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::operator++()
  {
    if (!table_)
    {
      throw std::runtime_error("invalid iterator");
    }
    ++index_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::operator++(int)
  {
    ConstIterator result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::operator==(
      const ConstIterator& other) const noexcept
  {
    return table_ == other.table_
        && firstTable_ == other.firstTable_
        && index_ == other.index_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator::operator!=(
      const ConstIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::begin()
  {
    return Iterator(this, true, 0);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Iterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::end()
  {
    return Iterator(this, false, table2_.size());
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::begin() const
  {
    return ConstIterator(this, true, 0);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::end() const
  {
    return ConstIterator(this, false, table2_.size());
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::cbegin() const
  {
    return ConstIterator(this, true, 0);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::ConstIterator
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::cend() const
  {
    return ConstIterator(this, false, table2_.size());
  }
}

#endif
