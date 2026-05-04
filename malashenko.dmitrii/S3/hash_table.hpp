#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <common/list/list.hpp>
namespace malashenko
{
  template< class T >
  class Equal {
    bool operator(const T& lhs, const T& rhs);
  };


  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    HashTable();
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

    void rehash(const size_t& slots);
    size_t size() const noexcept;
    void swap(HashTable< Key, Value, Hash, Equal >& rhs)
  private:
    size_t k;
    List< Key >* slots[];
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
  HashTable< class Key, class Value, class Hash, class Equal >::HashTable()
  {
    slots = new List< Key >[8];
    k = 8;
  }


  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< class Key, class Value, class Hash, class Equal >::size() const noexcept
  {
    return k;
  }
}




#endif