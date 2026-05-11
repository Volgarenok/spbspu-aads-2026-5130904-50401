#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>

namespace chernov {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    HashTable();
    ~HashTable();
    HashTable(size_t slots);
    HashTable(size_t num_buckets, size_t bucket_cap, size_t overflow_cap);
    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k);
    void rehash(size_t slots);
  private:
    Value * data_;
    size_t * bucket_sizes_;
    size_t num_buckets_;
    size_t bucket_cap_;
    size_t overflow_size_;
    size_t overflow_cap_;
  };
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable():
  data_(nullptr),
  bucket_sizes_(nullptr),
  num_buckets_(0),
  bucket_cap_(0),
  overflow_size_(0),
  overflow_cap_(0)
{}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(nullptr),
  bucket_sizes_(nullptr),
  num_buckets_(0),
  bucket_cap_(0),
  overflow_size_(0),
  overflow_cap_(0)
{
  constexpr size_t default_bucket_cap = 4;

  if (slots) {
    if (slots < default_bucket_cap) {
      num_buckets_ = 1;
      bucket_cap_ = slots;
      overflow_cap_ = 0;
    } else {
      num_buckets_ = slots / default_bucket_cap;
      bucket_cap_ = default_bucket_cap;
      overflow_cap_ = slots % default_bucket_cap;
    }

    data_ = static_cast< Value * >(::operator new (sizeof(Value) * (num_buckets_ * bucket_cap_ + overflow_cap_)));
    bucket_sizes_ = new size_t[num_buckets_]{0};
  }
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t num_buckets, size_t bucket_cap, size_t overflow_cap):
  data_(nullptr),
  bucket_sizes_(nullptr),
  num_buckets_(num_buckets),
  bucket_cap_(bucket_cap),
  overflow_size_(0),
  overflow_cap_(overflow_cap)
{
  size_t size = num_buckets_ * bucket_cap_ + overflow_cap_;
  if (size) {
    data_ = static_cast< Value * >(::operator new (sizeof(Value) * size));
    bucket_sizes_ = new size_t[num_buckets_]{0};
  }
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  // Исправить прохождение по элементам
  for (size_t i = 0; i < (num_buckets_ * bucket_cap_ + overflow_cap_); ++i) {
    data_[i].~Value();
  }
  ::operator delete (data_);
  delete [] bucket_sizes_;
}

#endif
