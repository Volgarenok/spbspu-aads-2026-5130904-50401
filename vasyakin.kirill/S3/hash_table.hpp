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
    HashIter(topit::Vector< ChainType >* buckets, size_t capacity, size_t start_idx);

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
}

#endif
