#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include "sip_hash.hpp"

namespace hvostov {

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;
  template< class Key, class Value >
  class HashTableConstIterator;

  template< class Key, class Value >
  class HashTableIterator {
  public:
    HashTableIterator();
    HashTableIterator(const HashTableIterator& it);
    HashTableIterator(HashTableIterator&& it) noexcept;
    HashTableIterator& operator=(const HashTableIterator& it);
    HashTableIterator& operator=(HashTableIterator&& it) noexcept;
    HashTableIterator& operator++();
    HashTableIterator operator++(int);
    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();
    bool operator==(const HashTableIterator& it) const;
    bool operator!=(const HashTableIterator& it) const;

  private:
    friend class HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
    template< class K, class V, class H, class E >
    friend class HashTable;
    friend class HashTableConstIterator< Key, Value >;
    void* table_;
    size_t current_idx_;

    HashTableIterator(void* table, size_t idx);
    void findNextValid();
  };

  template< class Key, class Value >
  class HashTableConstIterator {
  public:
    HashTableConstIterator();
    HashTableConstIterator(const HashTableConstIterator& it);
    HashTableConstIterator(HashTableConstIterator&& it) noexcept;
    HashTableConstIterator(const HashTableIterator< Key, Value >& it);
    HashTableConstIterator& operator=(const HashTableConstIterator& it);
    HashTableConstIterator& operator=(HashTableConstIterator&& it) noexcept;
    HashTableConstIterator& operator++();
    HashTableConstIterator operator++(int);
    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;
    bool operator==(const HashTableConstIterator& it) const;
    bool operator!=(const HashTableConstIterator& it) const;

  private:
    friend class HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
    template< class K, class V, class H, class E >
    friend class HashTable;

    const void* table_;
    size_t current_idx_;

    HashTableConstIterator(const void* table, size_t idx);
    void findNextValid();
  };

  template< class Key, class Value, class Hash = SipHash< Key >, class Equal = std::equal_to< Key > >
  class HashTable {
  public:
    using Iterator = HashTableIterator< Key, Value >;
    using ConstIterator = HashTableConstIterator< Key, Value >;

    HashTable() = default;
    explicit HashTable(size_t expected_elements);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    ~HashTable();

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    Value& operator[](const Key& k);
    const Value& operator[](const Key& k) const;

    void swap(HashTable& other) noexcept;
    void add(const Key& k, const Value& v);
    Value drop(const Key& k);
    bool has(const Key& k) const;
    void rehash(size_t new_bucket_count);
    void rehash();
    void clear() noexcept;

    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t getCapacity() const noexcept;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

  private:
    size_t bucket_size_ = 4;

    std::pair< Key, Value >* data_ = nullptr;
    size_t* bucket_sizes_ = nullptr;
    size_t bucket_count_ = 0;
    size_t overflow_cap_ = 0;
    size_t overflow_size_ = 0;
    size_t size_ = 0;

    Hash hasher_;
    Equal equal_;

    friend class HashTableIterator< Key, Value >;
    friend class HashTableConstIterator< Key, Value >;

    size_t overflowStart() const noexcept;
    size_t totalCapacity() const noexcept;

