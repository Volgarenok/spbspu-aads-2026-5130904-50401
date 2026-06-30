#ifndef Hash_TABLE_HPP
#define Hash_TABLE_HPP

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

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator {
  public:
    HashTableIterator();
    HashTableIterator(const HashTableIterator& it) = default;
    HashTableIterator(HashTableIterator&& it) noexcept = default;
    HashTableIterator& operator=(const HashTableIterator& it) = default;
    HashTableIterator& operator=(HashTableIterator&& it) noexcept = default;
    HashTableIterator& operator++();
    HashTableIterator operator++(int);
    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();
    bool operator==(const HashTableIterator& it) const;
    bool operator!=(const HashTableIterator& it) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
    HashTable< Key, Value, Hash, Equal >* table_;
    size_t current_idx_;

    HashTableIterator(HashTable< Key, Value, Hash, Equal >* table, size_t idx);
    void findNextValid();
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator {
  public:
    HashTableConstIterator();
    HashTableConstIterator(const HashTableConstIterator& it) = default;
    HashTableConstIterator(HashTableConstIterator&& it) noexcept = default;
    HashTableConstIterator(const HashTableIterator< Key, Value, Hash, Equal >& it);
    HashTableConstIterator& operator=(const HashTableConstIterator& it) = default;
    HashTableConstIterator& operator=(HashTableConstIterator&& it) noexcept = default;
    HashTableConstIterator& operator++();
    HashTableConstIterator operator++(int);
    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;
    bool operator==(const HashTableConstIterator& it) const;
    bool operator!=(const HashTableConstIterator& it) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;

    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t current_idx_;

    HashTableConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t idx);
    void findNextValid();
  };

  template< class Key, class Value, class Hash = SipHash< Key >, class Equal = std::equal_to< Key > >
  class HashTable {
  public:
    using Iterator = HashTableIterator< Key, Value, Hash, Equal >;
    using ConstIterator = HashTableConstIterator< Key, Value, Hash, Equal >;
    using BucketsUpdater = size_t (*)(size_t);
    using BucketSizeUpdater = size_t (*)(size_t);

    HashTable() = default;
    explicit HashTable(size_t expected_elements);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    ~HashTable();

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    Value& operator[](const Key& k);

    void swap(HashTable& other) noexcept;

    template< class K, class V >
    void add(K&& k, V&& v);

    bool remove(const Key& k);
    bool contains(const Key& k) const;
    Iterator find(const Key& k);
    ConstIterator find(const Key& k) const;
    void rehash();
    void rehash(size_t new_bucket_count);
    void rehash(size_t new_bucket_count, size_t new_bucket_size);
    void clear() noexcept;

    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t getCapacity() const noexcept;

    double loadFactor() const noexcept;
    double averageBucketSize() const noexcept;
    size_t overflowSize() const noexcept;
    size_t maxBucketSize() const noexcept;
    void maxLoadFactor(double mlf);
    double maxLoadFactor() const noexcept;
    void maxAverageBucketSize(double max);
    double maxAverageBucketSize() const noexcept;
    void maxOverflowSize(size_t mos);
    size_t maxOverflowSize() const noexcept;
    void setBucketsUpdater(BucketsUpdater upd);
    void setBucketSizeUpdater(BucketSizeUpdater upd);

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

    double maxLoadFactor_ = 0.7;
    double maxAverageBucketSize_ = 4.0;
    size_t maxOverflowSize_ = 8;
    BucketsUpdater bucketsUpdater_ = nullptr;
    BucketSizeUpdater bucketSizeUpdater_ = nullptr;

    Hash Hasher_;
    Equal equal_;

    friend class HashTableIterator< Key, Value, Hash, Equal >;
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;

    size_t overflowStart() const noexcept;
    size_t totalCapacity() const noexcept;

    Iterator findIndex(const Key& k);
    ConstIterator findIndex(const Key& k) const;

    template< class K, class V >
    void unsafeAdd(K&& k, V&& v);

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
  size_t new_bucket_count = bucket_count_ == 0 ? 16 : bucket_count_ * 2;
  if (bucketsUpdater_) {
    new_bucket_count = bucketsUpdater_(bucket_count_);
  }
  size_t new_bucket_size = bucket_size_;
  if (bucketSizeUpdater_) {
    new_bucket_size = bucketSizeUpdater_(bucket_size_);
  }
  rehash(new_bucket_count, new_bucket_size);
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::rehash(size_t new_bucket_count)
{
  rehash(new_bucket_count, bucket_size_);
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::rehash(size_t new_bucket_count, size_t new_bucket_size)
{
  HashTable new_table;
  new_table.bucket_size_ = new_bucket_size;
  new_table.maxLoadFactor_ = maxLoadFactor_;
  new_table.maxAverageBucketSize_ = maxAverageBucketSize_;
  new_table.maxOverflowSize_ = maxOverflowSize_;
  new_table.bucketsUpdater_ = bucketsUpdater_;
  new_table.bucketSizeUpdater_ = bucketSizeUpdater_;
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
  Hasher_(),
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
  maxLoadFactor_(other.maxLoadFactor_),
  maxAverageBucketSize_(other.maxAverageBucketSize_),
  maxOverflowSize_(other.maxOverflowSize_),
  bucketsUpdater_(other.bucketsUpdater_),
  bucketSizeUpdater_(other.bucketSizeUpdater_),
  Hasher_(other.Hasher_),
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
  data_(std::exchange(other.data_, nullptr)),
  bucket_sizes_(std::exchange(other.bucket_sizes_, nullptr)),
  bucket_count_(std::exchange(other.bucket_count_, 0)),
  overflow_cap_(std::exchange(other.overflow_cap_, 0)),
  overflow_size_(std::exchange(other.overflow_size_, 0)),
  size_(std::exchange(other.size_, 0)),
  maxLoadFactor_(other.maxLoadFactor_),
  maxAverageBucketSize_(other.maxAverageBucketSize_),
  maxOverflowSize_(other.maxOverflowSize_),
  bucketsUpdater_(other.bucketsUpdater_),
  bucketSizeUpdater_(other.bucketSizeUpdater_),
  Hasher_(std::move(other.Hasher_)),
  equal_(std::move(other.equal_))
{}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  delete[] data_;
  delete[] bucket_sizes_;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >& hvostov::HashTable< Key, Value, Hash, Equal >::operator=(
  const HashTable& other)
{
  if (this == std::addressof(other)) {
    return *this;
  }
  HashTable tmp(other);
  swap(tmp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTable< Key, Value, Hash, Equal >& hvostov::HashTable< Key, Value, Hash, Equal >::operator=(
  HashTable&& other) noexcept
{
  if (this == std::addressof(other)) {
    return *this;
  }
  HashTable tmp(std::move(other));
  swap(tmp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
Value& hvostov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
{
  auto it = findIndex(k);
  if (it == end()) {
    HashTable temp(*this);
    temp.unsafeAdd(k, Value{});
    swap(temp);
    it = findIndex(k);
  }
  return it->second;
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
  std::swap(maxLoadFactor_, other.maxLoadFactor_);
  std::swap(maxAverageBucketSize_, other.maxAverageBucketSize_);
  std::swap(maxOverflowSize_, other.maxOverflowSize_);
  std::swap(bucketsUpdater_, other.bucketsUpdater_);
  std::swap(bucketSizeUpdater_, other.bucketSizeUpdater_);
  std::swap(Hasher_, other.Hasher_);
  std::swap(equal_, other.equal_);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::Iterator hvostov::HashTable< Key, Value, Hash, Equal >::findIndex(
  const Key& k)
{
  if (bucket_count_ == 0) {
    return end();
  }
  size_t bucket = Hasher_(k) % bucket_count_;
  size_t start_idx = bucket * bucket_size_;
  size_t bucket_size = bucket_sizes_[bucket];
  for (size_t i = 0; i < bucket_size; ++i) {
    size_t idx = start_idx + i;
    if (equal_(data_[idx].first, k)) {
      return Iterator(this, idx);
    }
  }
  size_t start = overflowStart();
  for (size_t i = 0; i < overflow_size_; ++i) {
    size_t idx = start + i;
    if (equal_(data_[idx].first, k)) {
      return Iterator(this, idx);
    }
  }
  return end();
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator hvostov::HashTable< Key, Value, Hash, Equal >::
  findIndex(const Key& k) const
{
  if (bucket_count_ == 0) {
    return cend();
  }
  size_t bucket = Hasher_(k) % bucket_count_;
  size_t start_idx = bucket * bucket_size_;
  size_t bucket_size = bucket_sizes_[bucket];
  for (size_t i = 0; i < bucket_size; ++i) {
    size_t idx = start_idx + i;
    if (equal_(data_[idx].first, k)) {
      return ConstIterator(this, idx);
    }
  }
  size_t start = overflowStart();
  for (size_t i = 0; i < overflow_size_; ++i) {
    size_t idx = start + i;
    if (equal_(data_[idx].first, k)) {
      return ConstIterator(this, idx);
    }
  }
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
template< class K, class V >
void hvostov::HashTable< Key, Value, Hash, Equal >::unsafeAdd(K&& k, V&& v)
{
  if (
    bucket_count_ == 0 || overflow_size_ >= maxOverflowSize_ || loadFactor() >= maxLoadFactor_ ||
    averageBucketSize() >= maxAverageBucketSize_) {
    rehash();
  }
  size_t bucket = Hasher_(k) % bucket_count_;
  size_t& bucket_size = bucket_sizes_[bucket];

  if (bucket_size < bucket_size_) {
    size_t idx = bucket * bucket_size_ + bucket_size;
    data_[idx] = std::make_pair(std::forward< K >(k), std::forward< V >(v));
    bucket_size++;
    size_++;
  } else if (overflow_size_ < overflow_cap_) {
    size_t idx = overflowStart() + overflow_size_;
    data_[idx] = std::make_pair(std::forward< K >(k), std::forward< V >(v));
    overflow_size_++;
    size_++;
  } else {
    throw std::overflow_error("Hash table overflow");
  }
}

template< class Key, class Value, class Hash, class Equal >
template< class K, class V >
void hvostov::HashTable< Key, Value, Hash, Equal >::add(K&& k, V&& v)
{
  HashTable temp(*this);
  auto it = temp.findIndex(k);
  if (it != temp.end()) {
    it->second = std::forward< V >(v);
  } else {
    temp.unsafeAdd(std::forward< K >(k), std::forward< V >(v));
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
bool hvostov::HashTable< Key, Value, Hash, Equal >::remove(const Key& k)
{
  auto it = findIndex(k);
  if (it == end()) {
    return false;
  }
  HashTable temp(*this);
  temp.removeByIndex(it.current_idx_);
  swap(temp);
  return true;
}

template< class Key, class Value, class Hash, class Equal >
bool hvostov::HashTable< Key, Value, Hash, Equal >::contains(const Key& k) const
{
  return findIndex(k) != cend();
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::Iterator hvostov::HashTable< Key, Value, Hash, Equal >::find(
  const Key& k)
{
  return findIndex(k);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator hvostov::HashTable< Key, Value, Hash, Equal >::find(
  const Key& k) const
{
  return findIndex(k);
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
  auto it = findIndex(k);
  if (it == end()) {
    throw std::out_of_range("Key not found");
  }
  return it->second;
}

template< class Key, class Value, class Hash, class Equal >
const Value& hvostov::HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
{
  auto it = findIndex(k);
  if (it == cend()) {
    throw std::out_of_range("Key not found");
  }
  return it->second;
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

template< class Key, class Value, class Hash, class Equal >
double hvostov::HashTable< Key, Value, Hash, Equal >::loadFactor() const noexcept
{
  if (bucket_count_ == 0) {
    return 0.0;
  }
  return static_cast< double >(size_) / (bucket_count_ * bucket_size_);
}

template< class Key, class Value, class Hash, class Equal >
double hvostov::HashTable< Key, Value, Hash, Equal >::averageBucketSize() const noexcept
{
  if (bucket_count_ == 0) {
    return 0.0;
  }
  return static_cast< double >(size_) / bucket_count_;
}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::overflowSize() const noexcept
{
  return overflow_size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::maxBucketSize() const noexcept
{
  size_t max_size = 0;
  for (size_t i = 0; i < bucket_count_; ++i) {
    if (bucket_sizes_[i] > max_size) {
      max_size = bucket_sizes_[i];
    }
  }
  return max_size;
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::maxLoadFactor(double mlf)
{
  if (mlf <= 0.0 || mlf >= 1.0) {
    throw std::logic_error("maxLoadFactor must be in (0, 1)");
  }
  maxLoadFactor_ = mlf;
}

template< class Key, class Value, class Hash, class Equal >
double hvostov::HashTable< Key, Value, Hash, Equal >::maxLoadFactor() const noexcept
{
  return maxLoadFactor_;
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::maxAverageBucketSize(double max)
{
  maxAverageBucketSize_ = max;
}

template< class Key, class Value, class Hash, class Equal >
double hvostov::HashTable< Key, Value, Hash, Equal >::maxAverageBucketSize() const noexcept
{
  return maxAverageBucketSize_;
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::maxOverflowSize(size_t mos)
{
  maxOverflowSize_ = mos;
}

template< class Key, class Value, class Hash, class Equal >
size_t hvostov::HashTable< Key, Value, Hash, Equal >::maxOverflowSize() const noexcept
{
  return maxOverflowSize_;
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::setBucketsUpdater(BucketsUpdater upd)
{
  bucketsUpdater_ = upd;
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTable< Key, Value, Hash, Equal >::setBucketSizeUpdater(BucketSizeUpdater upd)
{
  bucketSizeUpdater_ = upd;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator():
  table_(nullptr),
  current_idx_(0)
{}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator(
  HashTable< Key, Value, Hash, Equal >* table,
  size_t idx):
  table_(table),
  current_idx_(idx)
{
  if (table_) {
    findNextValid();
  }
}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTableIterator< Key, Value, Hash, Equal >::findNextValid()
{
  if (!table_) {
    current_idx_ = 0;
    return;
  }

  if (table_->size_ == 0) {
    current_idx_ = table_->totalCapacity();
    return;
  }

  size_t overflow_start = table_->overflowStart();
  size_t total = table_->totalCapacity();

  while (current_idx_ < total) {
    if (current_idx_ < overflow_start) {
      size_t bucket = current_idx_ / table_->bucket_size_;
      size_t pos_in_bucket = current_idx_ % table_->bucket_size_;
      if (pos_in_bucket < table_->bucket_sizes_[bucket]) {
        return;
      }
      current_idx_ = (bucket + 1) * table_->bucket_size_;
    } else {
      if (current_idx_ < overflow_start + table_->overflow_size_) {
        return;
      }
      break;
    }
  }
  current_idx_ = total;
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTableIterator< Key, Value, Hash, Equal >& hvostov::HashTableIterator< Key, Value, Hash, Equal >::
operator++()
{
  if (table_) {
    if (current_idx_ < table_->totalCapacity()) {
      current_idx_++;
      findNextValid();
    }
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableIterator< Key, Value, Hash, Equal > hvostov::HashTableIterator< Key, Value, Hash, Equal >::operator++(
  int)
{
  HashTableIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key, Value >& hvostov::HashTableIterator< Key, Value, Hash, Equal >::operator*()
{
  return reinterpret_cast< std::pair< const Key, Value >& >(table_->data_[current_idx_]);
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key, Value >* hvostov::HashTableIterator< Key, Value, Hash, Equal >::operator->()
{
  return reinterpret_cast< std::pair< const Key, Value >* >(&table_->data_[current_idx_]);
}

template< class Key, class Value, class Hash, class Equal >
bool hvostov::HashTableIterator< Key, Value, Hash, Equal >::operator==(const HashTableIterator& it) const
{
  return table_ == it.table_ && current_idx_ == it.current_idx_;
}

template< class Key, class Value, class Hash, class Equal >
bool hvostov::HashTableIterator< Key, Value, Hash, Equal >::operator!=(const HashTableIterator& it) const
{
  return !(*this == it);
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator():
  table_(nullptr),
  current_idx_(0)
{}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
  const HashTable< Key, Value, Hash, Equal >* table,
  size_t idx):
  table_(table),
  current_idx_(idx)
{
  if (table_) {
    findNextValid();
  }
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
  const HashTableIterator< Key, Value, Hash, Equal >& it):
  table_(it.table_),
  current_idx_(it.current_idx_)
{}

template< class Key, class Value, class Hash, class Equal >
void hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::findNextValid()
{
  if (!table_) {
    current_idx_ = 0;
    return;
  }

  if (table_->size_ == 0) {
    current_idx_ = table_->totalCapacity();
    return;
  }

  size_t overflow_start = table_->overflowStart();
  size_t total = table_->totalCapacity();

  while (current_idx_ < total) {
    if (current_idx_ < overflow_start) {
      size_t bucket = current_idx_ / table_->bucket_size_;
      size_t pos_in_bucket = current_idx_ % table_->bucket_size_;

      if (pos_in_bucket < table_->bucket_sizes_[bucket]) {
        return;
      }

      current_idx_ = (bucket + 1) * table_->bucket_size_;
    } else {
      if (current_idx_ < overflow_start + table_->overflow_size_) {
        return;
      }
      break;
    }
  }

  current_idx_ = total;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableConstIterator< Key, Value, Hash, Equal >& hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::
operator++()
{
  if (table_) {
    if (current_idx_ < table_->totalCapacity()) {
      current_idx_++;
      findNextValid();
    }
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
hvostov::HashTableConstIterator< Key, Value, Hash, Equal > hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::
operator++(int)
{
  HashTableConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< const Key, Value >& hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::operator*() const
{
  return reinterpret_cast< const std::pair< const Key, Value >& >(table_->data_[current_idx_]);
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< const Key, Value >* hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::operator->() const
{
  return reinterpret_cast< const std::pair< const Key, Value >* >(&table_->data_[current_idx_]);
}

template< class Key, class Value, class Hash, class Equal >
bool hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::operator==(const HashTableConstIterator& it) const
{
  return table_ == it.table_ && current_idx_ == it.current_idx_;
}

template< class Key, class Value, class Hash, class Equal >
bool hvostov::HashTableConstIterator< Key, Value, Hash, Equal >::operator!=(const HashTableConstIterator& it) const
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
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator hvostov::HashTable< Key, Value, Hash, Equal >::
  begin() const
{
  return ConstIterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator hvostov::HashTable< Key, Value, Hash, Equal >::end()
  const
{
  return ConstIterator(this, totalCapacity());
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator hvostov::HashTable< Key, Value, Hash, Equal >::
  cbegin() const
{
  return ConstIterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename hvostov::HashTable< Key, Value, Hash, Equal >::ConstIterator hvostov::HashTable< Key, Value, Hash, Equal >::
  cend() const
{
  return ConstIterator(this, totalCapacity());
}

#endif
