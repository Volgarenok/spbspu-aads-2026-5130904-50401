#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <stdexcept>
#include <utility>
#include <cstddef>
#include "../common/vector.hpp"
#include "../common/list.hpp"

namespace vasyakin
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    using PairType = std::pair< Key, Value >;
    using ChainType = vasyakin::List< PairType >;

  public:
    HashIter();
    HashIter(vasyakin::Vector< ChainType >* buckets, size_t capacity, size_t start_idx);

    HashIter& operator++();
    HashIter operator++(int);
    PairType& operator*() const;
    PairType* operator->() const;
    bool operator==(const HashIter& other) const;
    bool operator!=(const HashIter& other) const;

  private:
    topit::Vector< ChainType >* buckets_;
    size_t capacity_;
    size_t bucket_idx_;
    vasyakin::LIter< PairType > list_it_;
    vasyakin::LIter< PairType > list_end_;

    void find_valid();
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIter< Key, Value, Hash, Equal >;
    using PairType = std::pair< Key, Value >;
    using ChainType = vasyakin::List< PairType >;

  public:
    HashConstIter();
    HashConstIter(const topit::Vector< ChainType >* buckets, size_t capacity, size_t start_idx);

    HashConstIter& operator++();
    HashConstIter operator++(int);
    const PairType& operator*() const;
    const PairType* operator->() const;
    bool operator==(const HashConstIter& other) const;
    bool operator!=(const HashConstIter& other) const;

  private:
    const topit::Vector< ChainType >* buckets_;
    size_t capacity_;
    size_t bucket_idx_;
    vasyakin::LCIter< PairType > list_it_;
    vasyakin::LCIter< PairType > list_end_;

    void find_valid();
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;

    using PairType = std::pair< Key, Value >;
    using ChainType = vasyakin::List< PairType >;
    using BucketsType = topit::Vector< ChainType >;

  public:
    using Iterator = HashIter< Key, Value, Hash, Equal >;
    using const_iterator = HashConstIter< Key, Value, Hash, Equal >;

    explicit HashTable(size_t slots, Hash hasher = Hash{}, Equal equal = Equal{});
    void add(const Key& key, const Value& v);
    Value drop(const Key& key);
    bool has(const Key& key) const;
    void rehash(size_t slots);
    Value& get(const Key& k);
    const Value& get(const Key& k) const;

    Iterator begin();
    Iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

  private:
    BucketsType buckets_;
    size_t size_;
    Hash hasher_;
    Equal equal_;

    std::pair< bool, vasyakin::Node< PairType >* > find_node(size_t ind, const Key& key) const;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal >::HashIter():
    buckets_(nullptr),
    capacity_(0),
    bucket_idx_(0),
    list_it_(nullptr),
    list_end_(nullptr)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal >::HashIter(vasyakin::Vector< ChainType >* buckets, size_t capacity, size_t start_idx):
    buckets_(buckets),
    capacity_(capacity),
    bucket_idx_(start_idx),
    list_it_(nullptr),
    list_end_(nullptr)
  {
    find_valid();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashIter< Key, Value, Hash, Equal >::find_valid()
  {
    while (bucket_idx_ < capacity_)
    {
      list_it_ = (*buckets_)[bucket_idx_].begin();
      list_end_ = (*buckets_)[bucket_idx_].end();
      if (list_it_ != list_end_)
      {
        return;
      }
      ++bucket_idx_;
    }

    buckets_ = nullptr;
    list_it_ = LIter< PairType >(nullptr);
    list_end_ = LIter< PairType >(nullptr);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal >& HashIter< Key, Value, Hash, Equal >::operator++()
  {
    if (!buckets_)
    {
      return *this;
    }

    ++list_it_;

    if (list_it_ != list_end_)
    {
      return *this;
    }

    ++bucket_idx_;
    find_valid();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashIter< Key, Value, Hash, Equal >::operator++(int)
  {
    HashIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashIter< Key, Value, Hash, Equal >::PairType& HashIter< Key, Value, Hash, Equal >::operator*() const
  {
    return *list_it_;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashIter< Key, Value, Hash, Equal >::PairType* HashIter< Key, Value, Hash, Equal >::operator->() const
  {
    return &(*list_it_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashIter< Key, Value, Hash, Equal >::operator==(const HashIter& other) const
  {
    if (!buckets_ && !other.buckets_)
    {
      return true;
    }
    if (!buckets_ || !other.buckets_)
    {
      return false;
    }
    if (bucket_idx_ >= capacity_ && other.bucket_idx_ >= other.capacity_)
    {
      return true;
    }

    return bucket_idx_ == other.bucket_idx_ && list_it_ == other.list_it_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter& other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
    buckets_(nullptr),
    capacity_(0),
    bucket_idx_(0),
    list_it_(nullptr),
    list_end_(nullptr)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal >::HashConstIter(const topit::Vector< ChainType >* buckets, size_t capacity, size_t start_idx):
    buckets_(buckets),
    capacity_(capacity),
    bucket_idx_(start_idx),
    list_it_(nullptr),
    list_end_(nullptr)
  {
    find_valid();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashConstIter< Key, Value, Hash, Equal >::find_valid()
  {
    while (bucket_idx_ < capacity_)
    {
      list_it_ = (*buckets_)[bucket_idx_].begin();
      list_end_ = (*buckets_)[bucket_idx_].end();

      if (list_it_ != list_end_)
      {
        return;
      }
      ++bucket_idx_;
    }
    buckets_ = nullptr;
    list_it_ = LCIter< PairType >(nullptr);
    list_end_ = LCIter< PairType >(nullptr);
  }
}

#endif
