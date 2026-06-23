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
      
  public:
    RobinTable() = delete;

    RobinTable(const RobinTable&);
    RobinTable(RobinTable&&) noexcept;

    explicit RobinTable(size_t);

    ~RobinHashTable() = default;

    RobinTable& operator=(const RobinTable& other);
    RobinTable& operator=(RobinTable&& other) noexcept;
    Value& operator[](const Key& k);

    Value& at(const Key& id);
    const Value& at(const Key& id) const;

    bool contains(const Key& k) const;
    
    void insert(const Key& k, const Value& v);
    Value remove(const Key& k);
    void rehash(size_t slots);
    
    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();

  private:
    topit::Vector< Node > slots_;
    size_t size_;
    Hash hasher_;
    Equal equal_;

    Node& findNode(const Key key&);
  };

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
}

#endif