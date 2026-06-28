#ifndef MY_ROBIN_ITER_HPP
#define MY_ROBIN_ITER_HPP

#include <cstddef>
#include <stdexcept>

#include "robin_node.hpp"
namespace zubarev
{
  template< class Key, class Value, class Hash, class Equal >
  class RobinHashTable;

  template< class Key, class Value, class Hash, class Equal >
  class RobinIter
  {
    friend class RobinHashTable< Key, Value, Hash, Equal >;
    using Node = RobinNode< Key, Value >;
    using Table = RobinHashTable< Key, Value, Hash, Equal >;

  public:
    RobinIter() noexcept;

    Node& operator*() const;
    Node* operator->() const noexcept;

    RobinIter& operator++();
    RobinIter operator++(int);

    RobinIter& operator--();
    RobinIter operator--(int);

    bool operator!=(const RobinIter&) const;
    bool operator==(const RobinIter&) const;

  private:
    RobinIter(size_t, Table*);
    size_t el_idx_;
    Table* table_;
  };

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal >::RobinIter(size_t el_idx, Table* table):
    el_idx_(el_idx),
    table_(table)
  {}

  template< class Key, class Value, class Hash, class Equal >
  RobinNode< Key, Value >& RobinIter< Key, Value, Hash, Equal >::operator*() const
  {
    if (!table_ || el_idx_ >= table_->capacity_) {
      throw std::out_of_range("Invalid iterator dereference");
    }

    if (!table_->slots_[el_idx_].occupied_) {
      throw std::logic_error("Dereferencing empty slot");
    }
    return table_->slots_[el_idx_];
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinNode< Key, Value >* RobinIter< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return std::addressof(operator*());
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal >& RobinIter< Key, Value, Hash, Equal >::operator++()
  {
    if (!table_) {
      return *this;
    }
    ++el_idx_;
    while (el_idx_ < table_->capacity_ && !table_->slots_[el_idx_].occupied_) {
      ++el_idx_;
    }
    return *this;
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinIter< Key, Value, Hash, Equal >::operator++(int)
  {
    RobinIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal >& RobinIter< Key, Value, Hash, Equal >::operator--()
  {
    if (!table_ || el_idx_ == 0) {
      throw std::out_of_range("Iterator cannot be decremented");
    }

    do {
      --el_idx_;
    } while (el_idx_ > 0 && !table_->slots_[el_idx_].occupied_);

    return *this;
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinIter< Key, Value, Hash, Equal >::operator--(int)
  {
    RobinIter tmp = *this;
    --(*this);
    return tmp;
  }
  template< class Key, class Value, class Hash, class Equal >
  bool RobinIter< Key, Value, Hash, Equal >::operator!=(const RobinIter& rhs) const
  {
    return !(*this == rhs);
  }
  template< class Key, class Value, class Hash, class Equal >
  bool RobinIter< Key, Value, Hash, Equal >::operator==(const RobinIter& rhs) const
  {
    if (table_ != rhs.table_) {
      return false;
    }
    return el_idx_ == rhs.el_idx_;
  }
}
#endif
