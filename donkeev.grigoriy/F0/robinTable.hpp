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

    std::pair< size_t, Node* > findNode(const Key&);
    std::pair< size_t, const Node* > findNode(const Key&) const;
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

  template< class Key, class Value, class Hash, class Equal >
  Value& RobinTable<Key, Value, Hash, Equal>::operator[](const Key& key)
  {
    std::pair< size_t, Node > result = findNode(key);
    if (result.second)
    {
      return result.second->value_;
    }
    
    insert(k, Value{});
    return findNode(k).second->value_;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& RobinTable<Key, Value, Hash, Equal>::at(const Key& key)
  {
    auto result = findNode(key);
    if (result.second)
    {
      return result.second->value_;
    }

    throw std::out_of_range("No such elemnt");
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& RobinTable<Key, Value, Hash, Equal>::at(const Key& key) const
  {
    auto result = findNode(key);
    if (result.second)
    {
      return result.second->value_;
    }
    throw std::out_of_range("No such elemnt");
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair<size_t, typename RobinTable<Key, Value, Hash, Equal>::Node*>
  RobinTable<Key, Value, Hash, Equal>::findNode(const Key& key)
  {
    if (empty())
    {
      return {0, nullptr};
    }
    
    size_t cap = slots_.getSize();
    size_t index = hasher_(key) % cap;
    int currPsl = 0;
    
    for (size_t i = 0; i < cap; ++i)
    {
      Node& node = slots_[index];
      
      if (!node.isOccupied_)
      {
        return {index, nullptr};
      }
      
      if (currPsl > node.psl_)
      {
        return {index, nullptr};
      }
      
      if (equal_(key, node.key_))
      {
        return {index, &slots_[index]};
      }
      
      index = (index + 1) % cap;
      cur_psl++;
    }
    
    return {cap, nullptr};
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair<size_t, const typename RobinTable<Key, Value, Hash, Equal>::Node*>
  RobinTable<Key, Value, Hash, Equal>::findNode(const Key& key) const
  {
    if (empty())
    {
      return {0, nullptr};
    }
    
    size_t cap = slots_.size();
    size_t index = hasher_(key) % cap;
    int cur_psl = 0;
    
    for (size_t i = 0; i < cap; ++i)
    {
      const Node& node = slots_[index];
      
      if (!node.isOccupied_)
      {
        return {index, nullptr};
      }
      
      if (cur_psl > node.psl_)
      {
        return {index, nullptr};
      }
      
      if (equal_(key, node.key_))
      {
        return {index, &slots_[index]};
      }
      
      index = (index + 1) % cap;
      cur_psl++;
    }
    
    return {cap, nullptr};
  }
  
}

#endif