#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <stdexcept>
#include <utility>
#include <cstddef>
#include <memory>
#include "vector.hpp"
#include "list.hpp"

namespace vasyakin
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashIter
  {
  public:
    using PairType = std::pair< Key, Value >;

    HashIter();

    HashIter& operator++() noexcept;
    HashIter operator++(int) noexcept;
    PairType& operator*() noexcept;
    PairType* operator->() noexcept;
    bool operator==(const HashIter& other) const noexcept;
    bool operator!=(const HashIter& other) const noexcept;

  private:
    HashIter(vasyakin::Vector< vasyakin::List< PairType > >* buckets,
      size_t capacity,
      size_t start_idx,
      vasyakin::LIter< PairType > list_it = vasyakin::LIter< PairType >{});

    HashIter(const HashConstIter< Key, Value, Hash, Equal >& other);

    vasyakin::Vector< vasyakin::List< PairType > >* buckets_;
    size_t capacity_;
    size_t bucket_idx_;
    vasyakin::LIter< PairType > list_it_;
    vasyakin::LIter< PairType > list_end_;

    void find_valid();

    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter
  {
  public:
    using PairType = std::pair< Key, Value >;

    HashConstIter();

    HashConstIter& operator++() noexcept;
    HashConstIter operator++(int) noexcept;
    const PairType& operator*() const noexcept;
    const PairType* operator->() const noexcept;
    bool operator==(const HashConstIter& other) const noexcept;
    bool operator!=(const HashConstIter& other) const noexcept;

  private:
    HashConstIter(
    const vasyakin::Vector< vasyakin::List< PairType > >* buckets,
    size_t capacity,
    size_t bucket_idx,
    vasyakin::LCIter< PairType > list_it = vasyakin::LCIter< PairType >{});

    const vasyakin::Vector< vasyakin::List< PairType > >* buckets_;
    size_t capacity_;
    size_t bucket_idx_;
    vasyakin::LCIter< PairType > list_it_;
    vasyakin::LCIter< PairType > list_end_;

    void find_valid();

    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIter< Key, Value, Hash, Equal >;

  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    using PairType = std::pair< Key, Value >;
    using Iterator = HashIter< Key, Value, Hash, Equal >;
    using ConstIterator = HashConstIter< Key, Value, Hash, Equal >;

    explicit HashTable(size_t slots);

    template< class T >
    std::pair< Iterator, bool > insert(T&& value);

    size_t erase(const Key& key) noexcept;

    bool contains(const Key& key) const noexcept;
    void rehash(size_t slots);

    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

  private:
    vasyakin::Vector< vasyakin::List< PairType > > buckets_;
    size_t size_;
    Hash hasher_;
    Equal equal_;

    std::pair< bool, vasyakin::detail::Node< PairType >* > find_node(size_t ind, const Key& key) const;

    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
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
  HashIter< Key, Value, Hash, Equal >::HashIter(
    vasyakin::Vector< vasyakin::List< PairType > >* buckets,
    size_t capacity,
    size_t bucket_idx,
    vasyakin::LIter< PairType > list_it):
    buckets_(buckets),
    capacity_(capacity),
    bucket_idx_(bucket_idx),
    list_it_(list_it),
    list_end_(buckets_ ? (*buckets_)[bucket_idx_].end() : vasyakin::LIter< PairType >{})
  {
    if (list_it_.ptr_ == nullptr && buckets_ != nullptr)
    {
      find_valid();
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal >::HashIter(
    const HashConstIter< Key, Value, Hash, Equal >& other):
    buckets_(const_cast< vasyakin::Vector< vasyakin::List< PairType > >* >(other.buckets_)),
    capacity_(other.capacity_),
    bucket_idx_(other.bucket_idx_),
    list_it_(other.list_it_),
    list_end_(other.list_end_)
  {}

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
    list_it_ = LIter< PairType >();
    list_end_ = LIter< PairType >();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal >& HashIter< Key, Value, Hash, Equal >::operator++() noexcept
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
  HashIter< Key, Value, Hash, Equal > HashIter< Key, Value, Hash, Equal >::operator++(int) noexcept
  {
    HashIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashIter< Key, Value, Hash, Equal >::PairType&
  HashIter< Key, Value, Hash, Equal >::operator*() noexcept
  {
    return *list_it_;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashIter< Key, Value, Hash, Equal >::PairType*
  HashIter< Key, Value, Hash, Equal >::operator->() noexcept
  {
    return std::addressof(*list_it_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashIter< Key, Value, Hash, Equal >::operator==(const HashIter& other) const noexcept
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
  bool HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter& other) const noexcept
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
  HashConstIter< Key, Value, Hash, Equal >::HashConstIter(
    const vasyakin::Vector< vasyakin::List< PairType > >* buckets,
    size_t capacity,
    size_t bucket_idx,
    vasyakin::LCIter< PairType > list_it):
    buckets_(buckets),
    capacity_(capacity),
    bucket_idx_(bucket_idx),
    list_it_(list_it),
    list_end_(buckets_ ? (*buckets_)[bucket_idx_].cend() : vasyakin::LCIter< PairType >{})
  {
    if (list_it_.ptr_ == nullptr && buckets_ != nullptr)
    {
      find_valid();
    }
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
    list_it_ = LCIter< PairType >();
    list_end_ = LCIter< PairType >();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal >&
    HashConstIter< Key, Value, Hash, Equal >::operator++() noexcept
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
  HashConstIter< Key, Value, Hash, Equal >
    HashConstIter< Key, Value, Hash, Equal >::operator++(int) noexcept
  {
    HashConstIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  const typename HashConstIter< Key, Value, Hash, Equal >::PairType&
    HashConstIter< Key, Value, Hash, Equal >::operator*() const noexcept
  {
    return *list_it_;
  }

  template< class Key, class Value, class Hash, class Equal >
  const typename HashConstIter< Key, Value, Hash, Equal >::PairType*
    HashConstIter< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return std::addressof(*list_it_);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashConstIter< Key, Value, Hash, Equal >::operator==(const HashConstIter& other) const noexcept
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
  bool HashConstIter< Key, Value, Hash, Equal >::operator!=(const HashConstIter& other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
    buckets_(slots, vasyakin::List< PairType >{}),
    size_(0),
    hasher_(),
    equal_()
  {
    if (slots == 0)
    {
      throw std::invalid_argument("Slots count must be > 0");
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class T >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::Iterator, bool >
  HashTable< Key, Value, Hash, Equal >::insert(T&& value)
  {
    size_t idx = hasher_(value.first) % buckets_.getSize();
    auto res = find_node(idx, value.first);

    if (res.first)
    {
      Iterator existing_it(
        &buckets_, buckets_.getSize(), idx, LIter< PairType >(res.second->next_));

      return {existing_it, false};
    }

    buckets_[idx].pushBack(std::forward< T >(value));
    ++size_;

    return {find(value.first), true};
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key) noexcept
  {
    size_t idx = hasher_(key) % buckets_.getSize();
    auto res = find_node(idx, key);

    if (!res.first)
    {
      return 0;
    }

    buckets_[idx].erase(LIter< PairType >(res.second));
    --size_;

    return 1;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::contains(const Key& key) const noexcept
  {
    size_t ind = hasher_(key) % buckets_.getSize();
    return find_node(ind, key).first;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    if (slots == 0 || slots == buckets_.getSize())
    {
      return;
    }

    vasyakin::Vector< vasyakin::List< PairType > > tmp(slots, vasyakin::List< PairType >{});
    for (size_t i = 0; i < buckets_.getSize(); ++i)
    {
      for (auto it = buckets_[i].begin(); it != buckets_[i].end(); ++it)
      {
        tmp[hasher_(it->first) % slots].pushBack(*it);
      }
    }

    buckets_.swap(tmp);
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const HashTable* >(this)->at(key));
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("Key not found");
    }

    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator
  HashTable< Key, Value, Hash, Equal >::find(const Key& key)
  {
    return Iterator(static_cast< const HashTable* >(this)->find(key));
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator
  HashTable< Key, Value, Hash, Equal >::find(const Key& key) const
  {
    size_t idx = hasher_(key) % buckets_.getSize();
    auto res = find_node(idx, key);

    if (!res.first)
    {
      return cend();
    }

    return ConstIterator(&buckets_, buckets_.getSize(), idx, vasyakin::LCIter< PairType >(res.second->next_));
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< bool, vasyakin::detail::Node< std::pair< Key, Value > >* >
    HashTable< Key, Value, Hash, Equal >::find_node(size_t ind, const Key& key) const
  {
    const auto& chain = buckets_[ind];
    auto prev = chain.fake_node_;
    auto curr = prev->next_;

    while (curr != chain.fake_node_)
    {
      if (equal_(curr->value().first, key))
      {
        return {true, prev};
      }

      prev = curr;
      curr = curr->next_;
    }

    return {false, nullptr};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator
    HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    return Iterator(&buckets_, buckets_.getSize(), 0);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator
    HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return Iterator();
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator
    HashTable< Key, Value, Hash, Equal >::begin() const noexcept
  {
    return ConstIterator(&buckets_, buckets_.getSize(), 0);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator
    HashTable< Key, Value, Hash, Equal >::end() const noexcept
  {
    return ConstIterator();
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator
    HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    return ConstIterator(&buckets_, buckets_.getSize(), 0);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator
    HashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return ConstIterator();
  }
}

#endif
