#ifndef KUKU_HASH_TABLE
#define KUKU_HASH_TABLE

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <cmath>
#include "../common/vector/top-it-vector.hpp"
#include "hash_node.hpp"
#include "kkht_iterator.hpp"
#include "kkht_const_iterator.hpp"

namespace petrov
{
  template< class T >
  struct Equal
  {
    bool operator()(const T& lhs, const T& rhs) const;
  };

  template< class Value, class Key, class Hash, class Equal >
  struct KKHTIterator;

  template< class Value, class Key, class Hash, class Equal >
  struct CKKHTIterator;

  template< class Value, class Key, class Hash, class Equal >
  struct KukuHashTable
  {
    using kkhTable_t = KukuHashTable< Value, Key, Hash, Equal >;
    using iter_t = KKHTIterator< Value, Key, Hash, Equal >;
    using citer_t = CKKHTIterator< Value, Key, Hash, Equal >;

    friend struct KKHTIterator< Value, Key, Hash, Equal >;
    friend struct CKKHTIterator< Value, Key, Hash, Equal >;

    KukuHashTable();
    KukuHashTable(size_t cap);
    ~KukuHashTable();
    KukuHashTable(const KukuHashTable& other);
    KukuHashTable(KukuHashTable&& other) noexcept;
    KukuHashTable& operator=(const KukuHashTable& other);
    KukuHashTable& operator=(KukuHashTable&& other) noexcept;

    void add(const Key& k, const Value& v);
    Value drop(const Key& k);
    bool has(const Key& k) const;
    Value& get(const Key& k);
    const Value& get(const Key& k) const;
    void swap(KukuHashTable& other) noexcept;

    void rehash(const size_t cap);
    void clear();

    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;

    iter_t begin() noexcept;
    iter_t end() noexcept;

    citer_t begin() const noexcept;
    citer_t end() const noexcept;

    citer_t cbegin() const noexcept;
    citer_t cend() const noexcept;

    private:
      topit::Vector< detail::HashNode< Key, Value > > data1_;
      topit::Vector< detail::HashNode< Key, Value > > data2_;
      size_t size_;
      size_t capacity_;
      Hash hash1_;
      Hash hash2_;
      Equal equal_;
      double load_;
      size_t maxKicks_;
  };

  template< class T >
  bool Equal< T >::operator()(const T& lhs, const T& rhs) const
  {
    return lhs == rhs;
  }

  template< class V, class K, class H, class E >
  KukuHashTable< V, K, H, E >::KukuHashTable():
    data1_(),
    data2_(),
    size_(0),
    capacity_(32),
    hash1_(0),
    hash2_(1234),
    equal_(),
    load_(0.0),
    maxKicks_(static_cast<size_t>(std::log2(32) * 2))
  {
    size_t actual_cap = capacity_ / 2;
    for (size_t i = 0; i < actual_cap; ++i)
    {
      data1_.pushBack(detail::HashNode< K, V >());
      data2_.pushBack(detail::HashNode< K, V >());
    }
  }

template< class V, class K, class H, class E >
  KukuHashTable< V, K, H, E >::KukuHashTable(size_t cap):
    data1_(),
    data2_(),
    size_(0),
    capacity_(cap * 2 + 16),
    hash1_(0),
    hash2_(1234),
    equal_(),
    load_(0.0),
    maxKicks_(static_cast< size_t >(std::log2(cap * 2 + 16) * 2))
  {
    size_t actual_cap = capacity_ / 2;
    for (size_t i = 0; i < actual_cap; ++i)
    {
      data1_.pushBack(detail::HashNode< K, V >());
      data2_.pushBack(detail::HashNode< K, V >());
    }
  }

  template< class V, class K, class H, class E >
  KukuHashTable< V, K, H, E >::~KukuHashTable()
  {
    clear();
  }

  template< class V, class K, class H, class E >
  KukuHashTable< V, K, H, E >::KukuHashTable(const KukuHashTable& other):
    data1_(),
    data2_(),
    size_(other.size_),
    capacity_(other.capacity_),
    hash1_(other.hash1_),
    hash2_(other.hash2_),
    equal_(other.equal_),
    load_(other.load_),
    maxKicks_(other.maxKicks_)
  {
    size_t actual_cap = capacity_ / 2;
    for (size_t i = 0; i < actual_cap; ++i)
    {
      data1_.pushBack(other.data1_[i]);
      data2_.pushBack(other.data2_[i]);
    }
  }

