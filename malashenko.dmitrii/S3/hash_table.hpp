#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <common/list/list.hpp>
namespace malashenko
{
  template< class T >
  class Equal {
    bool operator()(const T& lhs, const T& rhs);
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    HashTable();
    HashTable(const size_t& size);
    HashTable(const HashTable< Key, Value, Hash, Equal >& rhs);
    HashTable(HashTable< Key, Value, Hash, Equal >&& rhs);
    HashTable< Key, Value, Hash, Equal >& operator=(const HashTable< Key, Value, Hash, Equal >& rhs);
    HashTable< Key, Value, Hash, Equal >& operator=(HashTable< Key, Value, Hash, Equal >&& rhs);
    ~HashTable();

    void add(const Key& key, const Value& value);
    Value drop(const Key& key);

    bool has(const Key& key) const;
    Value get(const Key& key) const;
    Value operator[](const Key& key);
    const Value operator[](const Key& key) const;
    void rehash(const size_t& newSize);
    size_t size() const noexcept;
    void swap(HashTable< Key, Value, Hash, Equal >& rhs)
  private:
    size_t size_;
    List< std::pair< Key, Value > >* slots_[];
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< class Key, class Value, class Hash, class Equal >::~HashTable()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      slots[i].clear();
    }
    delete[] slots;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< class Key, class Value, class Hash, class Equal >::HashTable():
    slots_(new List< std::pair< Key, Value > >*[1]),
    size_(1)
  {
    slots_[0] = nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< class Key, class Value, class Hash, class Equal >::HashTable(const size_t& size):
    slots_(size ? new List< std::pair< Key, Value > >*[size] : nullptr),
    size_(size)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      slots_[i] = nullptr;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< class Key, class Value, class Hash, class Equal >::HashTable(const HashTable< Key, Value, Hash, Equal >& rhs)
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
  HashTable< class Key, class Value, class Hash, class Equal >::HashTable(HashTable< Key, Value, Hash, Equal >&& rhs)
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
  void HashTable< class Key, class Value, class Hash, class Equal >::swap(HashTable< Key, Value, Hash, Equal >& rhs)
  {
    using std::swap;
    std::swap(slots_, rhs.slots_);
    std::swap(size_, rhs.size_);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< class Key, class Value, class Hash, class Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< class Key, class Value, class Hash, class Equal >::add(const Key& key, const Value& value)
  {
    Hash hasher;
    Equal eq;
    size_t pos = hasher(key) % size_;
    if (!slots_[pos])
    {
      slots_[pos] = new List< std::pair< Key, Value > >();
    } else
    {
      for (LIter< std::pair< Key, Value > > start = slots_[pos]->begin(); start != slots_[pos]->end(); ++start)
      {
        if (eq(start->first(), key))
        {
          throw std::invalid_argument("This key is already in the table");
        }
      }
    }
    slots_[pos]->push_back({key, value});
  }


  template< class T >
  bool Equal< T >::operator()(const T& lhs, const T& rhs)
  {
    return lhs == rhs;
  }
}




#endif