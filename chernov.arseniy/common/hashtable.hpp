#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>
#include <htiter.hpp>

namespace chernov {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    template< class K, class V, class H, class E, bool IC >
    friend class HTIter;

    using slot = std::pair< const Key, Value >;
    using iterator = HTIter< Key, Value, Hash, Equal, false >;
    using const_iterator = HTIter< Key, Value, Hash, Equal, true >;

    HashTable();
    HashTable(const HashTable & ht);
    HashTable(HashTable && ht) noexcept;

    HashTable(size_t slots);
    HashTable(size_t num_buckets, size_t bucket_cap, size_t overflow_cap);

    ~HashTable();

    HashTable & operator=(const HashTable & ht);
    HashTable & operator=(HashTable && ht) noexcept;

    void swap(HashTable & ht) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t maxCapacity() const noexcept;
    size_t getOverflowCapacity() const noexcept;
    size_t getOverflowSize() const noexcept;
    double getAverageNumberOfItems() const noexcept;

    void clear() noexcept;

    void add(const Key & k, const Value & v);
    void remove(const Key & k);
    bool has(const Key & k) const;
    void rehash(size_t slots);
    void rehash(size_t num_buckets, size_t bucket_cap, size_t overflow_cap);

    void setMaxOverflowSize(size_t size) noexcept;
    void setMaxAverageNumberOfItems(double average) noexcept;

    void setUpdBuckets(std::function< size_t(size_t) > func);
    void setUpdBucketSize(std::function< size_t(size_t) > func);

    Value & at(const Key & k);
    const Value & at(const Key & k) const;