  template< class V, class K, class H, class E >
  KukuHashTable< V, K, H, E >::KukuHashTable(KukuHashTable&& other) noexcept:
    data1_(std::move(other.data1_)),
    data2_(std::move(other.data2_)),
    size_(other.size_),
    capacity_(other.capacity_),
    hash1_(std::move(other.hash1_)),
    hash2_(std::move(other.hash2_)),
    equal_(std::move(other.equal_)),
    load_(other.load_),
    maxKicks_(std::log2(capacity_) * 2)
  {
    other.size_ = 0;
    other.capacity_ = 0;
    other.load_ = 0.0;
    other.maxKicks_ = 0;
  }

  template< class V, class K, class H, class E >
  KukuHashTable< V, K, H, E >& KukuHashTable< V, K, H, E >::operator=(const KukuHashTable& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    kkhTable_t cpy = other;
    swap(cpy);
    return *this;
  }


  template< class V, class K, class H, class E >
  KukuHashTable< V, K, H, E >& KukuHashTable< V, K, H, E >::operator=(KukuHashTable&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    KukuHashTable< V, K, H, E> cpy(std::move(other));
    swap(cpy);
    return *this;
  }

  template< class V, class K, class H, class E >
  void KukuHashTable< V, K, H, E >::add(const K& k, const V& v)
  {
    if (load_ > 0.5)
    {
      rehash(capacity_ * 2);
    }

    if (has(k))
    {
      get(k) = v;
      return;
    }

    K curr_k = k;
    V curr_v = v;
    for (size_t i = 0; i < maxKicks_; ++i)
    {
      size_t idx1 = hash1_(curr_k) % data1_.getSize();
      if (data1_[idx1].state_ == detail::EMPTY)
      {
        data1_[idx1].value_ = curr_v;
        data1_[idx1].key_ = curr_k;
        data1_[idx1].state_ = detail::OCCUPIED;
        ++size_;
        load_ = static_cast< double >(size_) / capacity_;
        return;
      }
      std::swap(curr_k, data1_[idx1].key_);
      std::swap(curr_v, data1_[idx1].value_);

      size_t idx2 = hash2_(curr_k) % data2_.getSize();
      if (data2_[idx2].state_ == detail::EMPTY)
      {
        data2_[idx2].value_ = curr_v;
        data2_[idx2].key_ = curr_k;
        data2_[idx2].state_ = detail::OCCUPIED;
        ++size_;
        load_ = static_cast< double >(size_) / capacity_;
        return;
      }
      std::swap(curr_k, data2_[idx2].key_);
      std::swap(curr_v, data2_[idx2].value_);
    }

    rehash(capacity_ * 2);
    add(curr_k, curr_v);
  }

  template< class V, class K, class H, class E >
  V KukuHashTable< V, K, H, E >::drop(const K& k)
  {
    size_t posx = hash1_(k) % data1_.getSize();
    if (data1_[posx].state_ == detail::OCCUPIED && equal_(data1_[posx].key_, k))
    {
      V res = data1_[posx].value_;
      data1_[posx].state_ = detail::EMPTY;
      --size_;
      load_ = static_cast< double >(size_) / capacity_;
      return res;
    }

    posx = hash2_(k) % data2_.getSize();
    if (data2_[posx].state_ == detail::OCCUPIED && equal_(data2_[posx].key_, k))
    {
      V res = data2_[posx].value_;
      data2_[posx].state_ = detail::EMPTY;
      --size_;
      load_ = static_cast< double >(size_) / capacity_;
      return res;
    }

    throw std::runtime_error("Table hasn`t this key");
  }

  template< class V, class K, class H, class E >
  bool KukuHashTable< V, K, H, E >::has(const K& k) const
  {
    size_t id1 = hash1_(k) % data1_.getSize();
    size_t id2 = hash2_(k) % data2_.getSize();
    if ((data1_[id1].state_ == detail::OCCUPIED && equal_(data1_[id1].key_, k))
      || (data2_[id2].state_ == detail::OCCUPIED && equal_(data2_[id2].key_, k)))
    {
      return true;
    }
    return false;
  }

