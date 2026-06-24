#ifndef ROBIN_HASHTABLE_HPP
#define ROBIN_HASHTABLE_HPP

#include <cassert>

#include "robinNode.hpp"
#include "robinIterators.hpp"
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

    ~RobinTable() = default;

    RobinTable& operator=(const RobinTable&);
    RobinTable& operator=(RobinTable&&) noexcept;
    Value& operator[](const Key&);

    Value& at(const Key&);
    const Value& at(const Key&) const;

    bool contains(const Key&) const;
    
    void insert(const Key&, const Value&);
    Value remove(const Key&);
    void rehash(size_t);
    
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
    slots_ = topit::Vector< Node >(capacity, Node());
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
    
    insert(key, Value{});
    return findNode(key).second->value_;
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
  bool RobinTable<Key, Value, Hash, Equal>::contains(const Key& key) const
  {
    return findNode(key).second != nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  void RobinTable<Key, Value, Hash, Equal>::insert(const Key& key, const Value& value)
  {
    if (static_cast<double>(size_) / slots_.size() >= 0.75)
    {
      rehash(slots_.size() * 2);
    }
    
    size_t cap = slots_.size();
    size_t index = hasher_(key) % cap;
    
    Node toAdd{key, value, true, 0};
    
    for (size_t i = 0; i < cap; ++i)
    {
      if (!slots_[index].isOccupied_)
      {
        slots_[index] = toAdd;
        size_++;
        return;
      }
        
      if (equal_(slots_[index].key_, key))
      {
        slots_[index].value_ = value;
        return;
      }
        
      if (toAdd.psl_ > slots_[index].psl_)
      {
        std::swap(toAdd, slots_[index]);
      }
        
      toAdd.psl_++;
      index = (index + 1) % cap;
    }
    
    throw std::runtime_error("Overflow");
  }

  template< class Key, class Value, class Hash, class Equal >
  Value RobinTable<Key, Value, Hash, Equal>::remove(const Key& key)
  {
    std::pair< size_t, Node* > result = findNode(key);
    Node* toDel = result.second;
    size_t currId = result.first;
    
    if (!toDel)
    {
      throw std::out_of_range("Invalid key");
    }
    
    Value saved = std::move(toDel->value_);
    
    size_t cap = slots_.getSize();
    size_t nextId = (currId + 1) % cap;
    
    while (slots_[nextId].isOccupied_ && slots_[nextId].psl_ != 0)
    {
      slots_[currId] = slots_[nextId];
      slots_[currId].psl_--;
      
      currId = nextId;
      nextId = (nextId + 1) % cap;
    }
    
    slots_[currId] = Node{};
    size_--;
    
    return saved;
  }

  template< class Key, class Value, class Hash, class Equal >
  void RobinTable<Key, Value, Hash, Equal>::rehash(size_t newCapacity)
  {
    RobinTable tmp(newCapacity);
    
    size_t cap = slots_.size();
    for (size_t i = 0; i < cap; ++i)
    {
      if (slots_[i].isOccupied_)
      {
        tmp.insert(slots_[i].key_, slots_[i].value_);
      }
    }
    
    swap(tmp);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t RobinTable<Key, Value, Hash, Equal>::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t RobinTable<Key, Value, Hash, Equal>::capacity() const
  {
    return slots_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  bool RobinTable<Key, Value, Hash, Equal>::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void RobinTable<Key, Value, Hash, Equal>::clear()
  {
    for (size_t i = 0; i < slots_.size(); ++i)
    {
      slots_[i] = Node{};
    }

    size_ = 0;
  }

template< class Key, class Value, class Hash, class Equal >
void
RobinTable<Key, Value, Hash, Equal>::swap(RobinTable& other) noexcept
{
    std::swap(slots_, other.slots_);
    std::swap(size_, other.size_);
    std::swap(hasher_, other.hasher_);
    std::swap(equal_, other.equal_);
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
      currPsl++;
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