#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace zharov
{
  namespace detail
  {
    enum class State
    {
      EMPTY,
      OCCUPIED,
      TOMBSTONE
    };
  }

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class Iter
  {
  public:
    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();
    Iter& operator++();
    Iter operator++(int);
    Iter& operator--();
    Iter operator--(int);
    bool operator==(const Iter& it) const noexcept;
    bool operator!=(const Iter& it) const noexcept;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;
    detail::State* states_;
    std::pair< const Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    Iter(detail::State* states, std::pair< const Key, Value >* slots, size_t curr, size_t capacity);
  };

  template< class Key, class Value, class Hash, class Equal >
  class CIter
  {
  public:
    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;
    CIter& operator++();
    CIter operator++(int);
    CIter& operator--();
    CIter operator--(int);
    bool operator==(const CIter& it) const noexcept;
    bool operator!=(const CIter& it) const noexcept;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;
    detail::State* states_;
    std::pair< const Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    CIter(detail::State* states,
      std::pair< const Key, Value >* slots,
      size_t curr,
      size_t capacity);
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    using iterator = Iter< Key, Value, Hash, Equal >;
    using const_iterator = CIter< Key, Value, Hash, Equal >;

    HashTable();
    HashTable(const HashTable& table);
    HashTable(HashTable&& table) noexcept;
    HashTable(size_t capacity);
    ~HashTable();

    HashTable& operator=(const HashTable& table);
    HashTable& operator=(HashTable&& table) noexcept;
    Value& operator[](const Key& k);

    void add(const Key& k, const Value& v);
    void add(Key&& k, Value&& v);
    void remove(const Key& k);
    bool contains(const Key& k) const;
    void rehash(size_t slots = 0);
    void swap(HashTable& table) noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    double loadFactor() const noexcept;
    double tombstoneFactor() const noexcept;
    double maxLoadFactor() const noexcept;
    void maxLoadFactor(double mlf);
    double maxTombstoneFactor() const noexcept;
    void maxTombstoneFactor(double mtf);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    iterator find(const Key& k);
    const_iterator find(const Key& k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    Hash hasher_;
    Equal comparator_;
    detail::State* states_;
    std::pair< const Key, Value >* slots_;
    size_t capacity_;
    size_t size_;
    size_t tombstones_;
    double maxLoadFactor_;
    double maxTombstoneFactor_;

    void destroyAt(size_t pos);

    template< class K, class V >
    void addImpl(K&& k, V&& v);
  };
}

template< class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::HashTable():
  HashTable(16)
{}

template< class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t capacity):
  hasher_(Hash{}),
  comparator_(Equal{}),
  states_(nullptr),
  slots_(nullptr),
  capacity_(std::pow(2, std::ceil(std::log2(capacity)))),
  size_(0),
  tombstones_(0),
  maxLoadFactor_(0.75),
  maxTombstoneFactor_(0.25)
{
  try
  {
    states_ = new zharov::detail::State[capacity_]{};
    slots_ = static_cast< std::pair< const Key, Value >* >(
      ::operator new(sizeof(std::pair< const Key, Value >) * capacity_));
  }
  catch (...)
  {
    delete[] states_;
    ::operator delete(slots_);
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& table):
  HashTable(table.capacity_)
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (table.states_[i] == zharov::detail::State::OCCUPIED)
    {
      new (slots_ + i) std::pair< const Key, Value >(table.slots_[i].first, table.slots_[i].second);
      ++size_;
    }
    states_[i] = table.states_[i];
  }
}

template< class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& table) noexcept:
  hasher_(table.hasher_),
  comparator_(table.comparator_),
  states_(std::exchange(table.states_, nullptr)),
  slots_(std::exchange(table.slots_, nullptr)),
  capacity_(std::exchange(table.capacity_, 0)),
  size_(std::exchange(table.size_, 0)),
  tombstones_(std::exchange(table.tombstones_, 0)),
  maxLoadFactor_(table.maxLoadFactor_),
  maxTombstoneFactor_(table.maxTombstoneFactor_)
{}

template< class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (states_[i] == zharov::detail::State::OCCUPIED)
    {
      destroyAt(i);
    }
  }
  delete[] states_;
  ::operator delete(slots_);
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::swap(HashTable& table) noexcept
{
  std::swap(table.hasher_, hasher_);
  std::swap(table.comparator_, comparator_);
  std::swap(table.states_, states_);
  std::swap(table.slots_, slots_);
  std::swap(table.capacity_, capacity_);
  std::swap(table.size_, size_);
  std::swap(table.tombstones_, tombstones_);
  std::swap(table.maxLoadFactor_, maxLoadFactor_);
  std::swap(table.maxTombstoneFactor_, maxTombstoneFactor_);
}