    Value & operator[](const Key & k);
    const Value & operator[](const Key & k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    slot * data_;
    size_t * bucket_sizes_;
    size_t total_size_;

    size_t num_buckets_;
    size_t bucket_cap_;
    size_t overflow_size_;
    size_t overflow_cap_;

    size_t max_overflow_size_;
    double max_average_number_of_items_;

    Hash hasher_;
    Equal equal_;

    std::function< size_t(size_t) > upd_buckets_func_;
    std::function< size_t(size_t) > upd_bucket_size_func_;

    size_t getElementIndex(const Key & k) const;
    void removeElementByIndex(size_t index);
    void unsafeAddWithoutCheckingExisting(Key k, Value v);
    void calculateParamsFromSlots(size_t slots,
      size_t & new_num_buckets,
      size_t & new_bucket_cap,
      size_t & new_overflow_cap) const noexcept;
    void setParamsByCountSlots(size_t slots) noexcept;
    size_t getFirstValidIndex() const noexcept;
    size_t getEndIndex() const noexcept;
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
  max_overflow_size_(std::numeric_limits< size_t >::max()),
  max_average_number_of_items_(std::numeric_limits< double >::max()),
  hasher_(Hash{}),
  equal_(Equal{})
{}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & ht):
  HashTable(ht.num_buckets_, ht.bucket_cap_, ht.overflow_cap_)
{
  hasher_ = ht.hasher_;
  equal_ = ht.equal_;
  max_overflow_size_ = ht.max_overflow_size_;
  max_average_number_of_items_ = ht.max_average_number_of_items_;
  upd_buckets_func_ = ht.upd_buckets_func_;
  upd_bucket_size_func_ = ht.upd_bucket_size_func_;

  try {
    for (size_t i = 0; i < num_buckets_; ++i) {
      for (size_t j = 0; j < ht.bucket_sizes_[i]; ++j) {
        new (data_ + (i * bucket_cap_ + j)) slot(ht.data_[i * bucket_cap_ + j]);
        ++bucket_sizes_[i];
        ++total_size_;
      }
    }
    for (size_t i = 0; i < ht.overflow_size_; ++i) {
      new (data_ + (num_buckets_ * bucket_cap_ + i)) slot(ht.data_[num_buckets_ * bucket_cap_ + i]);
      ++overflow_size_;
      ++total_size_;
    }
  } catch (...) {
    clear();
    ::operator delete(data_);
    delete[] bucket_sizes_;
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable && ht) noexcept:
  data_(std::exchange(ht.data_, nullptr)),
  bucket_sizes_(std::exchange(ht.bucket_sizes_, nullptr)),
  total_size_(std::exchange(ht.total_size_, 0)),
  num_buckets_(std::exchange(ht.num_buckets_, 0)),
  bucket_cap_(std::exchange(ht.bucket_cap_, 0)),
  overflow_size_(std::exchange(ht.overflow_size_, 0)),
  overflow_cap_(std::exchange(ht.overflow_cap_, 0)),
  max_overflow_size_(std::exchange(ht.max_overflow_size_, std::numeric_limits< size_t >::max())),
  max_average_number_of_items_(std::exchange(ht.max_average_number_of_items_, std::numeric_limits< double >::max())),
  hasher_(ht.hasher_),
  equal_(ht.equal_),
  upd_buckets_func_(std::move(ht.upd_buckets_func_)),
  upd_bucket_size_func_(std::move(ht.upd_bucket_size_func_))
{}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(nullptr),
  bucket_sizes_(nullptr),
  total_size_(0),
  num_buckets_(0),
  bucket_cap_(0),
  overflow_size_(0),
  overflow_cap_(0),
  max_overflow_size_(std::numeric_limits< size_t >::max()),
  max_average_number_of_items_(std::numeric_limits< double >::max()),
  hasher_(Hash{}),
  equal_(Equal{})
{
  if (slots) {
    setParamsByCountSlots(slots);
    data_ = static_cast< slot * >(::operator new(sizeof(slot) * (num_buckets_ * bucket_cap_ + overflow_cap_)));
    try {
      bucket_sizes_ = new size_t[num_buckets_]{0};
    } catch (...) {
      ::operator delete(data_);
      throw;
    }
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
  max_overflow_size_(std::numeric_limits< size_t >::max()),
  max_average_number_of_items_(std::numeric_limits< double >::max()),
  hasher_(Hash{}),
  equal_(Equal{})
{
  size_t size = num_buckets_ * bucket_cap_ + overflow_cap_;
  if (size) {
    if (num_buckets == 0) {
      throw std::logic_error("num_buckets must be positive");
    }
    data_ = static_cast< slot * >(::operator new(sizeof(slot) * size));
    bucket_sizes_ = new size_t[num_buckets_]{0};
  }
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  clear();
  ::operator delete(data_);
  delete[] bucket_sizes_;
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal > &
  chernov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable & ht)
{
  if (this == std::addressof(ht)) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > new_ht = ht;
  swap(new_ht);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal > &
  chernov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable && ht) noexcept
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
  std::swap(max_overflow_size_, ht.max_overflow_size_);
  std::swap(max_average_number_of_items_, ht.max_average_number_of_items_);
  std::swap(hasher_, ht.hasher_);
  std::swap(equal_, ht.equal_);
  std::swap(upd_buckets_func_, ht.upd_buckets_func_);
  std::swap(upd_bucket_size_func_, ht.upd_bucket_size_func_);
}

template< class Key, class Value, class Hash, class Equal >
bool chernov::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return total_size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return total_size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::maxCapacity() const noexcept
{
  return num_buckets_ * bucket_cap_ + overflow_cap_;
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::getOverflowCapacity() const noexcept
{
  return overflow_cap_;
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::getOverflowSize() const noexcept
{
  return overflow_size_;
}

template< class Key, class Value, class Hash, class Equal >
double chernov::HashTable< Key, Value, Hash, Equal >::getAverageNumberOfItems() const noexcept
{
  return num_buckets_ ? total_size_ * 1.0 / num_buckets_ : 0.0;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < num_buckets_; ++i) {
    for (size_t j = 0; j < bucket_sizes_[i]; ++j) {
      data_[i * bucket_cap_ + j].~slot();
    }
    bucket_sizes_[i] = 0;
  }
  for (size_t i = 0; i < overflow_size_; ++i) {
    data_[num_buckets_ * bucket_cap_ + i].~slot();
  }
  overflow_size_ = 0;
  total_size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::add(const Key & k, const Value & v)
{
  HashTable new_ht(*this);

  try {
    size_t index = new_ht.getElementIndex(k);
    new_ht.data_[index].second = v;
  } catch (const std::out_of_range &) {
    new_ht.unsafeAddWithoutCheckingExisting(k, v);

    bool need_rehash = false;
    if (new_ht.overflow_size_ > new_ht.max_overflow_size_) {
      need_rehash = true;
    }
    if (!need_rehash && new_ht.getAverageNumberOfItems() > new_ht.max_average_number_of_items_) {
      need_rehash = true;
    }

    while (need_rehash) {
      if (!new_ht.upd_buckets_func_ || !new_ht.upd_bucket_size_func_) {
        throw std::logic_error("auto rehash required but update functions are not set");
      }

      size_t new_buckets = new_ht.upd_buckets_func_(new_ht.num_buckets_);
      size_t new_bucket_cap = new_ht.upd_bucket_size_func_(new_ht.bucket_cap_);

      if (new_buckets == 0) {
        new_buckets = 1;
      }
      if (new_bucket_cap == 0) {
        new_bucket_cap = 1;
      }

      size_t old_buckets = new_ht.num_buckets_;
      size_t old_bucket_cap = new_ht.bucket_cap_;

      new_ht.rehash(new_buckets, new_bucket_cap, new_ht.overflow_cap_);

      need_rehash = false;
      if (new_ht.overflow_size_ > new_ht.max_overflow_size_) {
        need_rehash = true;
      }
      if (!need_rehash && new_ht.getAverageNumberOfItems() > new_ht.max_average_number_of_items_) {
        need_rehash = true;
      }

      if (need_rehash) {
        if (new_ht.num_buckets_ <= old_buckets && new_ht.bucket_cap_ <= old_bucket_cap) {
          throw std::runtime_error("cannot satisfy limits after rehash");
        }
      }
    }
  }

  swap(new_ht);
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::remove(const Key & k)
{
  HashTable new_ht(*this);
  size_t index = new_ht.getElementIndex(k);
  new_ht.removeElementByIndex(index);
  swap(new_ht);
}

template< class Key, class Value, class Hash, class Equal >
bool chernov::HashTable< Key, Value, Hash, Equal >::has(const Key & k) const
{
  try {
    getElementIndex(k);
  } catch (const std::out_of_range &) {
    return false;
  }
  return true;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  size_t new_num_buckets, new_bucket_cap, new_overflow_cap;
  calculateParamsFromSlots(slots, new_num_buckets, new_bucket_cap, new_overflow_cap);
  rehash(new_num_buckets, new_bucket_cap, new_overflow_cap);
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::rehash(size_t num_buckets, size_t bucket_cap, size_t overflow_cap)
{
  HashTable new_ht(num_buckets, bucket_cap, overflow_cap);
  new_ht.hasher_ = hasher_;
  new_ht.equal_ = equal_;
  new_ht.max_overflow_size_ = max_overflow_size_;
  new_ht.max_average_number_of_items_ = max_average_number_of_items_;
  new_ht.upd_buckets_func_ = upd_buckets_func_;
  new_ht.upd_bucket_size_func_ = upd_bucket_size_func_;

  for (size_t i = 0; i < num_buckets_; ++i) {
    for (size_t j = 0; j < bucket_sizes_[i]; ++j) {
      slot element = data_[i * bucket_cap_ + j];
      new_ht.unsafeAddWithoutCheckingExisting(element.first, element.second);
    }
  }

  for (size_t i = 0; i < overflow_size_; ++i) {
    slot element = data_[num_buckets_ * bucket_cap_ + i];
    new_ht.unsafeAddWithoutCheckingExisting(element.first, element.second);
  }

  swap(new_ht);
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::setMaxOverflowSize(size_t size) noexcept
{
  max_overflow_size_ = size;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::setMaxAverageNumberOfItems(double average) noexcept
{
  max_average_number_of_items_ = average;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::setUpdBuckets(std::function< size_t(size_t) > func)
{
  upd_buckets_func_ = std::move(func);
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::setUpdBucketSize(std::function< size_t(size_t) > func)
{
  upd_bucket_size_func_ = std::move(func);
}

template< class Key, class Value, class Hash, class Equal >
Value & chernov::HashTable< Key, Value, Hash, Equal >::at(const Key & k)
{
  return operator[](k);
}

template< class Key, class Value, class Hash, class Equal >
const Value & chernov::HashTable< Key, Value, Hash, Equal >::at(const Key & k) const
{
  return operator[](k);
}

template< class Key, class Value, class Hash, class Equal >
Value & chernov::HashTable< Key, Value, Hash, Equal >::operator[](const Key & k)
{
  const HashTable * cthis = this;
  return const_cast< Value & >(cthis->operator[](k));
}

template< class Key, class Value, class Hash, class Equal >
const Value & chernov::HashTable< Key, Value, Hash, Equal >::operator[](const Key & k) const
{
  size_t index = getElementIndex(k);
  return data_[index].second;
}

template< class Key, class Value, class Hash, class Equal >
typename chernov::HashTable< Key, Value, Hash, Equal >::iterator chernov::HashTable< Key, Value, Hash, Equal >::begin()
{
  return iterator(this, getFirstValidIndex());
}

template< class Key, class Value, class Hash, class Equal >
typename chernov::HashTable< Key, Value, Hash, Equal >::const_iterator
  chernov::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return const_iterator(this, getFirstValidIndex());
}

template< class Key, class Value, class Hash, class Equal >
typename chernov::HashTable< Key, Value, Hash, Equal >::const_iterator
  chernov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return begin();
}

template< class Key, class Value, class Hash, class Equal >
typename chernov::HashTable< Key, Value, Hash, Equal >::iterator chernov::HashTable< Key, Value, Hash, Equal >::end()
{
  return iterator(this, getEndIndex());
}

template< class Key, class Value, class Hash, class Equal >
typename chernov::HashTable< Key, Value, Hash, Equal >::const_iterator
  chernov::HashTable< Key, Value, Hash, Equal >::end() const
{
  return const_iterator(this, getEndIndex());
}

template< class Key, class Value, class Hash, class Equal >
typename chernov::HashTable< Key, Value, Hash, Equal >::const_iterator
  chernov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return end();
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::getElementIndex(const Key & k) const
{
  if (num_buckets_) {
    size_t home_bucket = hasher_(k) % num_buckets_;
    for (size_t i = 0; i < bucket_sizes_[home_bucket]; ++i) {
      size_t index = home_bucket * bucket_cap_ + i;
      if (equal_(k, data_[index].first)) {
        return index;
      }
    }
    for (size_t i = 0; i < overflow_size_; ++i) {
      size_t index = num_buckets_ * bucket_cap_ + i;
      if (equal_(k, data_[index].first)) {
        return index;
      }
    }
  }
  throw std::out_of_range("Element not found");
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::removeElementByIndex(size_t index)
{
  if (bucket_cap_ == 0) {
    throw std::out_of_range("Capacity of hashtable is 0");
  }
  size_t last_bucket_element_index;
  size_t home_bucket = index / bucket_cap_;
  if (index < num_buckets_ * bucket_cap_) {
    last_bucket_element_index = home_bucket * bucket_cap_ + bucket_sizes_[home_bucket] - 1;
  } else {
    last_bucket_element_index = num_buckets_ * bucket_cap_ + overflow_size_ - 1;
  }

  data_[index].~slot();
  if (index != last_bucket_element_index) {
    new (data_ + index) slot(std::move(data_[last_bucket_element_index]));
    data_[last_bucket_element_index].~slot();
  }

  if (index < num_buckets_ * bucket_cap_) {
    --bucket_sizes_[home_bucket];
  } else {
    --overflow_size_;
  }
  --total_size_;
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::unsafeAddWithoutCheckingExisting(Key k, Value v)
{
  size_t hash = hasher_(k);
  if (num_buckets_ && bucket_sizes_[hash % num_buckets_] < bucket_cap_) {
    size_t home_bucket = hash % num_buckets_;
    new (data_ + (home_bucket * bucket_cap_ + bucket_sizes_[home_bucket])) slot{k, v};
    ++bucket_sizes_[home_bucket];
    ++total_size_;
  } else if (overflow_size_ < overflow_cap_) {
    new (data_ + (num_buckets_ * bucket_cap_ + overflow_size_)) slot{k, v};
    ++overflow_size_;
    ++total_size_;
  } else {
    throw std::length_error("Hashtable is full");
  }
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::calculateParamsFromSlots(size_t slots,
  size_t & new_num_buckets,
  size_t & new_bucket_cap,
  size_t & new_overflow_cap) const noexcept
{
  constexpr size_t default_bucket_cap = 4;
  constexpr size_t default_overflow_cap = 4;

  if (slots < default_bucket_cap) {
    new_num_buckets = 1;
    new_bucket_cap = slots;
    new_overflow_cap = 0;
  } else if (slots < default_bucket_cap + default_overflow_cap) {
    new_num_buckets = 1;
    new_bucket_cap = default_bucket_cap;
    new_overflow_cap = slots - default_bucket_cap;
  } else {
    new_num_buckets = (slots - default_overflow_cap) / default_bucket_cap;
    new_bucket_cap = default_bucket_cap;
    new_overflow_cap = default_overflow_cap + (slots - default_overflow_cap) % default_bucket_cap;
  }
}

template< class Key, class Value, class Hash, class Equal >
void chernov::HashTable< Key, Value, Hash, Equal >::setParamsByCountSlots(size_t slots) noexcept
{
  calculateParamsFromSlots(slots, num_buckets_, bucket_cap_, overflow_cap_);
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::getFirstValidIndex() const noexcept
{
  for (size_t i = 0; i < num_buckets_; ++i) {
    if (bucket_sizes_[i] > 0) {
      return i * bucket_cap_;
    }
  }
  if (overflow_size_ > 0) {
    return num_buckets_ * bucket_cap_;
  }
  return getEndIndex();
}

template< class Key, class Value, class Hash, class Equal >
size_t chernov::HashTable< Key, Value, Hash, Equal >::getEndIndex() const noexcept
{
  return num_buckets_ * bucket_cap_ + overflow_cap_;
}

#endif
