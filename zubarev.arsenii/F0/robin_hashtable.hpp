#ifndef ROBIN_HASHTABLE_HPP
#define ROBIN_HASHTABLE_HPP

#include <cstddef>

#include "robin_node.hpp"
#include <my_vector/top-it-vector.hpp>
#include "robin_iter.hpp"
#include "robin_citer.hpp"

namespace zubarev
{
  template< class Key, class Value, class Hash, class Equal >
  class RobinHashTable
  {
    friend class RobinIter< Key, Value, Hash, Equal >;
    friend class RobinCIter< Key, Value, Hash, Equal >;

    using Node = RobinNode< Key, Value >;
    using Iter = RobinIter< Key, Value, Hash, Equal >;
    using CIter = RobinCIter< Key, Value, Hash, Equal >;
    using Table = RobinHashTable< Key, Value, Hash, Equal >;

  public:
    explicit RobinHashTable(size_t cap = 16);
    ~RobinHashTable() = default;
    RobinHashTable(const RobinHashTable& table);
    RobinHashTable(RobinHashTable&& table) noexcept;
    RobinHashTable& operator=(const RobinHashTable& other);
    RobinHashTable& operator=(RobinHashTable&& other) noexcept;

    Value& operator[](const Key& k);
    Value& at(const Key& id);
    const Value& at(const Key& id) const;
    CIter find(const Key& id) const;
    Iter find(const Key& id);
    void swap(Table& rhs) noexcept;

    Iter add(const Key& k, Value v);
    Value drop(const Key& k);
    bool has(const Key& k) const;
    void rehash(size_t slots);

    Iter begin();
    Iter end();

    CIter cbegin() const;
    CIter cend() const;

    CIter begin() const;
    CIter end() const;

    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    double load_factor() const;

  private:
    size_t size_;
    size_t capacity_;
    Vector< Node > slots_;
    Hash hasher_;
    Equal equal_;

    Value drop_impl(const Key& k);

    std::pair< size_t, Node* > find_el(const Key& k) noexcept;

    const std::pair< size_t, const Node* > find_el(const Key& k) const noexcept;
  };

