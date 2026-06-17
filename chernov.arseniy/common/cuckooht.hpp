#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

namespace chernov {

  template< class K, class V, class H1, class H2, class E, bool IC >
  class CuckooHTIter;

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooHT {
  public:
    template< class K, class V, class H1, class H2, class E, bool IC >
    friend class CuckooHTIter;

    using Slot = std::pair< const Key, Value >;
    using iterator = CuckooHTIter< Key, Value, Hash1, Hash2, Equal, false >;
    using const_iterator = CuckooHTIter< Key, Value, Hash1, Hash2, Equal, true >;

    CuckooHT();
    CuckooHT(const CuckooHT & ht);
    CuckooHT(CuckooHT && ht) noexcept;

    explicit CuckooHT(size_t slots);

    ~CuckooHT();

    CuckooHT & operator=(const CuckooHT & ht);
    CuckooHT & operator=(CuckooHT && ht) noexcept;

    void swap(CuckooHT & ht) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t maxCapacity() const noexcept;

    void clear() noexcept;

    void add(Key k, Value v);
    void remove(Key k);
    bool has(Key k) const;
    void rehash(size_t slots);

    Value & at(const Key & k);
    const Value & at(const Key & k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    Slot * table1_;
    Slot * table2_;
    bool * occupied1_;
    bool * occupied2_;
    size_t capacity_;
    size_t count_;

    Hash1 hasher1_;
    Hash2 hasher2_;
    Equal equal_;

    CuckooHT(size_t capacity, int);

    size_t hash1(const Key & k) const noexcept;
    size_t hash2(const Key & k) const noexcept;

    const Slot * findKey(const Key & k) const;
    Slot * findKey(const Key & k);
    void insertWithEviction(Key k, Value v);
    void rehashInternal(size_t newSlots);

    size_t firstValidIndex() const noexcept;
    size_t endIndex() const noexcept;
  };

}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::CuckooHT():
  table1_(nullptr),
  table2_(nullptr),
  occupied1_(nullptr),
  occupied2_(nullptr),
  capacity_(0),
  count_(0),
  hasher1_(Hash1{}),
  hasher2_(Hash2{}),
  equal_(Equal{})
{}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::CuckooHT(size_t slots):
  table1_(nullptr),
  table2_(nullptr),
  occupied1_(nullptr),
  occupied2_(nullptr),
  capacity_(0),
  count_(0),
  hasher1_(Hash1{}),
  hasher2_(Hash2{}),
  equal_(Equal{})
{
  if (slots > 0)
  {
    capacity_ = (slots + 1) / 2;
    table1_ = static_cast< Slot * >(::operator new (sizeof(Slot) * capacity_));
    table2_ = static_cast< Slot * >(::operator new (sizeof(Slot) * capacity_));
    occupied1_ = new bool[capacity_]{false};
    try
    {
      occupied2_ = new bool[capacity_]{false};
    }
    catch (...)
    {
      ::operator delete (table1_);
      ::operator delete (table2_);
      delete [] occupied1_;
      throw;
    }
  }
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::CuckooHT(size_t capacity, int):
  table1_(nullptr),
  table2_(nullptr),
  occupied1_(nullptr),
  occupied2_(nullptr),
  capacity_(capacity),
  count_(0),
  hasher1_(Hash1{}),
  hasher2_(Hash2{}),
  equal_(Equal{})
{
  if (capacity_ > 0)
  {
    table1_ = static_cast< Slot * >(::operator new (sizeof(Slot) * capacity_));
    table2_ = static_cast< Slot * >(::operator new (sizeof(Slot) * capacity_));
    occupied1_ = new bool[capacity_]{false};
    try
    {
      occupied2_ = new bool[capacity_]{false};
    }
    catch (...)
    {
      ::operator delete (table1_);
      ::operator delete (table2_);
      delete [] occupied1_;
      throw;
    }
  }
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::CuckooHT(const CuckooHT & ht):
  CuckooHT(ht.capacity_, 0)
{
  hasher1_ = ht.hasher1_;
  hasher2_ = ht.hasher2_;
  equal_ = ht.equal_;

  try
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (ht.occupied1_[i])
      {
        new (table1_ + i) Slot(ht.table1_[i]);
        occupied1_[i] = true;
        ++count_;
      }
      if (ht.occupied2_[i])
      {
        new (table2_ + i) Slot(ht.table2_[i]);
        occupied2_[i] = true;
        ++count_;
      }
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (occupied1_[i])
      {
        table1_[i].~Slot();
      }
      if (occupied2_[i])
      {
        table2_[i].~Slot();
      }
    }
    ::operator delete (table1_);
    ::operator delete (table2_);
    delete [] occupied1_;
    delete [] occupied2_;
    throw;
  }
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::CuckooHT(CuckooHT && ht) noexcept:
  table1_(ht.table1_),
  table2_(ht.table2_),
  occupied1_(ht.occupied1_),
  occupied2_(ht.occupied2_),
  capacity_(ht.capacity_),
  count_(ht.count_),
  hasher1_(std::move(ht.hasher1_)),
  hasher2_(std::move(ht.hasher2_)),
  equal_(std::move(ht.equal_))
{
  ht.table1_ = nullptr;
  ht.table2_ = nullptr;
  ht.occupied1_ = nullptr;
  ht.occupied2_ = nullptr;
  ht.capacity_ = 0;
  ht.count_ = 0;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::~CuckooHT()
{
  clear();
  ::operator delete (table1_);
  ::operator delete (table2_);
  delete [] occupied1_;
  delete [] occupied2_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal > &
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::operator=(const CuckooHT & ht)
{
  if (this == std::addressof(ht))
  {
    return *this;
  }
  CuckooHT< Key, Value, Hash1, Hash2, Equal > new_ht(ht);
  swap(new_ht);
  return *this;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal > &
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::operator=(CuckooHT && ht) noexcept
{
  if (this == std::addressof(ht))
  {
    return *this;
  }
  CuckooHT< Key, Value, Hash1, Hash2, Equal > new_ht(std::move(ht));
  swap(new_ht);
  return *this;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::swap(CuckooHT & ht) noexcept
{
  std::swap(table1_, ht.table1_);
  std::swap(table2_, ht.table2_);
  std::swap(occupied1_, ht.occupied1_);
  std::swap(occupied2_, ht.occupied2_);
  std::swap(capacity_, ht.capacity_);
  std::swap(count_, ht.count_);
  std::swap(hasher1_, ht.hasher1_);
  std::swap(hasher2_, ht.hasher2_);
  std::swap(equal_, ht.equal_);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
{
  return count_ == 0;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
size_t chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
{
  return count_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
size_t chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::maxCapacity() const noexcept
{
  return 2 * capacity_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::clear() noexcept
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (occupied1_[i])
    {
      table1_[i].~Slot();
      occupied1_[i] = false;
    }
    if (occupied2_[i])
    {
      table2_[i].~Slot();
      occupied2_[i] = false;
    }
  }
  count_ = 0;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::add(Key k, Value v)
{
  CuckooHT new_ht(*this);
  Slot * existing = new_ht.findKey(k);
  if (existing != nullptr)
  {
    existing->second = v;
  }
  else
  {
    new_ht.insertWithEviction(std::move(k), std::move(v));
  }
  swap(new_ht);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::remove(Key k)
{
  CuckooHT new_ht(*this);
  Slot * slot = new_ht.findKey(k);
  if (slot == nullptr)
  {
    throw std::out_of_range("Element not found");
  }
  size_t index;
  if (slot >= new_ht.table1_ && slot < new_ht.table1_ + new_ht.capacity_)
  {
    index = slot - new_ht.table1_;
    slot->~Slot();
    new_ht.occupied1_[index] = false;
  }
  else
  {
    index = slot - new_ht.table2_;
    slot->~Slot();
    new_ht.occupied2_[index] = false;
  }
  --new_ht.count_;
  swap(new_ht);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::has(Key k) const
{
  return findKey(k) != nullptr;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::rehash(size_t slots)
{
  rehashInternal(slots);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
Value & chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::at(const Key & k)
{
  const CuckooHT * cthis = this;
  return const_cast< Value & >(cthis->at(k));
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
const Value & chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::at(const Key & k) const
{
  const Slot * slot = findKey(k);
  if (slot == nullptr)
  {
    throw std::out_of_range("Element not found");
  }
  return slot->second;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
size_t chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::hash1(const Key & k) const noexcept
{
  if (capacity_ == 0)
  {
    return 0;
  }
  return hasher1_(k) % capacity_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
size_t chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::hash2(const Key & k) const noexcept
{
  if (capacity_ == 0)
  {
    return 0;
  }
  return hasher2_(k) % capacity_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
const typename chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::Slot *
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::findKey(const Key & k) const
{
  if (capacity_ == 0)
  {
    return nullptr;
  }

  size_t h1 = hash1(k);
  if (occupied1_[h1] && equal_(k, table1_[h1].first))
  {
    return &table1_[h1];
  }

  size_t h2 = hash2(k);
  if (occupied2_[h2] && equal_(k, table2_[h2].first))
  {
    return &table2_[h2];
  }

  return nullptr;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::Slot *
chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::findKey(const Key & k)
{
  const CuckooHT * cthis = this;
  return const_cast< Slot * >(cthis->findKey(k));
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::insertWithEviction(Key k, Value v)
{
  const size_t maxIterations = capacity_ * 4 + 1;
  for (size_t iter = 0; iter < maxIterations; ++iter)
  {
    size_t h1 = hash1(k);
    if (!occupied1_[h1])
    {
      new (table1_ + h1) Slot(std::move(k), std::move(v));
      occupied1_[h1] = true;
      ++count_;
      return;
    }
    Slot old = std::move(table1_[h1]);
    table1_[h1].~Slot();
    new (table1_ + h1) Slot(std::move(k), std::move(v));
    k = std::move(old.first);
    v = std::move(old.second);

    size_t h2 = hash2(k);
    if (!occupied2_[h2])
    {
      new (table2_ + h2) Slot(std::move(k), std::move(v));
      occupied2_[h2] = true;
      ++count_;
      return;
    }
    old = std::move(table2_[h2]);
    table2_[h2].~Slot();
    new (table2_ + h2) Slot(std::move(k), std::move(v));
    k = std::move(old.first);
    v = std::move(old.second);
  }
  rehashInternal(2 * capacity_ + 1);
  insertWithEviction(std::move(k), std::move(v));
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void chernov::CuckooHT< Key, Value, Hash1, Hash2, Equal >::rehashInternal(size_t newSlots)
{
  size_t newCapacity = (newSlots + 1) / 2;
  CuckooHT new_ht(newCapacity, 0);
  new_ht.hasher1_ = hasher1_;
  new_ht.hasher2_ = hasher2_;
  new_ht.equal_ = equal_;
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (occupied1_[i])
    {
      new_ht.insertWithEviction(table1_[i].first, table1_[i].second);
    }
    if (occupied2_[i])
    {
      new_ht.insertWithEviction(table2_[i].first, table2_[i].second);
    }
  }
  swap(new_ht);
}

#endif