  template< class V, class K, class H, class E >
  V& KukuHashTable< V, K, H, E >::get(const K& k)
  {
    return const_cast< V& >(const_cast< const KukuHashTable& >(*this).get(k));
  }

  template< class V, class K, class H, class E >
  const V& KukuHashTable< V, K, H, E >::get(const K& k) const
  {
    size_t id1 = hash1_(k) % data1_.getSize();
    size_t id2 = hash2_(k) % data2_.getSize();
    if (data1_[id1].state_ == detail::OCCUPIED && equal_(data1_[id1].key_, k))
    {
      return data1_[id1].value_;
    }
    else if (data2_[id2].state_ == detail::OCCUPIED && equal_(data2_[id2].key_, k))
    {
      return data2_[id2].value_;
    }
    else
    {
      throw std::runtime_error("Table hasn`t this key");
    }
  }

  template< class V, class K, class H, class E >
  void KukuHashTable< V, K, H, E >::swap(KukuHashTable& other) noexcept
  {
    std::swap(data1_, other.data1_);
    std::swap(data2_, other.data2_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
    std::swap(load_, other.load_);
    std::swap(maxKicks_, other.maxKicks_);
  }

  template< class V, class K, class H, class E >
  size_t KukuHashTable< V, K, H, E >::size() const noexcept
  {
    return size_;
  }

  template< class V, class K, class H, class E >
  size_t KukuHashTable< V, K, H, E >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class V, class K, class H, class E >
  bool KukuHashTable< V, K, H, E >::empty() const noexcept
  {
    return size() == 0;
  }

  template< class V, class K, class H, class E >
  void KukuHashTable< V, K, H, E >::rehash(const size_t cap)
  {
    kkhTable_t newTable(cap);
    static size_t seed_counter = 1000;
    ++seed_counter;
    newTable.hash1_ = H(seed_counter);
    newTable.hash2_ = H(seed_counter * 7 + 1234);

    for (size_t i = 0; i < data1_.getSize(); ++i)
    {
      if (data1_[i].state_ == detail::OCCUPIED)
      {
        newTable.add(data1_[i].key_, data1_[i].value_);
      }
    }

    for (size_t i = 0; i < data2_.getSize(); ++i)
    {
      if (data2_[i].state_ == detail::OCCUPIED)
      {
        newTable.add(data2_[i].key_, data2_[i].value_);
      }
    }

    swap(newTable);
  }

  template< class V, class K, class H, class E >
  void KukuHashTable< V, K, H, E >::clear()
  {
    for (size_t i = 0; i < data1_.getSize(); ++i)
    {
      data1_[i].state_ = detail::EMPTY;
    }
    for (size_t i = 0; i < data2_.getSize(); ++i)
    {
      data2_[i].state_ = detail::EMPTY;
    }
    size_ = 0;
    load_ = 0.0;
  }

  template< class V, class K, class H, class E >
  KKHTIterator< V, K, H, E > KukuHashTable< V, K, H, E >::begin() noexcept
  {
    iter_t it(this, 1, 0);
    it.skipEmpty();
    return it;
  }

  template< class V, class K, class H, class E >
  CKKHTIterator< V, K, H, E > KukuHashTable< V, K, H, E >::begin() const noexcept
  {
    return cbegin();
  }

  template< class V, class K, class H, class E >
  CKKHTIterator< V, K, H, E > KukuHashTable< V, K, H, E >::cbegin() const noexcept
  {
    citer_t it(this, 1, 0);
    it.skipEmpty();
    return it;
  }

  template< class V, class K, class H, class E >
  KKHTIterator< V, K, H, E > KukuHashTable< V, K, H, E >::end() noexcept
  {
    return iter_t(this, 2, data2_.getSize());
  }

  template< class V, class K, class H, class E >
  CKKHTIterator< V, K, H, E > KukuHashTable< V, K, H, E >::end() const noexcept
  {
    return cend();
  }

  template< class V, class K, class H, class E >
  CKKHTIterator< V, K, H, E > KukuHashTable< V, K, H, E >::cend() const noexcept
  {
    return citer_t(this, 2, data2_.getSize());
  }
}

#endif