  template< class Key, class Value, class Hash, class Equal >
  std::pair< size_t, RobinNode< Key, Value >* >
  RobinHashTable< Key, Value, Hash, Equal >::find_el(const Key& k) noexcept
  {
    auto res = const_cast< const Table* >(this)->find_el(k);
    return {res.first, const_cast< Node* >(res.second)};
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< size_t, const RobinNode< Key, Value >* >
  RobinHashTable< Key, Value, Hash, Equal >::find_el(const Key& k) const noexcept
  {
    if (empty()) {
      return {0, nullptr};
    }

    size_t index = hasher_(k) % capacity_;
    int cur_psl = 0;

    for (size_t i = 0; i < capacity_; ++i) {
      const Node& cur_node = slots_[index];
      if (cur_node.psl_ == -1) {
        return {index, nullptr};
      }

      if (!cur_node.occupied_) {
        return {index, nullptr};
      }

      if (cur_psl > cur_node.psl_) {
        return {index, nullptr};
      }
      if (equal_(k, cur_node.key_)) {
        return {index, std::addressof(slots_[index])};
      }
      index = (index + 1) % capacity_;
      cur_psl++;
    }
    return {capacity_, nullptr};
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(size_t capacity):
    size_(0),
    capacity_(capacity == 0 ? 8 : capacity),
    slots_(capacity_, Node()),
    hasher_(),
    equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(const RobinHashTable& table):
    size_(table.size_),
    capacity_(table.capacity_),
    slots_(table.capacity_, Node()),
    hasher_(table.hasher_),
    equal_(table.equal_)
  {
    for (size_t i = 0; i < table.capacity_; ++i) {
      slots_[i] = table.slots_[i];
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(RobinHashTable&& table) noexcept:
    size_(table.size_),
    capacity_(table.capacity_),
    slots_(std::move(table.slots_)),
    hasher_(std::move(table.hasher_)),
    equal_(std::move(table.equal_))
  {
    table.size_ = 0;
    table.capacity_ = 0;
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >&
  RobinHashTable< Key, Value, Hash, Equal >::operator=(const RobinHashTable& rhs)
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    Table cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >&
  RobinHashTable< Key, Value, Hash, Equal >::operator=(RobinHashTable&& rhs) noexcept
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    Table cpy(std::move(rhs));
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& RobinHashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
  {
    Node* el = find_el(k).second;
    if (el) {
      return el->val_;
    } else {

      return (add(k, Value{}))->val_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& RobinHashTable< Key, Value, Hash, Equal >::at(const Key& id)
  {

    Node* el = find_el(id).second;
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("RobinHashTable: index out of range");
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  const Value& RobinHashTable< Key, Value, Hash, Equal >::at(const Key& id) const
  {
    const Node* el = find_el(id).second;
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("RobinHashTable: index out of range");
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::find(const Key& id) const
  {
    auto res = find_el(id);
    if (res.second == nullptr) {
      return cend();
    }
    return CIter(res.first, this);
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::find(const Key& id)
  {
    auto res = find_el(id);
    if (res.second == nullptr) {
      return end();
    }
    return Iter(res.first, this);
  }
  template< class Key, class Value, class Hash, class Equal >
  void RobinHashTable< Key, Value, Hash, Equal >::swap(Table& rhs) noexcept
  {
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(slots_, rhs.slots_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(equal_, rhs.equal_);
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::begin()
  {
    size_t cap = capacity();
    for (size_t i = 0; i < cap; ++i) {
      if (slots_[i].occupied_) {
        return Iter(i, this);
      }
    }
    return end();
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter(capacity_, this);
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    size_t cap = capacity();
    for (size_t i = 0; i < cap; ++i) {
      if (slots_[i].occupied_) {
        return CIter(i, this);
      }
    }
    return cend();
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::cend() const
  {
    return CIter(capacity_, this);
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::begin() const
  {
    return cbegin();
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::end() const
  {
    return cend();
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::add(const Key& k, Value v)
  {
    if (load_factor() >= 0.75) {
      rehash(capacity_ * 2);
    }

    size_t index = hasher_(k) % capacity_;

    Node node_to_add(k, v, true, 0);

    for (size_t i = 0; i < capacity_; ++i) {
      if (!slots_[index].occupied_) {
        slots_[index] = node_to_add;
        size_++;
        return Iter(index, this);
      }

      if (equal_(slots_[index].key_, node_to_add.key_)) {
        slots_[index].val_ = node_to_add.val_;
        return Iter(index, this);
      }

      if (node_to_add.psl_ > slots_[index].psl_) {
        std::swap(node_to_add, slots_[index]);
      }

      node_to_add.psl_++;
      index = (index + 1) % capacity_;
    }
    throw std::runtime_error("RobinHashTable overflow: rehash failed or capacity logic "
                             "error");
  }
  template< class Key, class Value, class Hash, class Equal >
  Value RobinHashTable< Key, Value, Hash, Equal >::drop_impl(const Key& k)
  {
    std::pair< size_t, Node* > found_el = find_el(k);
    Node* node_to_del = found_el.second;
    size_t cur_id = found_el.first;
    if (node_to_del) {

      Value saved_val = node_to_del->val_;

      size_t next_id = (cur_id + 1) % capacity_;
      while (slots_[next_id].occupied_ && slots_[next_id].psl_ != 0) {
        slots_[cur_id] = slots_[next_id];
        slots_[cur_id].psl_--;

        cur_id = next_id;
        next_id = (next_id + 1) % capacity_;
      }

      slots_[cur_id] = Node();
      size_--;
      return saved_val;
    }
    throw std::out_of_range("Key not found in drop()");
  }

  template< class Key, class Value, class Hash, class Equal >
  Value RobinHashTable< Key, Value, Hash, Equal >::drop(const Key& k)
  {
    Table tmp(*this);
    Value result = tmp.drop_impl(k);
    swap(tmp);
    return result;
  }
  template< class Key, class Value, class Hash, class Equal >
  bool RobinHashTable< Key, Value, Hash, Equal >::has(const Key& k) const
  {
    const std::pair< size_t, const Node* > found_el = find_el(k);
    return found_el.second != nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  void RobinHashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    Table tmp(slots);

    for (size_t i = 0; i < capacity_; ++i) {
      if (slots_[i].occupied_) {

        Node clean_node(slots_[i].key_, slots_[i].val_, true, 0);

        tmp.add(clean_node.key_, clean_node.val_);
      }
    }

    swap(tmp);
  }
  template< class Key, class Value, class Hash, class Equal >
  size_t RobinHashTable< Key, Value, Hash, Equal >::size() const
  {
    return size_;
  }
  template< class Key, class Value, class Hash, class Equal >
  size_t RobinHashTable< Key, Value, Hash, Equal >::capacity() const
  {
    return capacity_;
  }
  template< class Key, class Value, class Hash, class Equal >
  bool RobinHashTable< Key, Value, Hash, Equal >::empty() const
  {
    return size_ == 0;
  }
  template< class Key, class Value, class Hash, class Equal >
  double RobinHashTable< Key, Value, Hash, Equal >::load_factor() const
  {
    return static_cast< double >(size_) / capacity_;
  }
}

#endif
