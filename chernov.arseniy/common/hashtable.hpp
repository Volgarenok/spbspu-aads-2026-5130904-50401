#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

namespace chernov {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    using Element = std::pair< const Key, Value >;

    HashTable();
    HashTable(const HashTable & ht);
    HashTable(HashTable && ht) noexcept;
    ~HashTable();

    HashTable(size_t slots);
    HashTable(size_t num_buckets, size_t bucket_cap, size_t overflow_cap);

    HashTable & operator=(const HashTable & ht);
    HashTable & operator=(HashTable && ht) noexcept;

    void swap(HashTable & ht) noexcept;

    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k);
    void rehash(size_t slots);
    void rehash(size_t num_buckets, size_t bucket_cap, size_t overflow_cap);
  private:
    Element * data_;
    size_t * bucket_sizes_;
    size_t total_size_;

    size_t num_buckets_;
    size_t bucket_cap_;
    size_t overflow_size_;
    size_t overflow_cap_;

    Hash hasher_;
    Equal equal_;

    size_t getElementIndex(Key k);
    void removeElementByIndex(size_t index);
    void unsafeAddWithoutCheckingExisting(Key k, Value v);
    void setParamsByCountSlots(size_t slots);
  };
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable():
  data_(nullptr),
  bucket_sizes_(nullptr),
  total_size_(0),
  num_buckets_(0),
  bucket_cap_(0),
  overflow_size_(0),
  overflow_cap_(0),
  hasher_(Hash{}),
  equal_(Equal{})
{}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & ht):
  HashTable(ht.num_buckets_, ht.bucket_cap_, ht.overflow_cap_)
{
  try {
    for (size_t i = 0; i < num_buckets_; ++i) {
      for (size_t j = 0; j < ht.bucket_sizes_[i]; ++j) {
        new (data_ + (i * bucket_cap_ + j)) Element(ht.data_[i * bucket_cap_ + j]);
        ++bucket_sizes_[i];
        ++total_size_;
      }
    }
    for (size_t i = 0; i < ht.overflow_size_; ++i) {
      new (data_ + (num_buckets_ * bucket_cap_ + i)) Element(ht.data_[num_buckets_ * bucket_cap_ + i]);
      ++overflow_size_;
      ++total_size_;
    }
  } catch (...) {
    for (size_t i = 0; i < num_buckets_; ++i) {
      for (size_t j = 0; j < bucket_sizes_[i]; ++j) {
        data_[i * bucket_cap_ + j].~Element();
      }
    }
    for (size_t i = 0; i < overflow_size_; ++i) {
      data_[num_buckets_ * bucket_cap_ + i].~Element();
    }
    ::operator delete (data_);
    delete [] bucket_sizes_;
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable && ht) noexcept:
  data_(ht.data_),
  bucket_sizes_(ht.bucket_sizes_),
  total_size_(ht.total_size_),
  num_buckets_(ht.num_buckets_),
  bucket_cap_(ht.bucket_cap_),
  overflow_size_(ht.overflow_size_),
  overflow_cap_(ht.overflow_cap_),
  hasher_(ht.hasher_),
  equal_(ht.equal_)
{
  ht.data_ = nullptr;
  ht.bucket_sizes_ = nullptr;
  ht.total_size_ = 0;
  ht.num_buckets_ = 0;
  ht.bucket_cap_ = 0;
  ht.overflow_size_ = 0;
  ht.overflow_cap_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  for (size_t i = 0; i < num_buckets_; ++i) {
    for (size_t j = 0; j < bucket_sizes_[i]; ++j) {
      data_[i * bucket_cap_ + j].~Element();
    }
  }
  for (size_t i = 0; i < overflow_size_; ++i) {
    data_[num_buckets_ * bucket_cap_ + i].~Element();
  }
  ::operator delete (data_);
  delete [] bucket_sizes_;
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(nullptr),
  bucket_sizes_(nullptr),
  total_size_(0),
  num_buckets_(0),
  bucket_cap_(0),
  overflow_size_(0),
  overflow_cap_(0),
  hasher_(Hash{}),
  equal_(Equal{})
{
  constexpr size_t default_bucket_cap = 4;

  if (slots) {
    setParamsByCountSlots(slots);
    data_ = static_cast< Element * >(::operator new (sizeof(Element) * (num_buckets_ * bucket_cap_ + overflow_cap_)));
    bucket_sizes_ = new size_t[num_buckets_]{0};
  }
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t num_buckets, size_t bucket_cap, size_t overflow_cap):
  data_(nullptr),
  bucket_sizes_(nullptr),
  total_size_(0),
  num_buckets_(num_buckets),
  bucket_cap_(bucket_cap),
  overflow_size_(0),
  overflow_cap_(overflow_cap),
  hasher_(Hash{}),
  equal_(Equal{})
{
  size_t size = num_buckets_ * bucket_cap_ + overflow_cap_;
  if (size) {
    data_ = static_cast< Element * >(::operator new (sizeof(Element) * size));
    bucket_sizes_ = new size_t[num_buckets_]{0};
  }
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal > & chernov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable & ht)
{
  if (this == std::addressof(ht)) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > new_ht = ht;
  swap(new_ht);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal > & chernov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable && ht) noexcept
{
  if (this == std::addressof(ht)) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > new_ht = std::move(ht);
  swap(new_ht);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::swap(HashTable & ht) noexcept
{
  std::swap(data_, ht.data_);
  std::swap(bucket_sizes_, ht.bucket_sizes_);
  std::swap(total_size_, ht.total_size_);
  std::swap(num_buckets_, ht.num_buckets_);
  std::swap(bucket_cap_, ht.bucket_cap_);
  std::swap(overflow_size_, ht.overflow_size_);
  std::swap(overflow_cap_, ht.overflow_cap_);
  std::swap(hasher_, ht.hasher_);
  std::swap(equal_, ht.equal_);
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  HashTable< Key, Value, Hash, Equal > new_ht{*this};

  try {
    size_t index = new_ht.getElementIndex(k);
    new_ht.data_[index].second = v;
  } catch (const std::out_of_range & e) {
    unsafeAddWithoutCheckingExisting(k, v);
  }

  swap(new_ht);
}

template< class Key, class Value, class Hash, class Equal >
Value chernov::HashTable< Key, Value, Hash, Equal >::drop(Key k)
{
  HashTable< Key, Value, Hash, Equal > new_ht{*this};

  size_t index = new_ht.getElementIndex(k);
  Value value = new_ht.data_[index].second;
  new_ht.removeElementByIndex(index);

  swap(new_ht);
  return value;
}

template< class Key, class Value, class Hash, class Equal >
bool chernov::HashTable< Key, Value, Hash, Equal >::has(Key k)
{
  try {
    getElementIndex(k);
  } catch (const std::out_of_range & e) {
    return false;
  }
  return true;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  setParamsByCountSlots(slots);
  rehash(num_buckets_, bucket_cap_, overflow_cap_);
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::rehash(size_t num_buckets, size_t bucket_cap, size_t overflow_cap)
{
  HashTable< Key, Value, Hash, Equal > new_ht{num_buckets, bucket_cap, overflow_cap};

  for (size_t i = 0; i < num_buckets_; ++i) {
    for (size_t j = 0; j < bucket_sizes_[i]; ++j) {
      Element element = data_[i * bucket_cap_ + j];
      new_ht.unsafeAddWithoutCheckingExisting(element.first, element.second);
    }
  }

  for (size_t i = 0; i < overflow_size_; ++i) {
    Element element = data_[num_buckets_ * bucket_cap_ + i];
    new_ht.unsafeAddWithoutCheckingExisting(element.first, element.second);
  }

  swap(new_ht);
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::getElementIndex(Key k)
{
  size_t home_bucket = hasher_(k) % num_buckets_;
  for (size_t i = 0; i < bucket_sizes_[home_bucket]; ++i) {
    size_t index = home_bucket * bucket_cap_ + i;
    if (k == data_[index].first) {
      return index;
    }
  }
  for (size_t i = 0; i < overflow_size_; ++i) {
    size_t index = num_buckets_ * bucket_cap_ + i;
    if (k == data_[index].first) {
      return index;
    }
  }
  throw std::out_of_range("Element not found");
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::removeElementByIndex(size_t index)
{
  size_t last_bucket_element_index;
  if (index < num_buckets_ * bucket_cap_) {
    size_t home_bucket = index / bucket_cap_;
    last_bucket_element_index = home_bucket * bucket_cap_ + bucket_sizes_[home_bucket] - 1;
    --bucket_sizes_[home_bucket];
  } else {
    last_bucket_element_index = num_buckets_ * bucket_cap_ + overflow_size_ - 1;
    --overflow_size_;
  }
  std::swap(data_[index], data_[last_bucket_element_index]);
  data_[last_bucket_element_index].~Element();
  --total_size_;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::unsafeAddWithoutCheckingExisting(Key k, Value v)
{
  size_t home_bucket = hasher_(k) % num_buckets_;
  if (bucket_sizes_[home_bucket] < bucket_cap_) {
    new (data_ + (home_bucket * bucket_cap_ + bucket_sizes_[home_bucket])) Element{k, v};
    ++bucket_sizes_[home_bucket];
    ++total_size_;
  } else if (overflow_size_ < overflow_cap_) {
    new (data_ + (num_buckets_ * bucket_cap_ + overflow_size_)) Element{k, v};
    ++overflow_size_;
    ++total_size_;
  } else {
    throw std::length_error("Hashtable is full");
  }
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::setParamsByCountSlots(size_t slots)
{
  constexpr size_t default_bucket_cap = 4;
  constexpr size_t default_overflow_cap = 4;

  if (slots < default_bucket_cap) {
    num_buckets_ = 1;
    bucket_cap_ = slots;
    overflow_cap_ = 0;
  } else if (slots < default_bucket_cap + default_overflow_cap) {
    num_buckets_ = 1;
    bucket_cap_ = default_bucket_cap;
    overflow_cap_ = slots - default_bucket_cap;
  } else {
    num_buckets_ = (slots - default_overflow_cap) / default_bucket_cap;
    bucket_cap_ = default_bucket_cap;
    overflow_cap_ = default_overflow_cap + (slots - default_overflow_cap) % default_bucket_cap;
  }
}

#endif
