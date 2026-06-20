#ifndef ROBIN_HOOD_HASH_TABLE_HPP
#define ROBIN_HOOD_HASH_TABLE_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace hvostov {

  template< class Key, class T, class Hash, class Equal >
  class RobinHoodHashTable;

  namespace detail {
    template< class Key, class T >
    struct Slot {
      Slot() = delete;
      Slot(const Key& key, const T& value, size_t psl);
      Slot(Key&& key, T&& value, size_t psl);

      std::pair< const Key, T > kv;
      size_t psl = 0;
    };
  }

  template< class Key, class T, class Hash, class Equal >
  class RobinHoodHashTableIterator {
  public:
    RobinHoodHashTableIterator() = default;
    RobinHoodHashTableIterator(const RobinHoodHashTableIterator& it) = default;
    RobinHoodHashTableIterator(RobinHoodHashTableIterator&& it) noexcept = default;
    RobinHoodHashTableIterator& operator=(const RobinHoodHashTableIterator& it) = default;
    RobinHoodHashTableIterator& operator=(RobinHoodHashTableIterator&& it) noexcept = default;
    RobinHoodHashTableIterator& operator++();
    RobinHoodHashTableIterator operator++(int);
    std::pair< const Key, T >& operator*();
    std::pair< const Key, T >* operator->();
    bool operator==(const RobinHoodHashTableIterator& it) const;
    bool operator!=(const RobinHoodHashTableIterator& it) const;

  private:
    friend class RobinHoodHashTable< Key, T, Hash, Equal >;

    detail::Slot< Key, T >* slots_ = nullptr;
    bool* occupied_ = nullptr;
    size_t curr_ = 0;
    size_t capacity_ = 0;

    RobinHoodHashTableIterator(detail::Slot< Key, T >* slots, bool* occupied, size_t curr, size_t capacity);
  };

  template< class Key, class T, class Hash, class Equal >
  class RobinHoodHashTableConstIterator {
  public:
    RobinHoodHashTableConstIterator() = default;
    RobinHoodHashTableConstIterator(const RobinHoodHashTableConstIterator& it) = default;
    RobinHoodHashTableConstIterator(RobinHoodHashTableConstIterator&& it) noexcept = default;
    RobinHoodHashTableConstIterator(const RobinHoodHashTableIterator< Key, T, Hash, Equal >& it);
    RobinHoodHashTableConstIterator& operator=(const RobinHoodHashTableConstIterator& it) = default;
    RobinHoodHashTableConstIterator& operator=(RobinHoodHashTableConstIterator&& it) noexcept = default;
    RobinHoodHashTableConstIterator& operator++();
    RobinHoodHashTableConstIterator operator++(int);
    const std::pair< const Key, T >& operator*() const;
    const std::pair< const Key, T >* operator->() const;
    bool operator==(const RobinHoodHashTableConstIterator& it) const;
    bool operator!=(const RobinHoodHashTableConstIterator& it) const;

  private:
    friend class RobinHoodHashTable< Key, T, Hash, Equal >;

    const detail::Slot< Key, T >* slots_ = nullptr;
    const bool* occupied_ = nullptr;
    size_t curr_ = 0;
    size_t capacity_ = 0;

    RobinHoodHashTableConstIterator(const detail::Slot< Key, T >* slots, const bool* occupied, size_t curr,
                                    size_t capacity);
  };

  template< class Key, class T, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class RobinHoodHashTable {
  public:
    using iterator = RobinHoodHashTableIterator< Key, T, Hash, Equal >;
    using const_iterator = RobinHoodHashTableConstIterator< Key, T, Hash, Equal >;

    RobinHoodHashTable();
    RobinHoodHashTable(const RobinHoodHashTable& other);
    RobinHoodHashTable(RobinHoodHashTable&& other) noexcept;
    explicit RobinHoodHashTable(size_t capacity);
    RobinHoodHashTable(std::initializer_list< std::pair< const Key, T > > init);
    ~RobinHoodHashTable();

    RobinHoodHashTable& operator=(const RobinHoodHashTable& other);
    RobinHoodHashTable& operator=(RobinHoodHashTable&& other) noexcept;

    T& operator[](const Key& k);
    const T& operator[](const Key& k) const;
    T& at(const Key& k);
    const T& at(const Key& k) const;

    template< class... Args >
    void emplace(const Key& k, Args&&... args);

    template< class... Args >
    void emplace(Key&& k, Args&&... args);

    void rehash(size_t count = 0);
    void reserve(size_t count);

    void swap(RobinHoodHashTable& other) noexcept;

    iterator find(const Key& k);
    const_iterator find(const Key& k) const;
    bool contains(const Key& k) const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    void erase(const Key& key);
    void clear() noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

  private:
    template< class K, class... Args >
    void emplaceImpl(K&& k, Args&&... args);

    void rehashEmplace(Key&& k, T&& v);
    size_t findSlot(const Key& k) const;

    Hash hasher_;
    Equal equal_;
    bool* occupied_;
    detail::Slot< Key, T >* slots_;
    size_t capacity_;
    size_t size_;
    float max_load_factor_;
  };

}