template< class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >&
  zharov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& table)
{
  if (std::addressof(table) == this)
  {
    return *this;
  }

  HashTable< Key, Value, Hash, Equal > cp(table);
  swap(cp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >&
  zharov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& table) noexcept
{
  if (std::addressof(table) == this)
  {
    return *this;
  }

  HashTable< Key, Value, Hash, Equal > cp(std::move(table));
  swap(cp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
size_t zharov::HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return capacity_;
}

template< class Key, class Value, class Hash, class Equal >
size_t zharov::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
double zharov::HashTable< Key, Value, Hash, Equal >::loadFactor() const noexcept
{
  return static_cast< double >(size_) / capacity_;
}

template< class Key, class Value, class Hash, class Equal >
double zharov::HashTable< Key, Value, Hash, Equal >::tombstoneFactor() const noexcept
{
  return static_cast< double >(tombstones_) / capacity_;
}

template< class Key, class Value, class Hash, class Equal >
double zharov::HashTable< Key, Value, Hash, Equal >::maxLoadFactor() const noexcept
{
  return maxLoadFactor_;
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::maxLoadFactor(double mlf)
{
  if (mlf <= 0.0 || mlf >= 1.0)
  {
    throw std::logic_error("maxLoadFactor must be in (0, 1)");
  }
  maxLoadFactor_ = mlf;
}

template< class Key, class Value, class Hash, class Equal >
double zharov::HashTable< Key, Value, Hash, Equal >::maxTombstoneFactor() const noexcept
{
  return maxTombstoneFactor_;
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::maxTombstoneFactor(double mtf)
{
  if (mtf <= 0.0 || mtf >= 1.0)
  {
    throw std::logic_error("maxTombstoneFactor must be in (0, 1)");
  }
  maxTombstoneFactor_ = mtf;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::HashTable< Key, Value, Hash, Equal >::contains(const Key& k) const
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == zharov::detail::State::OCCUPIED && comparator_(slots_[pos].first, k))
    {
      return true;
    }
    else if (states_[pos] == zharov::detail::State::EMPTY)
    {
      return false;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
{
  addImpl(k, v);
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::add(Key&& k, Value&& v)
{
  addImpl(std::forward< Key >(k), std::forward< Value >(v));
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::destroyAt(size_t pos)
{
  using pair_t = std::pair< const Key, Value >;
  (slots_ + pos)->~pair_t();
}

template< class Key, class Value, class Hash, class Equal >
template< class K, class V >
void zharov::HashTable< Key, Value, Hash, Equal >::addImpl(K&& k, V&& v)
{
  if (loadFactor() >= maxLoadFactor_ || tombstoneFactor() > maxTombstoneFactor_)
  {
    rehash();
  }
  if (contains(k))
  {
    throw std::logic_error("Key already exist");
  }
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == zharov::detail::State::EMPTY ||
      states_[pos] == zharov::detail::State::TOMBSTONE)
    {
      break;
    }
  }
  new (slots_ + pos) std::pair< const Key, Value >(std::forward< K >(k), std::forward< V >(v));
  states_[pos] = zharov::detail::State::OCCUPIED;
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::remove(const Key& k)
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == zharov::detail::State::EMPTY)
    {
      throw std::logic_error("Key not found");
    }
    else if (states_[pos] == zharov::detail::State::OCCUPIED && comparator_(k, slots_[pos].first))
    {
      destroyAt(pos);
      states_[pos] = zharov::detail::State::TOMBSTONE;
      --size_;
      ++tombstones_;
      return;
    }
  }
  throw std::logic_error("Key not found");
}

template< class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  slots = slots ? std::pow(2, std::ceil(std::log2(slots))) : capacity_ * 2;
  if (slots < size_)
  {
    throw std::logic_error("Rehash size too small");
  }
  HashTable< Key, Value, Hash, Equal > new_table(slots);
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (states_[i] == zharov::detail::State::OCCUPIED)
    {
      new_table.add(const_cast< Key&& >(std::move(slots_[i].first)), std::move(slots_[i].second));
    }
  }
  swap(new_table);
}

template< class Key, class Value, class Hash, class Equal >
Value& zharov::HashTable< Key, Value, Hash, Equal >::at(const Key& k)
{
  const HashTable* const_table = this;
  return const_cast< Value& >((*const_table).at(k));
}

template< class Key, class Value, class Hash, class Equal >
const Value& zharov::HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
{
  const_iterator it = find(k);
  if (it == cend())
  {
    throw std::logic_error("Key not found");
  }
  return it->second;
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::HashTable< Key, Value, Hash, Equal >::iterator
  zharov::HashTable< Key, Value, Hash, Equal >::find(const Key& k)
{
  const HashTable* const_table = this;
  const_iterator cit = const_table->find(k);
  return iterator(states_, slots_, cit.curr_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename zharov::HashTable< Key, Value, Hash, Equal >::const_iterator
  zharov::HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
{
  size_t hash = hasher_(k);
  for (size_t i = 0; i < capacity_; ++i)
  {
    size_t pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == zharov::detail::State::OCCUPIED && comparator_(slots_[pos].first, k))
    {
      return const_iterator(states_, slots_, pos, capacity_);
    }
    else if (states_[pos] == zharov::detail::State::EMPTY)
    {
      break;
    }
  }
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
Value& zharov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
{
  if (!contains(k))
  {
    add(k, Value{});
  }
  return at(k);
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal >::Iter(zharov::detail::State* states,
  std::pair< const Key, Value >* slots,
  size_t curr,
  size_t capacity):
  states_(states),
  slots_(slots),
  curr_(curr),
  capacity_(capacity)
{}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >::CIter(zharov::detail::State* states,
  std::pair< const Key, Value >* slots,
  size_t curr,
  size_t capacity):
  states_(states),
  slots_(slots),
  curr_(curr),
  capacity_(capacity)
{}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key, Value >& zharov::Iter< Key, Value, Hash, Equal >::operator*()
{
  return slots_[curr_];
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key, Value >* zharov::Iter< Key, Value, Hash, Equal >::operator->()
{
  return slots_ + curr_;
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal >& zharov::Iter< Key, Value, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && states_[curr_] != zharov::detail::State::OCCUPIED)
  {
    ++curr_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::Iter< Key, Value, Hash, Equal >::operator++(int)
{
  Iter< Key, Value, Hash, Equal > temp = *this;
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal >& zharov::Iter< Key, Value, Hash, Equal >::operator--()
{
  --curr_;
  while (curr_ != 0 && states_[curr_] != zharov::detail::State::OCCUPIED)
  {
    --curr_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::Iter< Key, Value, Hash, Equal >::operator--(int)
{
  Iter< Key, Value, Hash, Equal > temp = *this;
  --(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::Iter< Key, Value, Hash, Equal >::operator==(const Iter& it) const noexcept
{
  return slots_ == it.slots_ && states_ == it.states_ && curr_ == it.curr_ &&
    capacity_ == it.capacity_;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::Iter< Key, Value, Hash, Equal >::operator!=(const Iter& it) const noexcept
{
  return !(it == *this);
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< const Key, Value >& zharov::CIter< Key, Value, Hash, Equal >::operator*() const
{
  return slots_[curr_];
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< const Key, Value >* zharov::CIter< Key, Value, Hash, Equal >::operator->() const
{
  return slots_ + curr_;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >& zharov::CIter< Key, Value, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && states_[curr_] != zharov::detail::State::OCCUPIED)
  {
    ++curr_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::CIter< Key, Value, Hash, Equal >::operator++(int)
{
  CIter< Key, Value, Hash, Equal > temp = *this;
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >& zharov::CIter< Key, Value, Hash, Equal >::operator--()
{
  --curr_;
  while (curr_ != 0 && states_[curr_] != zharov::detail::State::OCCUPIED)
  {
    --curr_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::CIter< Key, Value, Hash, Equal >::operator--(int)
{
  CIter< Key, Value, Hash, Equal > temp = *this;
  --(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::CIter< Key, Value, Hash, Equal >::operator==(const CIter& it) const noexcept
{
  return slots_ == it.slots_ && states_ == it.states_ && curr_ == it.curr_ &&
    capacity_ == it.capacity_;
}

template< class Key, class Value, class Hash, class Equal >
bool zharov::CIter< Key, Value, Hash, Equal >::operator!=(const CIter& it) const noexcept
{
  return !(it == *this);
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::begin()
{
  Iter< Key, Value, Hash, Equal > it(states_, slots_, 0, capacity_);
  while (it.curr_ < it.capacity_ && it.states_[it.curr_] != zharov::detail::State::OCCUPIED)
  {
    ++it.curr_;
  }
  return it;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::begin() const
{
  CIter< Key, Value, Hash, Equal > it(states_, slots_, 0, capacity_);
  while (it.curr_ < it.capacity_ && it.states_[it.curr_] != zharov::detail::State::OCCUPIED)
  {
    ++it.curr_;
  }
  return it;
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >
  zharov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  CIter< Key, Value, Hash, Equal > it(states_, slots_, 0, capacity_);
  while (it.curr_ < it.capacity_ && it.states_[it.curr_] != zharov::detail::State::OCCUPIED)
  {
    ++it.curr_;
  }
  return it;
}

template< class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iter< Key, Value, Hash, Equal >(states_, slots_, capacity_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::end() const
{
  return CIter< Key, Value, Hash, Equal >(states_, slots_, capacity_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return CIter< Key, Value, Hash, Equal >(states_, slots_, capacity_, capacity_);
}

#endif
