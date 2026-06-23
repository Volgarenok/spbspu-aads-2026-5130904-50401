#ifndef ROBIN_HASHTABLE_HPP
#define ROBIN_HASHTABLE_HPP

#include <cassert>

#include "robinNode.hpp"
#include "../common/top-it-vector.hpp"

namespace donkeev
{

  template< class Key, class Value, class Hash, class Equal >
  class RobinTable
  {
    using Node = RobinNode<Key, Value>;
    using Table = RobinTable<Key, Value, Hash, Equal>;
      
  public:
    RobinTable() = delete;

    RobinTable(const RobinTable&);
    RobinTable(RobinTable&&) noexcept;

    explicit RobinTable(size_t);

    ~RobinHashTable() = default;

    RobinTable& operator=(const RobinTable&);
    RobinTable& operator=(RobinTable&&) noexcept;
    Value& operator[](const Key&);

    Value& at(const Key&);
    const Value& at(const Key&) const;

    bool contains(const Key&) const;
    
    void insert(const Key&, const Value&);
    Value remove(const Key&);
    void rehash(ssize_t);
    
    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();

  private:
    topit::Vector< Node > slots_;
    size_t size_;
    Hash hasher_;
    Equal equal_;

    Node& findNode(const Key&);
    void swap(Table&) noexcept;
  };

  template< class Key, class Value, class Hash, class Equal >
  RobinTable<Key, Value, Hash, Equal>::RobinTable(const RobinTable& other):
    slots_(other.slots_),
    size_(other.size_),
    hasher_(other.hasher_),
    equal_(other.equal_)
  {}

  template< class Key, class Value, class Hash, class Equal >
  RobinTable<Key, Value, Hash, Equal>::RobinTable(RobinTable&& other) noexcept:
    slots_(std::move(other.slots_)),
    size_(other.size_),
    hasher_(std::move(other.hasher_)),
    equal_(std::move(other.equal_))
  {
    other.size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinTable<Key, Value, Hash, Equal>::RobinTable(size_t capacity):
    slots_(),
    size_(0),
    hasher_(),
    equal_()
  {
    assert(capacity > 0);
    slots_ = topit::Vector(capacity, Node());
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinTable<Key, Value, Hash, Equal>& RobinTable<Key, Value, Hash, Equal>::operator=(const RobinTable& other)
  {
    if (this != &other)
    {
      RobinTable copy(other);
      swap(copy);
    }

    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinTable<Key, Value, Hash, Equal>& RobinTable<Key, Value, Hash, Equal>::operator=(RobinTable&& other) noexcept
  {
    if (this != &other)
    {
      RobinTable copy(std::move(other));
      swap(copy);
    }

    return *this;
  }

  
}

#endif