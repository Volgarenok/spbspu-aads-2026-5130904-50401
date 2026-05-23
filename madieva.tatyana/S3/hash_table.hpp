#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include "../common/vector.hpp"
#include "../common/list.hpp"
#include <stdexcept>

namespace madieva {

  template<class Key, class Value, class Hash, class Equal>
  class HTIter;

  template<class Key, class Value, class Hash, class Equal>
  class HTCIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
    friend class HTIter<Key, Value, Hash, Equal>;
    friend class HTCIter<Key, Value, Hash, Equal>;
    using Pair = std::pair<Key, Value>;
    using h_it = HTIter<Key, Value, Hash, Equal>;
    using hc_it = HTCIter<Key, Value, Hash, Equal>;
    Vector< List< Pair > > buckets_;
    Hash hasher_;
    Equal comparer_;
    size_t count_;
    size_t getIndex(const Key& k) const;
  public:
    h_it begin();
    h_it end();
    hc_it begin() const;
    hc_it end() const;
    explicit HashTable(size_t count = 16);
    void add(Key k, Value v);
    bool has(Key k) const;
    Value & get(Key k);
    const Value & get(Key k) const;
    Value drop(Key k);
    void rehash(size_t slots);
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;
  };

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::getIndex(const Key & k) const {
    size_t size = buckets_.getSize();
    if (size == 0) {
      return 0;
    }
    return hasher_(k) % size;
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter<Key, Value, Hash, Equal> HashTable< Key, Value, Hash, Equal >::begin()
  {
    for (size_t i = 0; i < buckets_.getSize(); ++i) {
      if (buckets_[i].size() > 0) {
        return h_it(i, buckets_[i].begin(), &buckets_);
      }
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter<Key, Value, Hash, Equal> HashTable< Key, Value, Hash, Equal >::end()
  {
    return h_it(buckets_.getSize(), LIter< Pair >(nullptr), & buckets_);
  }

  template<class Key, class Value, class Hash, class Equal>
  HTCIter<Key, Value, Hash, Equal> HashTable<Key, Value, Hash, Equal>::begin() const
  {
    for (size_t i = 0; i < buckets_.getSize(); ++i) {
      if (buckets_[i].size() > 0) {
        return hc_it(i, buckets_[i].begin(), &buckets_);
      }
    }
    return end();
  }

  template<class Key, class Value, class Hash, class Equal>
  HTCIter<Key, Value, Hash, Equal> HashTable<Key, Value, Hash, Equal>::end() const
  {
    return hc_it(buckets_.getSize(), LCIter<Pair>(nullptr), &buckets_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t count) :
    buckets_(), count_(0)
  {
    buckets_.reserve(count);
    for(size_t i = 0; i < count; ++i) {
      buckets_.pushBack(List< Pair >());
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
  {
    size_t index = getIndex(k);
    List< Pair > & bucket = buckets_[index];
    LIter< Pair > it = bucket.begin();
    for (size_t i = 0; i < bucket.size(); ++i) {
      if (comparer_((*it).first, k)) {
        (*it).second = v;
        return;
      }
      ++it;
    }
    bucket.push_back(std::make_pair(k, v));
    ++count_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(Key k) const
  {
    size_t index = getIndex(k);
    const List< Pair > & bucket = buckets_[index];
    LCIter< Pair > it = bucket.begin();
    for (size_t i = 0; i < bucket.size(); ++i) {
      if (comparer_((*it).first, k)) {
        return true;
      }
      ++it;
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::get(Key k)
  {
    size_t index = getIndex(k);
    List< Pair > & bucket = buckets_[index];
    LIter< Pair > it = bucket.begin();
    for (size_t i = 0; i < bucket.size(); ++i) {
      if (comparer_((*it).first, k)) {
        return (*it).second;
      }
      ++it;
    }
    throw std::out_of_range("no such key\n");
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value & HashTable< Key, Value, Hash, Equal >::get(Key k) const
  {
    size_t index = getIndex(k);
    const List< Pair > & bucket = buckets_[index];
    LCIter< Pair > it = bucket.begin();
    for (size_t i = 0; i < bucket.size(); ++i) {
      if (comparer_((*it).first, k)) {
        return (*it).second;
      }
      ++it;
    }
    throw std::out_of_range("no such key\n");
  }


  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(Key k)
  {
    size_t index = getIndex(k);
    List< Pair > & bucket = buckets_[index];
    LIter< Pair > it = bucket.begin();
    for (size_t i = 0; i < bucket.size(); ++i) {
      if (comparer_((*it).first, k)) {
        Value temp = (*it).second;
        bucket.erase(it);
        count_--;
        return temp;
      }
      ++it;
    }
    throw std::out_of_range("no element\n");
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t new_slots)
  {
    Vector< List< Pair > > new_buckets;
    new_buckets.reserve(new_slots);
    for(size_t i = 0; i < new_slots; ++i) {
      new_buckets.pushBack(List< Pair >());
    }
    for (size_t i = 0; i < buckets_.getSize(); ++i) {
      List<Pair>& bucket = buckets_[i];
      LIter<Pair> it = bucket.begin();
      for (size_t j = 0; j < bucket.size(); ++j) {
        size_t index = hasher_((*it).first) % new_slots;
        new_buckets[index].push_back(std::make_pair((*it).first, (*it).second));
        ++it;
      }
    }
    buckets_ = std::move(new_buckets);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return count_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
  {
    return buckets_.getCapacity();
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return count_ == 0;
  }
}

#include "hash_table_iter.hpp"
#include "hash_table_citer.hpp"

#endif