template< class Key, class T >
hvostov::detail::Slot< Key, T >::Slot(const Key& key, const T& value, size_t psl):
  kv(key, value),
  psl(psl)
{}

template< class Key, class T >
hvostov::detail::Slot< Key, T >::Slot(Key&& key, T&& value, size_t psl):
  kv(std::move(key), std::move(value)),
  psl(psl)
{}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::RobinHoodHashTable(
    std::initializer_list< std::pair< const Key, T > > init):
  RobinHoodHashTable(init.size())
{
  for (const auto& pair : init) {
    emplace(pair.first, pair.second);
  }
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::RobinHoodHashTable():
  hasher_(),
  equal_(),
  occupied_(nullptr),
  slots_(nullptr),
  capacity_(0),
  size_(0),
  max_load_factor_(0.75f)
{}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::RobinHoodHashTable(size_t capacity):
  hasher_(),
  equal_(),
  occupied_(nullptr),
  slots_(nullptr),
  capacity_(0),
  size_(0),
  max_load_factor_(0.75f)
{
  reserve(capacity);
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::RobinHoodHashTable(const RobinHoodHashTable& other):
  hasher_(other.hasher_),
  equal_(other.equal_),
  occupied_(nullptr),
  slots_(nullptr),
  capacity_(0),
  size_(0),
  max_load_factor_(other.max_load_factor_)
{
  if (other.capacity_ > 0) {
    try {
      occupied_ = new bool[other.capacity_]();
      slots_ = static_cast< detail::Slot< Key, T >* >(::operator new(sizeof(detail::Slot< Key, T >) * other.capacity_));
      capacity_ = other.capacity_;

      for (size_t i = 0; i < capacity_; ++i) {
        if (other.occupied_[i]) {
          new (slots_ + i)
              detail::Slot< Key, T >(other.slots_[i].kv.first, other.slots_[i].kv.second, other.slots_[i].psl);
          occupied_[i] = true;
          ++size_;
        }
      }
    } catch (...) {
      delete[] occupied_;
      ::operator delete(slots_);
      throw;
    }
  }
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::RobinHoodHashTable(RobinHoodHashTable&& other) noexcept:
  hasher_(std::move(other.hasher_)),
  equal_(std::move(other.equal_)),
  occupied_(other.occupied_),
  slots_(other.slots_),
  capacity_(other.capacity_),
  size_(other.size_),
  max_load_factor_(other.max_load_factor_)
{
  other.occupied_ = nullptr;
  other.slots_ = nullptr;
  other.capacity_ = 0;
  other.size_ = 0;
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::~RobinHoodHashTable()
{
  if (slots_) {
    for (size_t i = 0; i < capacity_; ++i) {
      if (occupied_[i]) {
        (slots_ + i)->~Slot();
      }
    }
    ::operator delete(slots_);
  }
  delete[] occupied_;
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >&
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::operator=(const RobinHoodHashTable& other)
{
  if (this != std::addressof(other)) {
    RobinHoodHashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >&
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::operator=(RobinHoodHashTable&& other) noexcept
{
  if (this != std::addressof(other)) {
    swap(other);
  }
  return *this;
}

template< class Key, class T, class Hash, class Equal >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::swap(RobinHoodHashTable& other) noexcept
{
  std::swap(hasher_, other.hasher_);
  std::swap(equal_, other.equal_);
  std::swap(occupied_, other.occupied_);
  std::swap(slots_, other.slots_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(max_load_factor_, other.max_load_factor_);
}

template< class Key, class T, class Hash, class Equal >
size_t hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::findSlot(const Key& k) const
{
  if (capacity_ == 0) {
    return capacity_;
  }

  size_t idx = hasher_(k) % capacity_;
  for (size_t i = 0; i < capacity_; ++i, idx = (idx + 1) % capacity_) {
    if (!occupied_[idx] || slots_[idx].psl < i) {
      return capacity_;
    }
    if (equal_(slots_[idx].kv.first, k)) {
      return idx;
    }
  }
  return capacity_;
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::find(const Key& k)
{
  size_t idx = findSlot(k);
  if (idx == capacity_) {
    return end();
  }
  return iterator(slots_, occupied_, idx, capacity_);
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::const_iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::find(const Key& k) const
{
  size_t idx = findSlot(k);
  if (idx == capacity_) {
    return end();
  }
  return const_iterator(slots_, occupied_, idx, capacity_);
}

template< class Key, class T, class Hash, class Equal >
T& hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::operator[](const Key& k)
{
  if (!contains(k)) {
    emplace(k);
  }
  return at(k);
}

template< class Key, class T, class Hash, class Equal >
const T& hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::operator[](const Key& k) const
{
  return at(k);
}

template< class Key, class T, class Hash, class Equal >
T& hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::at(const Key& k)
{
  size_t idx = findSlot(k);
  if (idx == capacity_) {
    throw std::out_of_range("Key not found");
  }
  return slots_[idx].kv.second;
}

template< class Key, class T, class Hash, class Equal >
const T& hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::at(const Key& k) const
{
  size_t idx = findSlot(k);
  if (idx == capacity_) {
    throw std::out_of_range("Key not found");
  }
  return slots_[idx].kv.second;
}

template< class Key, class T, class Hash, class Equal >
bool hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::contains(const Key& k) const
{
  return findSlot(k) != capacity_;
}

template< class Key, class T, class Hash, class Equal >
template< class... Args >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::emplace(const Key& k, Args&&... args)
{
  emplaceImpl(k, std::forward< Args >(args)...);
}

template< class Key, class T, class Hash, class Equal >
template< class... Args >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::emplace(Key&& k, Args&&... args)
{
  emplaceImpl(std::move(k), std::forward< Args >(args)...);
}

template< class Key, class T, class Hash, class Equal >
template< class K, class... Args >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::emplaceImpl(K&& k, Args&&... args)
{
  if (size_ >= capacity_ * max_load_factor_) {
    rehash(capacity_ * 2);
  }

  size_t hash = hasher_(k) % capacity_;
  size_t idx = hash;
  size_t psl = 0;

  for (size_t i = 0; i < capacity_; ++i, idx = (idx + 1) % capacity_, ++psl) {
    if (!occupied_[idx]) {
      break;
    }
    if (slots_[idx].psl < psl) {
      break;
    }
    if (equal_(slots_[idx].kv.first, k)) {
      slots_[idx].kv.second = T(std::forward< Args >(args)...);
      return;
    }
  }

  idx = hash;
  psl = 0;
  Key key(std::forward< K >(k));
  T value(std::forward< Args >(args)...);

  while (true) {
    if (!occupied_[idx]) {
      new (slots_ + idx) detail::Slot< Key, T >(std::move(key), std::move(value), psl);
      occupied_[idx] = true;
      ++size_;
      return;
    }

    if (psl > slots_[idx].psl) {
      Key old_key(std::move(const_cast< Key& >(slots_[idx].kv.first)));
      T old_value(std::move(slots_[idx].kv.second));
      size_t old_psl = slots_[idx].psl;

      (slots_ + idx)->~Slot();

      new (slots_ + idx) detail::Slot< Key, T >(std::move(key), std::move(value), psl);

      key = std::move(old_key);
      value = std::move(old_value);
      psl = old_psl;
    }

    idx = (idx + 1) % capacity_;
    ++psl;
  }
}

template< class Key, class T, class Hash, class Equal >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::rehashEmplace(Key&& k, T&& v)
{
  size_t hash = hasher_(k) % capacity_;
  size_t idx = hash;
  size_t psl = 0;

  Key key(std::move(k));
  T value(std::move(v));

  while (true) {
    if (!occupied_[idx]) {
      new (slots_ + idx) detail::Slot< Key, T >(std::move(key), std::move(value), psl);
      occupied_[idx] = true;
      ++size_;
      return;
    }

    if (psl > slots_[idx].psl) {
      Key old_key(std::move(const_cast< Key& >(slots_[idx].kv.first)));
      T old_value(std::move(slots_[idx].kv.second));
      size_t old_psl = slots_[idx].psl;

      (slots_ + idx)->~Slot();

      new (slots_ + idx) detail::Slot< Key, T >(std::move(key), std::move(value), psl);

      key = std::move(old_key);
      value = std::move(old_value);
      psl = old_psl;
    }

    idx = (idx + 1) % capacity_;
    ++psl;
  }
}

template< class Key, class T, class Hash, class Equal >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::rehash(size_t count)
{
  size_t new_capacity = count ? count : capacity_ * 2;
  if (new_capacity < size_) {
    throw std::invalid_argument("capacity too small");
  }

  if (new_capacity < 8) {
    new_capacity = 8;
  }

  RobinHoodHashTable tmp;
  tmp.capacity_ = new_capacity;
  tmp.max_load_factor_ = max_load_factor_;
  tmp.hasher_ = hasher_;
  tmp.equal_ = equal_;

  try {
    tmp.occupied_ = new bool[new_capacity]();
    tmp.slots_ = static_cast< detail::Slot< Key, T >* >(::operator new(sizeof(detail::Slot< Key, T >) * new_capacity));

    for (size_t i = 0; i < capacity_; ++i) {
      if (occupied_[i]) {
        tmp.rehashEmplace(std::move(const_cast< Key& >(slots_[i].kv.first)), std::move(slots_[i].kv.second));
        (slots_ + i)->~Slot();
      }
    }

  } catch (...) {
    throw;
  }

  swap(tmp);
}

template< class Key, class T, class Hash, class Equal >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::reserve(size_t count)
{
  size_t needed = static_cast< size_t >(count / max_load_factor_) + 1;
  if (needed > capacity_) {
    rehash(needed);
  }
}

template< class Key, class T, class Hash, class Equal >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::erase(const Key& key)
{
  if (capacity_ == 0) {
    return;
  }

  size_t idx = hasher_(key) % capacity_;
  size_t found = capacity_;

  for (size_t i = 0; i < capacity_; ++i, idx = (idx + 1) % capacity_) {
    if (!occupied_[idx] || slots_[idx].psl < i) {
      return;
    }
    if (equal_(slots_[idx].kv.first, key)) {
      found = idx;
      break;
    }
  }

  if (found == capacity_) {
    return;
  }

  size_t next = (found + 1) % capacity_;
  while (occupied_[next] && slots_[next].psl > 0) {
    Key moved_key(std::move(const_cast< Key& >(slots_[next].kv.first)));
    T moved_value(std::move(slots_[next].kv.second));
    size_t moved_psl = slots_[next].psl - 1;

    (slots_ + found)->~Slot();
    (slots_ + next)->~Slot();

    new (slots_ + found) detail::Slot< Key, T >(std::move(moved_key), std::move(moved_value), moved_psl);
    occupied_[found] = true;
    occupied_[next] = false;

    found = next;
    next = (found + 1) % capacity_;
  }

  (slots_ + found)->~Slot();
  occupied_[found] = false;
  --size_;
}

template< class Key, class T, class Hash, class Equal >
void hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::clear() noexcept
{
  if (slots_) {
    for (size_t i = 0; i < capacity_; ++i) {
      if (occupied_[i]) {
        (slots_ + i)->~Slot();
      }
    }
    ::operator delete(slots_);
    slots_ = nullptr;
  }
  delete[] occupied_;
  occupied_ = nullptr;
  capacity_ = 0;
  size_ = 0;
}

template< class Key, class T, class Hash, class Equal >
bool hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class T, class Hash, class Equal >
size_t hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class T, class Hash, class Equal >
size_t hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::capacity() const noexcept
{
  return capacity_;
}

template< class Key, class T, class Hash, class Equal >
std::pair< const Key, T >& hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >::operator*()
{
  return slots_[curr_].kv;
}

template< class Key, class T, class Hash, class Equal >
std::pair< const Key, T >* hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >::operator->()
{
  return &slots_[curr_].kv;
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >&
hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && !occupied_[curr_]) {
    ++curr_;
  }
  return *this;
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >
hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >::operator++(int)
{
  RobinHoodHashTableIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class T, class Hash, class Equal >
bool hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >::operator==(const RobinHoodHashTableIterator& it) const
{
  return slots_ == it.slots_ && curr_ == it.curr_;
}

template< class Key, class T, class Hash, class Equal >
bool hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >::operator!=(const RobinHoodHashTableIterator& it) const
{
  return !(*this == it);
}

template< class Key, class T, class Hash, class Equal >
const std::pair< const Key, T >& hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::operator*() const
{
  return slots_[curr_].kv;
}

template< class Key, class T, class Hash, class Equal >
const std::pair< const Key, T >* hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::operator->() const
{
  return &slots_[curr_].kv;
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >&
hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && !occupied_[curr_]) {
    ++curr_;
  }
  return *this;
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >
hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::operator++(int)
{
  RobinHoodHashTableConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class T, class Hash, class Equal >
bool hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::operator==(
    const RobinHoodHashTableConstIterator& it) const
{
  return slots_ == it.slots_ && curr_ == it.curr_;
}

template< class Key, class T, class Hash, class Equal >
bool hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::operator!=(
    const RobinHoodHashTableConstIterator& it) const
{
  return !(*this == it);
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::begin() noexcept
{
  size_t i = 0;
  while (i < capacity_ && !occupied_[i]) {
    ++i;
  }
  return iterator(slots_, occupied_, i, capacity_);
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::end() noexcept
{
  return iterator(slots_, occupied_, capacity_, capacity_);
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::const_iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::begin() const noexcept
{
  return cbegin();
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::const_iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::end() const noexcept
{
  return cend();
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::const_iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::cbegin() const noexcept
{
  size_t i = 0;
  while (i < capacity_ && !occupied_[i]) {
    ++i;
  }
  return const_iterator(slots_, occupied_, i, capacity_);
}

template< class Key, class T, class Hash, class Equal >
typename hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::const_iterator
hvostov::RobinHoodHashTable< Key, T, Hash, Equal >::cend() const noexcept
{
  return const_iterator(slots_, occupied_, capacity_, capacity_);
}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTableIterator< Key, T, Hash, Equal >::RobinHoodHashTableIterator(detail::Slot< Key, T >* slots,
                                                                                       bool* occupied, size_t curr,
                                                                                       size_t capacity):
  slots_(slots),
  occupied_(occupied),
  curr_(curr),
  capacity_(capacity)
{}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::RobinHoodHashTableConstIterator(
    const detail::Slot< Key, T >* slots, const bool* occupied, size_t curr, size_t capacity):
  slots_(slots),
  occupied_(occupied),
  curr_(curr),
  capacity_(capacity)
{}

template< class Key, class T, class Hash, class Equal >
hvostov::RobinHoodHashTableConstIterator< Key, T, Hash, Equal >::RobinHoodHashTableConstIterator(
    const RobinHoodHashTableIterator< Key, T, Hash, Equal >& it):
  slots_(it.slots_),
  occupied_(it.occupied_),
  curr_(it.curr_),
  capacity_(it.capacity_)
{}

#endif
