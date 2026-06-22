#ifndef RH_HASH_TABLE_HPP
#define RH_HASH_TABLE_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace zharov
{
  template< class Key, class Value, class Hash, class Equal >
  class RHHashTable;

  namespace detail
  {
    template< class Key, class Value >
    struct Slot
    {
      Slot() = delete;
      Slot(const Key& key, const Value& value, size_t psl):
        kv_(key, value),
        psl_(psl)
      {}
      Slot(Key&& key, Value&& value, size_t psl):
        kv_(std::move(key), std::move(value)),
        psl_(psl)
      {}
      std::pair< const Key, Value > kv_;
      size_t psl_;
    };
  }

  template< class Key, class Value, class Hash, class Equal >
  class Iter
  {
  public:
    std::pair< const Key, Value >& operator*() const;
    std::pair< const Key, Value >* operator->() const;
    Iter& operator++();
    Iter operator++(int);
    bool operator==(const Iter& it) const;
    bool operator!=(const Iter& it) const;

  private:
    friend class RHHashTable< Key, Value, Hash, Equal >;
    bool* occupied_;
    detail::Slot< Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    Iter(bool* occupied, detail::Slot< Key, Value >* slots, size_t curr, size_t capacity);
  };

  template< class Key, class Value, class Hash, class Equal >
  class CIter
  {
  public:
    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;
    CIter& operator++();
    CIter operator++(int);
    bool operator==(const CIter& it) const;
    bool operator!=(const CIter& it) const;

  private:
    friend class RHHashTable< Key, Value, Hash, Equal >;
    bool* occupied_;
    detail::Slot< Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    CIter(bool* occupied, detail::Slot< Key, Value >* slots, size_t curr, size_t capacity);
  };

  template< class Key, class Value, class Hash, class Equal >
  class RHHashTable
  {
  public:
    using iterator = Iter< Key, Value, Hash, Equal >;
    using const_iterator = CIter< Key, Value, Hash, Equal >;

    RHHashTable();
    explicit RHHashTable(size_t capacity);
    RHHashTable(const RHHashTable& table);
    RHHashTable(RHHashTable&& table) noexcept;
    ~RHHashTable();

    RHHashTable& operator=(const RHHashTable& table);
    RHHashTable& operator=(RHHashTable&& table) noexcept;

    void add(const Key& k, const Value& v);
    void add(Key&& k, Value&& v);
    void remove(const Key& k);
    bool has(const Key& k) const;
    void rehash(size_t slots = 0);
    void swap(RHHashTable& table) noexcept;
    size_t size() const;
    size_t capacity() const;
    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    template< class K, class V >
    void addImpl(K&& k, V&& v);

    Hash hasher_;
    Equal equal_;
    bool* occupied_;
    detail::Slot< Key, Value >* slots_;
    size_t capacity_;
    size_t size_;
  };
}

template< class Key, class Value, class Hash, class Equal >
zharov::RHHashTable< Key, Value, Hash, Equal >::RHHashTable():
  RHHashTable(16)
{}

