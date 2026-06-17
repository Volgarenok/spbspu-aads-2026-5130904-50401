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

    std::pair< Slot *, int > findKey(const Key & k) const;
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

#endif
