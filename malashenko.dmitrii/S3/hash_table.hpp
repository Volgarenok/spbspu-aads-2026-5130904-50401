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

    void add(const Key& k, const Value& v);
    Value drop(const Key& k);

    bool has(const Key& k) const;
    Value get(const Key& k) const;
    Value operator[](const Key& k);
    const Value operator[](const Key& k) const;

    void rehash(const size_t& newSize);
    size_t size() const noexcept;
    void swap(HashTable< Key, Value, Hash, Equal >& rhs)
  private:
    size_t k_;
    List< Key >* slots_[];
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< class Key, class Value, class Hash, class Equal >::~HashTable()
  {
    for (size_t i = 0; i < k; ++i)
    {
      slots[i].clear();
    }
    delete[] slots;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< class Key, class Value, class Hash, class Equal >::HashTable():
    slots_(new List< Key >[1]),
    k_(1)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< class Key, class Value, class Hash, class Equal >::HashTable(const size_t& size):
    slots_(size ? List< Key >[size] : nullptr),
    k_(size)
  {}

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
    std::swap(k_, rhs.k_);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< class Key, class Value, class Hash, class Equal >::size() const noexcept
  {
    return k;
  }



  template< class T >
  bool Equal< T >::operator()(const T& lhs, const T& rhs)
  {
    return lhs == rhs;
  }
}




#endif