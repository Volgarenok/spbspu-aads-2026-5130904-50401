#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace kuznetsov {
  namespace detail {
    enum class State {
      FREE,
      STORE,
      DELETED
    };

    template< bool B, class T, class F >
    struct conditional {
      using type = T;
    };

    template< class T, class F >
    struct conditional< false, T, F > {
      using type = F;
    };

    template< class F, class S >
    struct Slot {
      Slot() = delete;
      Slot(F&& f, S&& s):
        first(std::forward< F >(f)),
        second(std::forward< S >(s))
      {}
      Slot(const F& f, const S& s):
        first(f),
        second(s)
      {}

      F first;
      S second;
    };
  }
  template< class Key, class Value, bool IsConst >
  struct Iterator;

  template< class Key, class Value, class Hash, class Equal >
  struct HashTable {
    using const_iterator = Iterator< Key, Value, true >;
    using iterator = Iterator< Key, Value, false >;

    HashTable();
    HashTable(const HashTable&);
    HashTable(HashTable&&) noexcept;
    HashTable(size_t capacity);

    ~HashTable() noexcept;

    HashTable& operator=(const HashTable&);
    HashTable& operator=(HashTable&&) noexcept;

    void add(Key&& k, Value&& val);
    void add(const Key& k, const Value& val);

    void remove(const Key& k);

    bool contains(const Key& k) const noexcept;
    void rehash(size_t slots = 0);
    double loadFactor() const noexcept;
    double tombstoneFactor() const noexcept;
    void setMaxLoadFactor(double max);
    void setMaxTombFactor(double max);

    iterator find(const Key&);
    const_iterator find(const Key&) const;

    void swap(HashTable&) noexcept;

    size_t size() const noexcept;
    size_t capacity() const noexcept;

    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    Value& operator[](const Key& k);
    const Value& operator[](const Key& k) const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
  private:
    Hash hasher_;
    Equal comparator_;
    detail::State* states_;
    detail::Slot< Key, Value >* slots_;
    size_t size_;
    size_t capacity_;
    double maxLoadFactor_;
    double maxTombFactor_;
    size_t getFreeSlot(const Key& k) const noexcept;
    size_t findIndex(const Key& k) const noexcept;
    template< class K, class V >
    void addImpl(K&& k, V&& val);
  };

  template< class Key, class Value, bool IsConst >
  struct Iterator {
    using value_type = detail::Slot< const Key, Value >;
    using reference = typename detail::conditional< IsConst, const value_type&, value_type& >::type;
    using pointer = typename detail::conditional< IsConst, const value_type*, value_type* >::type;

    template< bool OthConst >
    bool operator==(const Iterator< Key, Value, OthConst >&) const;

    template< bool OthConst >
    bool operator!=(const Iterator< Key, Value, OthConst >&) const;

    reference operator*();
    pointer operator->();

    Iterator operator++();
    Iterator operator--();

    Iterator operator++(int);
    Iterator operator--(int);

  private:
    template< class, class, class, class >
    friend struct HashTable;
    value_type* slots_;
    detail::State* states_;
    size_t i_;
    size_t cap_;
    Iterator(detail::Slot< Key, Value >* slots, detail::State* s, size_t ind, size_t cap);
  };
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::begin() noexcept
{
  size_t i = 0;
  while (i < capacity_ && states_[i] != detail::State::STORE) {
    ++i;
  }
  return iterator(slots_, states_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::begin() const noexcept
{
  size_t i = 0;
  while (i < capacity_ && states_[i] != detail::State::STORE) {
    ++i;
  }
  return const_iterator(slots_, states_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
{
  size_t i = 0;
  while (i < capacity_ && states_[i] != detail::State::STORE) {
    ++i;
  }
  return const_iterator(slots_, states_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::end() noexcept
{
  return iterator(slots_, states_, capacity_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::end() const noexcept
{
  return const_iterator(slots_, states_, capacity_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::cend() const noexcept
{
  return const_iterator(slots_, states_, capacity_, capacity_);
}

template< class K, class V, bool IsConst >
kuznetsov::Iterator< K, V, IsConst >::Iterator(detail::Slot< K, V >* slt, detail::State* s, size_t ind, size_t cap):
  slots_(reinterpret_cast< value_type* >(slt)),
  states_(s),
  i_(ind),
  cap_(cap)
{}

template< class K, class V, bool IsConst >
template< bool OthConst >
bool kuznetsov::Iterator< K, V, IsConst >::operator==(const Iterator< K, V, OthConst >& oth) const
{
  bool f = (this->slots_ + this->i_) == (oth.slots_ + oth.i_);
  return f;
}

template< class K, class V, bool IsConst >
template< bool OthConst >
bool kuznetsov::Iterator< K, V, IsConst >::operator!=(const Iterator< K, V, OthConst >& oth) const
{
  return !(*this == oth);
}

template< class K, class V, bool IsConst >
typename kuznetsov::Iterator< K, V, IsConst >::reference kuznetsov::Iterator< K, V, IsConst >::operator*()
{
  return slots_[i_];
}

template< class K, class V, bool IsConst >
typename kuznetsov::Iterator< K, V, IsConst >::pointer kuznetsov::Iterator< K, V, IsConst >::operator->()
{
  return slots_ + i_;
}

template< class K, class V, bool IsConst >
kuznetsov::Iterator< K, V, IsConst > kuznetsov::Iterator< K, V, IsConst >::operator++()
{
  ++i_;
  while (i_ < cap_ && states_[i_] != detail::State::STORE) {
    ++i_;
  }
  return *this;
}

template< class K, class V, bool IsConst >
kuznetsov::Iterator< K, V, IsConst > kuznetsov::Iterator< K, V, IsConst >::operator++(int)
{
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class K, class V, bool IsConst >
kuznetsov::Iterator< K, V, IsConst > kuznetsov::Iterator< K, V, IsConst >::operator--()
{
  if (i_ == 0) {
    return *this;
  }
  while (i_ > 0 && states_[i_] != detail::State::STORE) {
    --i_;
  }
  return *this;
}

template< class K, class V, bool IsConst >
kuznetsov::Iterator< K, V, IsConst > kuznetsov::Iterator< K, V, IsConst >::operator--(int)
{
  Iterator tmp = *this;
  --(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable():
  HashTable(16)
{}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::~HashTable() noexcept
{
  for (size_t i = 0; i < capacity_; ++i) {
    if (states_[i] == detail::State::STORE) {
      (slots_ + i)->~Slot();
    }
  }
  ::operator delete(slots_);
  delete[] states_;
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return capacity_;
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& oth):
  HashTable(oth.capacity())
{
  maxLoadFactor_ = oth.maxLoadFactor_;
  maxTombFactor_ = oth.maxTombFactor_;
  for (size_t i = 0; i < capacity_; ++i) {
    if (oth.states_[i] == detail::State::STORE) {
      new (slots_ + i) detail::Slot< Key, Value >(oth.slots_[i].first, oth.slots_[i].second);
      ++size_;
    }
    states_[i] = oth.states_[i];
  }
}

template< class K, class V, class Hash, class Equal >
kuznetsov::HashTable< K, V, Hash, Equal >::HashTable(size_t capacity):
  hasher_(Hash{}),
  comparator_(Equal{}),
  states_(nullptr),
  slots_(nullptr),
  size_(0),
  capacity_(std::pow(2, std::ceil(std::log2(capacity)))),
  maxLoadFactor_(0.75),
  maxTombFactor_(0.75)
{
  try {
    states_ = new detail::State[capacity_]{};
    slots_ = static_cast< detail::Slot< K, V >* >(::operator new(sizeof(detail::Slot< K, V >) * capacity_));
  } catch (...) {
    delete[] states_;
    ::operator delete(slots_);
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& oth) noexcept:
  hasher_(oth.hasher_),
  comparator_(oth.comparator_),
  states_(std::exchange(oth.states_, nullptr)),
  slots_(std::exchange(oth.slots_, nullptr)),
  size_(std::exchange(oth.size_, 0)),
  capacity_(std::exchange(oth.capacity_, 0)),
  maxLoadFactor_(std::exchange(oth.maxLoadFactor_, 0.0)),
  maxTombFactor_(std::exchange(oth.maxTombFactor_, 0.0))
{}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >&
  kuznetsov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& oth)
{
  if (std::addressof(oth) == this) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > cp(oth);
  swap(cp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >&
  kuznetsov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& oth) noexcept
{
  if (std::addressof(oth) == this) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > cp(std::move(oth));
  swap(cp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::swap(HashTable& oth) noexcept
{
  std::swap(hasher_, oth.hasher_);
  std::swap(comparator_, oth.comparator_);
  std::swap(states_, oth.states_);
  std::swap(slots_, oth.slots_);
  std::swap(size_, oth.size_);
  std::swap(capacity_, oth.capacity_);
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::getFreeSlot(const Key& k) const noexcept
{
  size_t hash = hasher_(k);
  size_t pos = 0;
  size_t i = 0;
  for (; i < capacity_; ++i) {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == detail::State::FREE or states_[pos] == detail::State::DELETED) {
      break;
    }
  }
  return pos;
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::findIndex(const Key& k) const noexcept
{
  size_t hash = hasher_(k);
  size_t pos = 0;
  for (size_t i = 0; i < capacity_; ++i) {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == detail::State::FREE) {
      return capacity_;
    }
    if (states_[pos] == detail::State::STORE && comparator_(k, slots_[pos].first)) {
      return pos;
    }
  }
  return capacity_;
}

template< class Key, class Value, class Hash, class Equal >
template< class K, class V >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::addImpl(K&& k, V&& val)
{
  if (size_ == capacity_) {
    throw std::logic_error("Not enough slots_");
  }
  if (contains(k)) {
    throw std::logic_error("Such key already exist");
  }
  size_t pos = getFreeSlot(k);
  new (slots_ + pos) detail::Slot< Key, Value >(std::forward< K >(k), std::forward< V >(val));
  states_[pos] = detail::State::STORE;
  ++size_;
  if (loadFactor() > maxLoadFactor_ || tombstoneFactor() > maxTombFactor_) {
    rehash();
  }
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::add(Key&& k, Value&& val)
{
  addImpl(std::forward< Key >(k), std::forward< Value >(val));
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& val)
{
  addImpl(k, val);
}

template< class Key, class Value, class Hash, class Equal >
bool kuznetsov::HashTable< Key, Value, Hash, Equal >::contains(const Key& k) const noexcept
{
  return findIndex(k) != capacity_;
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::remove(const Key& k)
{
  size_t pos = findIndex(k);
  if (pos == capacity_) {
    throw std::logic_error("Not found key");
  }
  (slots_ + pos)->~Slot();
  states_[pos] = detail::State::DELETED;
  --size_;
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  size_t newCap = !slots ? capacity_ * 2 : pow(2, ceil(log2(slots)));
  if (size_ > newCap) {
    throw std::logic_error("Count of elements more then new count of slots");
  }
  HashTable< Key, Value, Hash, Equal > newTable(newCap);
  for (size_t i = 0; i < capacity_; ++i) {
    if (states_[i] == detail::State::STORE) {
      newTable.add(slots_[i].first, slots_[i].second);
    }
  }
  swap(newTable);
}

template< class Key, class Value, class Hash, class Equal >
const Value& kuznetsov::HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
{
  size_t pos = findIndex(k);
  if (pos == capacity_) {
    throw std::out_of_range("Key not found");
  }
  return slots_[pos].second;
}

template< class Key, class Value, class Hash, class Equal >
Value& kuznetsov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
{
  try {
    return at(k);
  } catch (const std::out_of_range&) {
    this->add(k, Value{});
    return at(k);
  }
}

template< class Key, class Value, class Hash, class Equal >
const Value& kuznetsov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& k) const
{
  return at(k);
}

template< class Key, class Value, class Hash, class Equal >
Value& kuznetsov::HashTable< Key, Value, Hash, Equal >::at(const Key& k)
{
  const HashTable* cthis = this;
  return const_cast< Value& >((*cthis).at(k));
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::find(const Key& k)
{
  return iterator(slots_, states_, findIndex(k), capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
  kuznetsov::HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
{
  return const_iterator(slots_, states_, findIndex(k), capacity_);
}

template< class Key, class Value, class Hash, class Equal >
double kuznetsov::HashTable< Key, Value, Hash, Equal >::loadFactor() const noexcept
{
  return size_ / capacity_;
}
template< class Key, class Value, class Hash, class Equal >
double kuznetsov::HashTable< Key, Value, Hash, Equal >::tombstoneFactor() const noexcept
{
  double tomb = 0.0;
  for (size_t i = 0; i < capacity_; ++i) {
    if (states_[i] == detail::State::DELETED) {
      tomb += 1.0;
    }
  }
  return tomb / capacity_;
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::setMaxLoadFactor(double max)
{
  if (max <= 0) {
    throw std::logic_error("Invalid argument");
  }
  maxLoadFactor_ = max;
}
template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::setMaxTombFactor(double max)
{
  if (max <= 0) {
    throw std::logic_error("Invalid argument");
  }
  maxTombFactor_ = max;
}

#endif
