#ifndef HASH_TABLE
#define HASH_TABLE
#include <list/list.hpp>
#include "hash_table_iter.hpp"
namespace malashenko
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTableIter;
  template< class T >
  struct Equal {
    bool operator()(const T& lhs, const T& rhs);
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    using ht_t = HashTable<Key, Value, Hash, Equal>;
    using htIter_t = HashTableIter<Key, Value, Hash, Equal>;
    HashTable();
    HashTable(const size_t& size);
    HashTable(const ht_t& rhs);
    HashTable(ht_t&& rhs);
    ht_t& operator=(const ht_t& rhs);
    ht_t& operator=(ht_t&& rhs);
    ~HashTable();

    void add(const Key& key, const Value& value);
    Value drop(const Key& key);

    bool has(const Key& key) const;

    Value& get(const Key& key);
    const Value& get(const Key& key) const;

    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;

    htIter_t begin();
    htIter_t end();

    void rehash(const size_t& newSize);
    size_t size() const noexcept;
    void swap(ht_t& rhs);
  private:
    friend class HashTableIter<Key, Value, Hash, Equal>;
    size_t size_;
    List< std::pair< Key, Value > >** slots_;
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
  HashTable< Key, Value, Hash, Equal >::HashTable(const size_t& size):
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
      slots_[i] = rhs.slots_[i] ? new List< Key >(*rhs.slots_[i]) : nullptr;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(ht_t&& rhs):
    slots_(std::move(rhs.slots_)),
    size_(std::move(rhs.size_))
  {
    rhs.slots_ = nullptr;
    rhs.size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const ht_t& rhs)  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    HashTable tmp(rhs);
    swap(tmp);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(ht_t&& rhs)
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
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
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
          throw std::invalid_argument("This key is already in the table");
        }
      }
    }
    slots_[pos]->push_back({key, value});
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
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
  Value& HashTable< Key, Value, Hash, Equal >::get(const Key& key)
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
  const Value& HashTable< Key, Value, Hash, Equal >::get(const Key& key) const
  {
    return const_cast< Value& >(static_cast< const HashTable& >(*this).get(key));
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    if (!has(key))
    {
      add(key, Value());
    }
    return get(key);
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key) const
  {
    return get(key);
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
  void HashTable< Key, Value, Hash, Equal >::rehash(const size_t& newSize)
  {
    ht_t newTable(newSize);
    for (htIter_t start = begin(); start != end(); ++start)
    {
      newTable.add(start->first, start->second);
    }
    swap(newTable);
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    if (!has(key))
    {
      throw std::invalid_argument("Key not found");
    }

    Hash hasher;
    Equal eq;
    size_t pos = hasher(key) % size_;

    Value res = Value();
    for (LIter< std::pair< Key, Value > > start = slots_[pos]->begin(); start != slots_[pos]->end(); ++start)
    {
      if (eq(start->first, key))
      {
        res = start->second;
        slots_[pos]->cut(start);
        break;
      }
    }
    return res;
  }

  template< class T >
  bool Equal< T >::operator()(const T& lhs, const T& rhs)
  {
    return lhs == rhs;
  }
}




#endif