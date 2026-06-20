#ifndef HASH_TABLE
#define HASH_TABLE
#include <list/list.hpp>
#include "hash_table_iter.hpp"
#include "hash_table_cIter.hpp"
namespace malashenko
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTableIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIter;

  template< class T >
  struct Equal {
    bool operator()(const T& lhs, const T& rhs);
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    using ht_t = HashTable<Key, Value, Hash, Equal>;
    using htIter_t = HashTableIter<Key, Value, Hash, Equal>;
    using htConstIter_t = HashTableConstIter<Key, Value, Hash, Equal>;

    HashTable();
    HashTable(size_t size);
    HashTable(const ht_t& rhs);
    HashTable(ht_t&& rhs) noexcept;
    ~HashTable();
    ht_t& operator=(const ht_t& rhs);
    ht_t& operator=(ht_t&& rhs) noexcept;

    std::pair< htIter_t, bool > insert(const Key& key, const Value& value);
    std::pair< htIter_t, bool > insert(const Key& key, Value&& value);

    size_t erase(const Key& key);


    bool contains(const Key& key) const;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    htIter_t begin();
    htIter_t end();
    htConstIter_t cbegin() const;
    htConstIter_t cend() const;

    void rehash(size_t newSize);
    size_t size() const noexcept;
    void swap(ht_t& rhs);
  private:
    friend class HashTableIter<Key, Value, Hash, Equal>;
    size_t size_;
    List< std::pair< Key, Value > >** slots_;

    template<class V>
    std::pair< htIter_t, bool > insert_impl(const Key& key, V&& value);

  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      delete slots_[i];
    }
    delete[] slots_;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    size_(1),
    slots_(new List< std::pair< Key, Value > >*[1])
  {
    slots_[0] = nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t size):
    size_(size),
    slots_(size ? new List< std::pair< Key, Value > >*[size] : nullptr)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      slots_[i] = nullptr;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const ht_t& rhs):
    HashTable(rhs.size_)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      slots_[i] = rhs.slots_[i] ? new List< std::pair< Key, Value > >(*rhs.slots_[i]) : nullptr;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(ht_t&& rhs) noexcept:
    size_(std::exchange(rhs.size_, 0)),
    slots_(std::exchange(rhs.slots_, nullptr))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const ht_t& rhs)
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    HashTable tmp(rhs);
    swap(tmp);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(ht_t&& rhs) noexcept
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    HashTable tmp(std::move(rhs));
    swap(tmp);
    return *this;
  }


  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(ht_t& rhs)
  {
    using std::swap;
    std::swap(slots_, rhs.slots_);
    std::swap(size_, rhs.size_);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  template<class V>
  std::pair< HashTableIter< Key, Value, Hash, Equal >, bool >
  HashTable< Key, Value, Hash, Equal >::insert_impl(const Key& key, V&& value)
  {

    Hash hasher;
    Equal eq;
    size_t pos = hasher(key) % size_;

    if (!slots_[pos])
    {
      slots_[pos] = new List< std::pair< Key, Value > >();
    }
    else
    {
      for (LIter< std::pair< Key, Value > > start = slots_[pos]->begin(); start != slots_[pos]->end(); ++start)
      {
        if (eq(start->first, key))
        {
          return {HashTableIter<Key, Value, Hash, Equal>(this, pos, start), false};
        }
      }
    }
    slots_[pos]->push_back({key, std::forward< V >(value)});

    LIter< std::pair< Key, Value > > list_it = slots_[pos]->end();
    --list_it;
    return { HashTableIter<Key, Value, Hash, Equal>(this, pos, list_it), true };
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< HashTableIter< Key, Value, Hash, Equal >, bool >
  HashTable< Key, Value, Hash, Equal >::insert(const Key& key, const Value& value)
  {
    return insert_impl(key, value);
  }


  template< class Key, class Value, class Hash, class Equal >
  std::pair< HashTableIter< Key, Value, Hash, Equal >, bool >
  HashTable< Key, Value, Hash, Equal >::insert(const Key& key, Value&& value)
  {
    return insert_impl(key, std::move(value));
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::contains(const Key& key) const
  {
    Hash hasher;
    Equal eq;
    size_t pos = hasher(key) % size_;
    if (slots_[pos])
    {
      for (LIter< std::pair< Key, Value > > start = slots_[pos]->begin(); start != slots_[pos]->end(); ++start)
      {
        if (eq(start->first, key))
        {
          return true;
        }
      }
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const HashTable& >(*this).at(key));
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    Hash hasher;
    Equal eq;
    size_t pos = hasher(key) % size_;

    if (!slots_[pos])
    {
      throw std::invalid_argument("Key not found");
    }

    for (LIter< std::pair< Key, Value > > start = slots_[pos]->begin(); start != slots_[pos]->end(); ++start)
    {
      if (eq(start->first, key))
      {
        return start->second;
      }
    }
    throw std::invalid_argument("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    if (!contains(key))
    {
      insert(key, Value());
    }
    return at(key);
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](Key&& key)
  {
    if (!contains(std::move(key)))
    {
      insert(std::move(key), Value());
    }
    return at(std::move(key));
  }


  template< class Key, class Value, class Hash, class Equal >
  HashTableIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      if (slots_[i] && !slots_[i]->empty())
      {
        return htIter_t(this, i, slots_[i]->begin());
      }
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return htIter_t(this, size_, LIter<std::pair<Key, Value>>());
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    for (size_t i = 0; i < size_; ++i)
    {
      if (slots_[i] && !slots_[i]->empty())
      {
        return htConstIter_t(this, i, slots_[i]->cbegin());
      }
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return htConstIter_t(this, size_, LCIter<std::pair<Key, Value>>());
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t newSize)
  {
    ht_t newTable(newSize);
    for (htIter_t start = begin(); start != end(); ++start)
    {
      newTable.insert(start->first, start->second);
    }
    swap(newTable);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key)
  {


    Hash hasher;
    Equal eq;
    size_t pos = hasher(key) % size_;

    for (LIter< std::pair< Key, Value > > start = slots_[pos]->begin(); start != slots_[pos]->end(); ++start)
    {
      if (eq(start->first, key))
      {
        slots_[pos]->cut(start);
        return 1;
      }
    }
    return 0;
  }

  template< class T >
  bool Equal< T >::operator()(const T& lhs, const T& rhs)
  {
    return lhs == rhs;
  }
}

#endif