    size_t getElementIndex(const Key& k) const;
    void unsafeAdd(const Key& k, const Value& v);
    void removeByIndex(size_t index);
    void allocate(size_t num_buckets);
  };

}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::overflowStart() const noexcept
{
  return bucket_count_ * bucket_size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::totalCapacity() const noexcept
{
  return overflowStart() + overflow_cap_;
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::rehash()
{
  rehash(bucket_count_ == 0 ? 16 : bucket_count_ * 2);
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::allocate(size_t num_buckets)
{
  size_t new_bucket_count = num_buckets;
  size_t new_overflow_cap = std::max(size_t(4), num_buckets / 2);
  size_t new_total_size = new_bucket_count * bucket_size_ + new_overflow_cap;

  std::pair< Key, Value >* new_data = new std::pair< Key, Value >[new_total_size];
  try {
    size_t* new_bucket_sizes = new size_t[new_bucket_count]();

    delete[] data_;
    delete[] bucket_sizes_;

    data_ = new_data;
    bucket_sizes_ = new_bucket_sizes;
    bucket_count_ = new_bucket_count;
    overflow_cap_ = new_overflow_cap;
    size_ = 0;
    overflow_size_ = 0;

  } catch (...) {
    delete[] new_data;
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t expected_elements):
  bucket_size_(4),
  hasher_(),
  equal_()
{
  if (expected_elements == 0) {
    return;
  }
  constexpr double default_load_factor = 0.7;
  size_t needed_capacity = static_cast< size_t >(std::ceil(expected_elements / default_load_factor));
  if (needed_capacity <= bucket_size_) {
    bucket_count_ = 1;
  } else {
    bucket_count_ = needed_capacity / bucket_size_;
  }
  allocate(bucket_count_);
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
  bucket_size_(other.bucket_size_),
  bucket_count_(other.bucket_count_),
  overflow_cap_(other.overflow_cap_),
  overflow_size_(other.overflow_size_),
  size_(other.size_),
  hasher_(other.hasher_),
  equal_(other.equal_)
{
  if (other.bucket_count_ == 0) {
    data_ = nullptr;
    bucket_sizes_ = nullptr;
    return;
  }
  size_t total_size = overflowStart() + overflow_cap_;
  std::pair< Key, Value >* new_data = new std::pair< Key, Value >[total_size];
  try {
    size_t* new_bucket_sizes = new size_t[bucket_count_];
    for (size_t i = 0; i < total_size; ++i) {
      new_data[i] = other.data_[i];
    }
    for (size_t i = 0; i < bucket_count_; ++i) {
      new_bucket_sizes[i] = other.bucket_sizes_[i];
    }
    data_ = new_data;
    bucket_sizes_ = new_bucket_sizes;
  } catch (...) {
    delete[] new_data;
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
  bucket_size_(other.bucket_size_),
  data_(other.data_),
  bucket_sizes_(other.bucket_sizes_),
  bucket_count_(other.bucket_count_),
  overflow_cap_(other.overflow_cap_),
  overflow_size_(other.overflow_size_),
  size_(other.size_),
  hasher_(std::move(other.hasher_)),
  equal_(std::move(other.equal_))
{
  other.data_ = nullptr;
  other.bucket_sizes_ = nullptr;
  other.bucket_count_ = 0;
  other.overflow_cap_ = 0;
  other.overflow_size_ = 0;
  other.size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  delete[] data_;
  delete[] bucket_sizes_;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >&
hvostov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
{
  if (this == std::addressof(other)) {
    return *this;
  }
  HashTable tmp(other);
  swap(tmp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >&
hvostov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
{
  if (this == std::addressof(other)) {
    return *this;
  }
  delete[] data_;
  delete[] bucket_sizes_;

  bucket_size_ = other.bucket_size_;
  data_ = other.data_;
  bucket_sizes_ = other.bucket_sizes_;
  bucket_count_ = other.bucket_count_;
  overflow_cap_ = other.overflow_cap_;
  overflow_size_ = other.overflow_size_;
  size_ = other.size_;
  hasher_ = std::move(other.hasher_);
  equal_ = std::move(other.equal_);

  other.data_ = nullptr;
  other.bucket_sizes_ = nullptr;
  other.bucket_count_ = 0;
  other.overflow_cap_ = 0;
  other.overflow_size_ = 0;
  other.size_ = 0;
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
Value& hvostov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
{
  if (!has(k)) {
    add(k, Value{});
  }
  return at(k);
}

template< class Key, class Value, class Hash, class Equal >
const Value& hvostov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& k) const
{
  return at(k);
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
{
  std::swap(bucket_size_, other.bucket_size_);
  std::swap(data_, other.data_);
  std::swap(bucket_sizes_, other.bucket_sizes_);
  std::swap(bucket_count_, other.bucket_count_);
  std::swap(overflow_cap_, other.overflow_cap_);
  std::swap(overflow_size_, other.overflow_size_);
  std::swap(size_, other.size_);
  std::swap(hasher_, other.hasher_);
  std::swap(equal_, other.equal_);
}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::getElementIndex(const Key& k) const
{
  if (bucket_count_ == 0) {
    throw std::out_of_range("Key not found");
  }
  size_t bucket = hasher_(k) % bucket_count_;
  size_t start_idx = bucket * bucket_size_;
  size_t bucket_size = bucket_sizes_[bucket];
  for (size_t i = 0; i < bucket_size; ++i) {
    size_t idx = start_idx + i;
    if (equal_(data_[idx].first, k)) {
      return idx;
    }
  }
  size_t start = overflowStart();
  for (size_t i = 0; i < overflow_size_; ++i) {
    size_t idx = start + i;
    if (equal_(data_[idx].first, k)) {
      return idx;
    }
  }
  throw std::out_of_range("Key not found");
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::unsafeAdd(const Key& k, const Value& v)
{
  if (bucket_count_ == 0) {
    throw std::overflow_error("Hash table overflow");
  }
  size_t bucket = hasher_(k) % bucket_count_;
  size_t& bucket_size = bucket_sizes_[bucket];

  if (bucket_size < bucket_size_) {
    size_t idx = bucket * bucket_size_ + bucket_size;
    data_[idx] = std::make_pair(k, v);
    bucket_size++;
    size_++;
  } else if (overflow_size_ < overflow_cap_) {
    size_t idx = overflowStart() + overflow_size_;
    data_[idx] = std::make_pair(k, v);
    overflow_size_++;
    size_++;
  } else {
    throw std::overflow_error("Hash table overflow");
  }
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
{
  HashTable temp(*this);
  try {
    temp.data_[temp.getElementIndex(k)].second = v;
  } catch (const std::out_of_range& e) {
    temp.unsafeAdd(k, v);
  }
  swap(temp);
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::removeByIndex(size_t index)
{
  size_t last_idx;
  if (index < overflowStart()) {
    size_t bucket = index / bucket_size_;
    size_t start_idx = bucket * bucket_size_;
    last_idx = start_idx + bucket_sizes_[bucket] - 1;
    bucket_sizes_[bucket]--;
  } else {
    last_idx = overflowStart() + overflow_size_ - 1;
    overflow_size_--;
  }
  if (index != last_idx) {
    std::swap(data_[index], data_[last_idx]);
  }
  size_--;
}

template< class Key, class Value, class Hash, class Equal >
Value hvostov::HashTable< Key, Value, Hash, Equal >::drop(const Key& k)
{
  size_t idx = getElementIndex(k);
  Value val = data_[idx].second;
  HashTable temp(*this);
  temp.removeByIndex(idx);
  swap(temp);
  return val;
}

template< class Key, class Value, class Hash, class Equal >
bool hvostov::HashTable< Key, Value, Hash, Equal >::has(const Key& k) const
{
  try {
    getElementIndex(k);
    return true;
  } catch (const std::out_of_range&) {
    return false;
  }
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::rehash(size_t new_bucket_count)
{
  HashTable new_table;
  new_table.bucket_size_ = bucket_size_;
  new_table.allocate(new_bucket_count);
  for (size_t b = 0; b < bucket_count_; ++b) {
    size_t start_idx = b * bucket_size_;
    for (size_t i = 0; i < bucket_sizes_[b]; ++i) {
      size_t idx = start_idx + i;
      new_table.unsafeAdd(data_[idx].first, data_[idx].second);
    }
  }
  size_t overflow_start_idx = overflowStart();
  for (size_t i = 0; i < overflow_size_; ++i) {
    size_t idx = overflow_start_idx + i;
    new_table.unsafeAdd(data_[idx].first, data_[idx].second);
  }
  swap(new_table);
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  delete[] data_;
  delete[] bucket_sizes_;
  data_ = nullptr;
  bucket_sizes_ = nullptr;
  bucket_count_ = 0;
  overflow_cap_ = 0;
  overflow_size_ = 0;
  size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
Value& hvostov::HashTable< Key, Value, Hash, Equal >::at(const Key& k)
{
  size_t idx = getElementIndex(k);
  return data_[idx].second;
}

template< class Key, class Value, class Hash, class Equal >
const Value& hvostov::HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
{
  return const_cast< HashTable* >(this)->at(k);
}

template< class Key, class Value, class Hash, class Equal >
bool hvostov::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::getCapacity() const noexcept
{
  return totalCapacity();
}

template< class Key, class Value >
hvostov::HashTableIterator< Key, Value >::HashTableIterator():
  table_(nullptr),
  current_idx_(0)
{}

template< class Key, class Value >
hvostov::HashTableIterator< Key, Value >::HashTableIterator(void* table, size_t idx):
  table_(table),
  current_idx_(idx)
{
  if (table_) {
    findNextValid();
  }
}

template< class Key, class Value >
hvostov::HashTableIterator< Key, Value >::HashTableIterator(const HashTableIterator& it):
  table_(it.table_),
  current_idx_(it.current_idx_)
{}

template< class Key, class Value >
hvostov::HashTableIterator< Key, Value >::HashTableIterator(HashTableIterator&& it) noexcept:
  table_(it.table_),
  current_idx_(it.current_idx_)
{
  it.table_ = nullptr;
  it.current_idx_ = 0;
}

template< class Key, class Value >
hvostov::HashTableIterator< Key, Value >&
hvostov::HashTableIterator< Key, Value >::operator=(const HashTableIterator& it)
{
  if (this != &it) {
    table_ = it.table_;
    current_idx_ = it.current_idx_;
  }
  return *this;
}

template< class Key, class Value >
hvostov::HashTableIterator< Key, Value >&
hvostov::HashTableIterator< Key, Value >::operator=(HashTableIterator&& it) noexcept
{
  if (this != &it) {
    table_ = it.table_;
    current_idx_ = it.current_idx_;
    it.table_ = nullptr;
    it.current_idx_ = 0;
  }
  return *this;
}

template< class Key, class Value >
void hvostov::HashTableIterator< Key, Value >::findNextValid()
{
  if (!table_) {
    current_idx_ = 0;
    return;
  }
  using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
  TableType* t = static_cast< TableType* >(table_);

  if (t->size_ == 0) {
    current_idx_ = t->totalCapacity();
    return;
  }
  size_t overflow_start = t->overflowStart();
  size_t total = t->totalCapacity();

  while (current_idx_ < total) {
    if (current_idx_ < overflow_start) {
      size_t bucket = current_idx_ / t->bucket_size_;
      size_t pos_in_bucket = current_idx_ % t->bucket_size_;
      if (pos_in_bucket < t->bucket_sizes_[bucket]) {
        return;
      }
      current_idx_ = (bucket + 1) * t->bucket_size_;
    } else {
      if (current_idx_ < overflow_start + t->overflow_size_) {
        return;
      }
      break;
    }
  }
  current_idx_ = total;
}

template< class Key, class Value >
typename hvostov::HashTableIterator< Key, Value >& hvostov::HashTableIterator< Key, Value >::operator++()
{
  if (table_) {
    using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
    TableType* t = static_cast< TableType* >(table_);
    if (current_idx_ < t->totalCapacity()) {
      current_idx_++;
      findNextValid();
    }
  }
  return *this;
}

template< class Key, class Value >
hvostov::HashTableIterator< Key, Value > hvostov::HashTableIterator< Key, Value >::operator++(int)
{
  HashTableIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value >
std::pair< const Key, Value >& hvostov::HashTableIterator< Key, Value >::operator*()
{
  using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
  TableType* t = static_cast< TableType* >(table_);
  return reinterpret_cast< std::pair< const Key, Value >& >(t->data_[current_idx_]);
}

template< class Key, class Value >
std::pair< const Key, Value >* hvostov::HashTableIterator< Key, Value >::operator->()
{
  using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
  TableType* t = static_cast< TableType* >(table_);
  return reinterpret_cast< std::pair< const Key, Value >* >(&t->data_[current_idx_]);
}

template< class Key, class Value >
bool hvostov::HashTableIterator< Key, Value >::operator==(const HashTableIterator& it) const
{
  return table_ == it.table_ && current_idx_ == it.current_idx_;
}

template< class Key, class Value >
bool hvostov::HashTableIterator< Key, Value >::operator!=(const HashTableIterator& it) const
{
  return !(*this == it);
}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >::HashTableConstIterator():
  table_(nullptr),
  current_idx_(0)
{}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >::HashTableConstIterator(const void* table, size_t idx):
  table_(table),
  current_idx_(idx)
{
  if (table_) {
    findNextValid();
  }
}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >::HashTableConstIterator(const HashTableConstIterator& it):
  table_(it.table_),
  current_idx_(it.current_idx_)
{}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >::HashTableConstIterator(HashTableConstIterator&& it) noexcept:
  table_(it.table_),
  current_idx_(it.current_idx_)
{
  it.table_ = nullptr;
  it.current_idx_ = 0;
}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >::HashTableConstIterator(const HashTableIterator< Key, Value >& it):
  table_(it.table_),
  current_idx_(it.current_idx_)
{}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >&
hvostov::HashTableConstIterator< Key, Value >::operator=(const HashTableConstIterator& it)
{
  if (this != &it) {
    table_ = it.table_;
    current_idx_ = it.current_idx_;
  }
  return *this;
}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >&
hvostov::HashTableConstIterator< Key, Value >::operator=(HashTableConstIterator&& it) noexcept
{
  if (this != &it) {
    table_ = it.table_;
    current_idx_ = it.current_idx_;
    it.table_ = nullptr;
    it.current_idx_ = 0;
  }
  return *this;
}

template< class Key, class Value >
void hvostov::HashTableConstIterator< Key, Value >::findNextValid()
{
  if (!table_) {
    current_idx_ = 0;
    return;
  }

  using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
  const TableType* t = static_cast< const TableType* >(table_);

  if (t->size_ == 0) {
    current_idx_ = t->totalCapacity();
    return;
  }

  size_t overflow_start = t->overflowStart();
  size_t total = t->totalCapacity();

  while (current_idx_ < total) {
    if (current_idx_ < overflow_start) {
      size_t bucket = current_idx_ / t->bucket_size_;
      size_t pos_in_bucket = current_idx_ % t->bucket_size_;

      if (pos_in_bucket < t->bucket_sizes_[bucket]) {
        return;
      }

      current_idx_ = (bucket + 1) * t->bucket_size_;
    } else {
      if (current_idx_ < overflow_start + t->overflow_size_) {
        return;
      }
      break;
    }
  }

  current_idx_ = total;
}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value >& hvostov::HashTableConstIterator< Key, Value >::operator++()
{
  if (table_) {
    using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
    const TableType* t = static_cast< const TableType* >(table_);
    if (current_idx_ < t->totalCapacity()) {
      current_idx_++;
      findNextValid();
    }
  }
  return *this;
}

template< class Key, class Value >
hvostov::HashTableConstIterator< Key, Value > hvostov::HashTableConstIterator< Key, Value >::operator++(int)
{
  HashTableConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value >
const std::pair< const Key, Value >& hvostov::HashTableConstIterator< Key, Value >::operator*() const
{
  using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
  const TableType* t = static_cast< const TableType* >(table_);
  return reinterpret_cast< const std::pair< const Key, Value >& >(t->data_[current_idx_]);
}

template< class Key, class Value >
const std::pair< const Key, Value >* hvostov::HashTableConstIterator< Key, Value >::operator->() const
{
  using TableType = HashTable< Key, Value, SipHash< Key >, std::equal_to< Key > >;
  const TableType* t = static_cast< const TableType* >(table_);
  return reinterpret_cast< const std::pair< const Key, Value >* >(&t->data_[current_idx_]);
}

template< class Key, class Value >
bool hvostov::HashTableConstIterator< Key, Value >::operator==(const HashTableConstIterator& it) const
{
  return table_ == it.table_ && current_idx_ == it.current_idx_;
}

template< class Key, class Value >
bool hvostov::HashTableConstIterator< Key, Value >::operator!=(const HashTableConstIterator& it) const
{
  return !(*this == it);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::Iterator hvostov::HashTable< Key, Value, Hash, Equal >::begin()
{
  return Iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::Iterator hvostov::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iterator(this, totalCapacity());
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator
hvostov::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return ConstIterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator
hvostov::HashTable< Key, Value, Hash, Equal >::end() const
{
  return ConstIterator(this, totalCapacity());
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator
hvostov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return ConstIterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator
hvostov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return ConstIterator(this, totalCapacity());
}

#endif