template< class Key, class Value, class Hash, class Equal >
zharov::RHHashTable< Key, Value, Hash, Equal >::RHHashTable(size_t capacity):
  hasher_(Hash{}),
  equal_(Equal{}),
  occupied_(nullptr),
  slots_(nullptr),
  capacity_(capacity),
  size_(0)
{
  try
  {
    occupied_ = new bool[capacity_]{};
    slots_ = static_cast< detail::Slot< Key, Value >* >(
      ::operator new(sizeof(detail::Slot< Key, Value >) * capacity_));
  }
  catch (...)
  {
    delete[] occupied_;
    ::operator delete(slots_);
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
void zharov::RHHashTable< Key, Value, Hash, Equal >::swap(RHHashTable& table) noexcept
{
  std::swap(hasher_, table.hasher_);
  std::swap(equal_, table.equal_);
  std::swap(occupied_, table.occupied_);
  std::swap(slots_, table.slots_);
  std::swap(capacity_, table.capacity_);
  std::swap(size_, table.size_);
}

template< class Key, class Value, class Hash, class Equal >
zharov::RHHashTable< Key, Value, Hash, Equal >::RHHashTable(const RHHashTable& table):
  RHHashTable(table.capacity_)
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (table.occupied_[i])
    {
      new (slots_ + i) detail::Slot< Key, Value >(
        table.slots_[i].kv_.first, table.slots_[i].kv_.second, table.slots_[i].psl_);
      occupied_[i] = true;
      ++size_;
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
zharov::RHHashTable< Key, Value, Hash, Equal >::RHHashTable(RHHashTable&& table) noexcept:
  hasher_(std::move(table.hasher_)),
  equal_(std::move(table.equal_)),
  occupied_(table.occupied_),
  slots_(table.slots_),
  capacity_(table.capacity_),
  size_(table.size_)
{
  table.occupied_ = nullptr;
  table.slots_ = nullptr;
  table.capacity_ = 0;
  table.size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
zharov::RHHashTable< Key, Value, Hash, Equal >&
  zharov::RHHashTable< Key, Value, Hash, Equal >::operator=(const RHHashTable& table)
{
  RHHashTable tmp(table);
  swap(tmp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::RHHashTable< Key, Value, Hash, Equal >&
  zharov::RHHashTable< Key, Value, Hash, Equal >::operator=(RHHashTable&& table) noexcept
{
  swap(table);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::RHHashTable< Key, Value, Hash, Equal >::~RHHashTable()
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (occupied_[i])
    {
      (slots_ + i)->~Slot();
    }
  }
  delete[] occupied_;
  ::operator delete(slots_);
}

template< class Key, class Value, class Hash, class Equal >
size_t zharov::RHHashTable< Key, Value, Hash, Equal >::size() const
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t zharov::RHHashTable< Key, Value, Hash, Equal >::capacity() const
{
  return capacity_;
}

template< class Key, class Value, class Hash, class Equal >
void zharov::RHHashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
{
  addImpl(k, v);
}

template< class Key, class Value, class Hash, class Equal >
void zharov::RHHashTable< Key, Value, Hash, Equal >::add(Key&& k, Value&& v)
{
  addImpl(std::forward< Key >(k), std::forward< Value >(v));
}

template< class Key, class Value, class Hash, class Equal >
template< class K, class V >
void zharov::RHHashTable< Key, Value, Hash, Equal >::addImpl(K&& k, V&& v)
{
  if (size_ >= capacity_ * 3 / 4)
  {
    rehash();
  }
  size_t end = hasher_(k) % capacity_;
  size_t psl = 0;
  bool check_dup = true;
  for (size_t i = 0; i < capacity_; ++i, end = (end + 1) % capacity_, ++psl)
  {
    if (!occupied_[end])
    {
      break;
    }
    if (check_dup)
    {
      if (slots_[end].psl_ < psl)
      {
        check_dup = false;
      }
      else if (equal_(slots_[end].kv_.first, k))
      {
        throw std::invalid_argument("key already exists");
      }
    }
  }
  new (slots_ + end) detail::Slot< Key, Value >(std::forward< K >(k), std::forward< V >(v), psl);
  occupied_[end] = true;
  ++size_;
  while (slots_[end].psl_ > 0)
  {
    size_t prev = (end + capacity_ - 1) % capacity_;
    if (slots_[prev].psl_ >= slots_[end].psl_ - 1)
    {
      break;
    }
    std::swap(
      const_cast< Key& >(slots_[prev].kv_.first), const_cast< Key& >(slots_[end].kv_.first));
    std::swap(slots_[prev].kv_.second, slots_[end].kv_.second);
    std::swap(slots_[prev].psl_, slots_[end].psl_);
    --(slots_ + prev)->psl_;
    ++(slots_ + end)->psl_;
    end = prev;
  }
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::RHHashTable< Key, Value, Hash, Equal >::has(const Key& k) const
{
  size_t idx = hasher_(k) % capacity_;
  for (size_t i = 0; i < capacity_; ++i, idx = (idx + 1) % capacity_)
  {
    if (!occupied_[idx] || slots_[idx].psl_ < i)
    {
      return false;
    }
    if (equal_(slots_[idx].kv_.first, k))
    {
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
const Value& zharov::RHHashTable< Key, Value, Hash, Equal >::at(const Key& k) const
{
  size_t idx = hasher_(k) % capacity_;
  for (size_t i = 0; i < capacity_; ++i, idx = (idx + 1) % capacity_)
  {
    if (!occupied_[idx] || slots_[idx].psl_ < i)
    {
      throw std::out_of_range("key not found");
    }
    if (equal_(slots_[idx].kv_.first, k))
    {
      return slots_[idx].kv_.second;
    }
  }
  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Hash, class Equal >
Value& zharov::RHHashTable< Key, Value, Hash, Equal >::at(const Key& k)
{
  const RHHashTable* const_table = this;
  return const_cast< Value& >((*const_table).at(k));
}

template< class Key, class Value, class Hash, class Equal >
void zharov::RHHashTable< Key, Value, Hash, Equal >::remove(const Key& k)
{
  size_t idx = hasher_(k) % capacity_;
  size_t found = capacity_;
  for (size_t i = 0; i < capacity_; ++i, idx = (idx + 1) % capacity_)
  {
    if (!occupied_[idx] || slots_[idx].psl_ < i)
    {
      throw std::out_of_range("key not found");
    }
    if (equal_(slots_[idx].kv_.first, k))
    {
      found = idx;
      break;
    }
  }
  if (found == capacity_)
  {
    throw std::out_of_range("key not found");
  }
  size_t next = (found + 1) % capacity_;
  while (occupied_[next] && slots_[next].psl_ != 0)
  {
    const_cast< Key& >(slots_[found].kv_.first) =
      std::move(const_cast< Key& >(slots_[next].kv_.first));
    slots_[found].kv_.second = std::move(slots_[next].kv_.second);
    slots_[found].psl_ = slots_[next].psl_;
    --(slots_ + found)->psl_;
    found = next;
    next = (found + 1) % capacity_;
  }
  (slots_ + found)->~Slot();
  occupied_[found] = false;
  --size_;
}

template< class Key, class Value, class Hash, class Equal >
void zharov::RHHashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  size_t new_capacity = slots ? slots : capacity_ * 2;
  if (new_capacity < size_)
  {
    throw std::invalid_argument("capacity too small");
  }
  RHHashTable tmp(new_capacity);
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (occupied_[i])
    {
      tmp.add(std::move(slots_[i].kv_.first), std::move(slots_[i].kv_.second));
    }
  }
  swap(tmp);
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal >::Iter(bool* occupied,
  detail::Slot< Key, Value >* slots,
  size_t curr,
  size_t capacity):
  occupied_(occupied),
  slots_(slots),
  curr_(curr),
  capacity_(capacity)
{}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key, Value >& zharov::Iter< Key, Value, Hash, Equal >::operator*() const
{
  return slots_[curr_].kv_;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key, Value >* zharov::Iter< Key, Value, Hash, Equal >::operator->() const
{
  return &slots_[curr_].kv_;
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal >& zharov::Iter< Key, Value, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && !occupied_[curr_])
  {
    ++curr_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::Iter< Key, Value, Hash, Equal >::operator++(int)
{
  Iter tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::Iter< Key, Value, Hash, Equal >::operator==(const Iter& it) const
{
  return curr_ == it.curr_;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::Iter< Key, Value, Hash, Equal >::operator!=(const Iter& it) const
{
  return !(*this == it);
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >::CIter(bool* occupied,
  detail::Slot< Key, Value >* slots,
  size_t curr,
  size_t capacity):
  occupied_(occupied),
  slots_(slots),
  curr_(curr),
  capacity_(capacity)
{}

template< class Key, class Value, class Hash, class Equal >
const std::pair< const Key, Value >& zharov::CIter< Key, Value, Hash, Equal >::operator*() const
{
  return slots_[curr_].kv_;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< const Key, Value >* zharov::CIter< Key, Value, Hash, Equal >::operator->() const
{
  return &slots_[curr_].kv_;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >& zharov::CIter< Key, Value, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && !occupied_[curr_])
  {
    ++curr_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::CIter< Key, Value, Hash, Equal >::operator++(int)
{
  CIter tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::CIter< Key, Value, Hash, Equal >::operator==(const CIter& it) const
{
  return curr_ == it.curr_;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::CIter< Key, Value, Hash, Equal >::operator!=(const CIter& it) const
{
  return !(*this == it);
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::RHHashTable< Key, Value, Hash, Equal >::iterator
  zharov::RHHashTable< Key, Value, Hash, Equal >::begin()
{
  size_t i = 0;
  while (i < capacity_ && !occupied_[i])
  {
    ++i;
  }
  return iterator(occupied_, slots_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::RHHashTable< Key, Value, Hash, Equal >::const_iterator
  zharov::RHHashTable< Key, Value, Hash, Equal >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::RHHashTable< Key, Value, Hash, Equal >::const_iterator
  zharov::RHHashTable< Key, Value, Hash, Equal >::cbegin() const
{
  size_t i = 0;
  while (i < capacity_ && !occupied_[i])
  {
    ++i;
  }
  return const_iterator(occupied_, slots_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::RHHashTable< Key, Value, Hash, Equal >::iterator
  zharov::RHHashTable< Key, Value, Hash, Equal >::end()
{
  return iterator(occupied_, slots_, capacity_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::RHHashTable< Key, Value, Hash, Equal >::const_iterator
  zharov::RHHashTable< Key, Value, Hash, Equal >::end() const
{
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::RHHashTable< Key, Value, Hash, Equal >::const_iterator
  zharov::RHHashTable< Key, Value, Hash, Equal >::cend() const
{
  return const_iterator(occupied_, slots_, capacity_, capacity_);
}

#endif
