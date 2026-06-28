#ifndef MY_ROBIN_CITER_HPP
#define MY_ROBIN_CITER_HPP

#include <cstddef>
#include <stdexcept>

#include "robin_node.hpp"

namespace zubarev
{
  template< class Key, class Value, class Hash, class Equal >
  class RobinHashTable;

  template< class Key, class Value, class Hash, class Equal >
  class RobinCIter
  {
    friend class RobinHashTable< Key, Value, Hash, Equal >;
    using Node = RobinNode< Key, Value >;
    using Table = RobinHashTable< Key, Value, Hash, Equal >;

  public:
    RobinCIter() noexcept;

    const Node& operator*() const;
    const Node* operator->() const noexcept;

    RobinCIter& operator++();
    RobinCIter operator++(int);

    RobinCIter& operator--();
    RobinCIter operator--(int);

    bool operator!=(const RobinCIter&) const;
    bool operator==(const RobinCIter&) const;

  private:
    RobinCIter(size_t, const Table*);
    size_t el_idx_;
    const Table* table_;
  };

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal >::RobinCIter(size_t el_idx, const Table* table):
    el_idx_(el_idx),
    table_(table)
  {}

  template< class Key, class Value, class Hash, class Equal >
  const RobinNode< Key, Value >& RobinCIter< Key, Value, Hash, Equal >::operator*() const
  {
    if (el_idx_ > table_->capacity()) {
      throw std::out_of_range("Invalid node in overflow");
    }
    return table_->slots_[el_idx_];
  }

  template< class Key, class Value, class Hash, class Equal >
  const RobinNode< Key, Value >* RobinCIter< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return std::addressof(operator*());
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal >& RobinCIter< Key, Value, Hash, Equal >::operator++()
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
  RobinCIter< Key, Value, Hash, Equal > RobinCIter< Key, Value, Hash, Equal >::operator++(int)
  {
    RobinCIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal >& RobinCIter< Key, Value, Hash, Equal >::operator--()
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
  RobinCIter< Key, Value, Hash, Equal > RobinCIter< Key, Value, Hash, Equal >::operator--(int)
  {
    RobinCIter tmp = *this;
    --(*this);
    return tmp;
  }
  template< class Key, class Value, class Hash, class Equal >
  bool RobinCIter< Key, Value, Hash, Equal >::operator!=(const RobinCIter& rhs) const
  {
    return !(*this == rhs);
  }
  template< class Key, class Value, class Hash, class Equal >
  bool RobinCIter< Key, Value, Hash, Equal >::operator==(const RobinCIter& rhs) const
  {
    if (table_ != rhs.table_) {
      return false;
    }
    return el_idx_ == rhs.el_idx_;
  }
}
#endif
