#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP
#include <stdexcept>
#include <utility>
#include <vector.hpp>
namespace madieva
{
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooHashTable
  {
  public:
    using Pair = std::pair< Key, Value >;

    explicit CuckooHashTable(size_t initialSize = 16, size_t maxRehashSteps = 100);
    CuckooHashTable(const CuckooHashTable & other);
    CuckooHashTable & operator=(const CuckooHashTable & other);
    ~CuckooHashTable() = default;

    template< typename K, typename V >
    void insert(K && key, V && value);
    bool contains(const Key & key) const;
    Value & get(const Key & key);
    const Value & get(const Key & key) const;
    Value & operator[](const Key & key);
    Value erase(const Key & key);
    void clear();

    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;
    void swap(CuckooHashTable & other) noexcept;
  private:
    Vector< Pair > table1_;
    Vector< Pair > table2_;
    Hash1 hash1_;
    Hash2 hash2_;
    Equal equal_;
    size_t size_;
    size_t maxRehashSteps_;

    size_t index1(const Key & key) const;
    size_t index2(const Key & key) const;
    void rehash();
    void rehash(size_t newSize);
    bool isOccupied(const Pair & pair) const;
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(size_t initialSize, size_t maxRehashSteps):
    table1_(),
    table2_(),
    hash1_(),
    hash2_(),
    equal_(),
    size_(0),
    maxRehashSteps_(maxRehashSteps)
  {
    table1_.reserve(initialSize);
    table2_.reserve(initialSize);

    for (size_t i = 0; i < initialSize; ++i) {
      table1_.pushBack(Pair());
      table2_.pushBack(Pair());
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(
    const CuckooHashTable & other):
    table1_(other.table1_),
    table2_(other.table2_),
    hash1_(other.hash1_),
    hash2_(other.hash2_),
    equal_(other.equal_),
    size_(other.size_),
    maxRehashSteps_(other.maxRehashSteps_)
  {}

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal > &
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(
    const CuckooHashTable & other)
  {
    if (this == &other) {
      return *this;
    }
    CuckooHashTable copy(other);
    swap(copy);
    return *this;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  template< typename K, typename V >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insert(K && key, V && value)
  {
    if (contains(key)) {
      size_t idx1 = index1(key);
      if (isOccupied(table1_[idx1]) && equal_(table1_[idx1].first, key)) {
        table1_[idx1].second = std::forward< V >(value);
        return;
      }

      size_t idx2 = index2(key);
      if (isOccupied(table2_[idx2]) && equal_(table2_[idx2].first, key)) {
        table2_[idx2].second = std::forward< V >(value);
        return;
      }
    }

    Pair newPair(std::forward< K >(key), std::forward< V >(value));
    size_t steps = 0;

    while (steps < maxRehashSteps_) {
      size_t idx1 = index1(newPair.first);

      if (!isOccupied(table1_[idx1])) {
        table1_[idx1] = std::move(newPair);
        ++size_;
        return;
      }

      Pair displaced = std::move(table1_[idx1]);
      table1_[idx1] = std::move(newPair);
      newPair = std::move(displaced);
      ++steps;

      size_t idx2 = index2(newPair.first);

      if (!isOccupied(table2_[idx2])) {
        table2_[idx2] = std::move(newPair);
        ++size_;
        return;
      }

      displaced = std::move(table2_[idx2]);
      table2_[idx2] = std::move(newPair);
      newPair = std::move(displaced);
      ++steps;
    }

    rehash();
    insert(std::move(newPair.first), std::move(newPair.second));
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::contains(const Key & key) const
  {
    size_t idx1 = index1(key);

    if (isOccupied(table1_[idx1]) && equal_(table1_[idx1].first, key)) {
      return true;
    }

    size_t idx2 = index2(key);

    if (isOccupied(table2_[idx2]) && equal_(table2_[idx2].first, key)) {
      return true;
    }

    return false;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value & CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key & key)
  {
    size_t idx1 = index1(key);
    if (isOccupied(table1_[idx1]) && equal_(table1_[idx1].first, key)) {
      return table1_[idx1].second;
    }
    size_t idx2 = index2(key);
    if (isOccupied(table2_[idx2]) && equal_(table2_[idx2].first, key)) {
      return table2_[idx2].second;
    }
    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const Value & CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key & key) const
  {
    size_t idx1 = index1(key);
    if (isOccupied(table1_[idx1]) && equal_(table1_[idx1].first, key)) {
      return table1_[idx1].second;
    }
    size_t idx2 = index2(key);
    if (isOccupied(table2_[idx2]) && equal_(table2_[idx2].first, key)) {
      return table2_[idx2].second;
    }
    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value & CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator[](const Key & key)
  {
    if (!contains(key)) {
      insert(key, Value());
    }
    return get(key);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::erase(const Key & key)
  {
    size_t idx1 = index1(key);

    if (isOccupied(table1_[idx1]) && equal_(table1_[idx1].first, key)) {
      Value val = table1_[idx1].second;
      table1_[idx1] = Pair();
      --size_;
      return val;
    }

    size_t idx2 = index2(key);

    if (isOccupied(table2_[idx2]) && equal_(table2_[idx2].first, key)) {
      Value val = table2_[idx2].second;
      table2_[idx2] = Pair();
      --size_;
      return val;
    }

    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::clear()
  {
    const size_t cap1 = table1_.getSize();
    const size_t cap2 = table2_.getSize();

    table1_.clear();
    table2_.clear();

    table1_.reserve(cap1);
    table2_.reserve(cap2);

    for (size_t i = 0; i < cap1; ++i) {
      table1_.pushBack(Pair());
    }

    for (size_t i = 0; i < cap2; ++i) {
      table2_.pushBack(Pair());
    }

    size_ = 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::capacity() const noexcept
  {
    return table1_.getSize() + table2_.getSize();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::swap(
    CuckooHashTable & other) noexcept
  {
    table1_.swap(other.table1_);
    table2_.swap(other.table2_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
    std::swap(size_, other.size_);
    std::swap(maxRehashSteps_, other.maxRehashSteps_);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::index1(const Key & key) const
  {
    size_t capacity = table1_.getSize();

    if (capacity == 0) {
      return 0;
    }
    return hash1_(key) % capacity;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::index2(const Key & key) const
  {
    size_t capacity = table2_.getSize();

    if (capacity == 0) {
      return 0;
    }
    return hash2_(key) % capacity;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::isOccupied(const Pair & pair) const
  {
    return pair.first != Key();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::rehash()
  {
    size_t newSize = table1_.getSize() * 2;

    if (newSize == 0) {
      newSize = 16;
    }

    rehash(newSize);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::rehash(size_t newSize)
  {
    Vector< Pair > oldTable1 = std::move(table1_);
    Vector< Pair > oldTable2 = std::move(table2_);

    table1_ = Vector< Pair >();
    table2_ = Vector< Pair >();

    table1_.reserve(newSize);
    table2_.reserve(newSize);

    for (size_t i = 0; i < newSize; ++i) {
      table1_.pushBack(Pair());
      table2_.pushBack(Pair());
    }
    size_ = 0;

    for (size_t i = 0; i < oldTable1.getSize(); ++i) {
      if (isOccupied(oldTable1[i])) {
        insert(oldTable1[i].first, oldTable1[i].second);
      }
    }

    for (size_t i = 0; i < oldTable2.getSize(); ++i) {
      if (isOccupied(oldTable2[i])) {
        insert(oldTable2[i].first, oldTable2[i].second);
      }
    }
  }
}

#endif
