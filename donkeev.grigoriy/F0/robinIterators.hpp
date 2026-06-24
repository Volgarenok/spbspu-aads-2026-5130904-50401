#ifndef ROBINITERATORS_HPP
#define ROBINITERATORS_HPP

#include <cstddef>

#include "robinNode.hpp"

namespace donkeev
{
  template< class Key, class Value, class Hash, class Equal >
  class RobinTable;

  template< class Key, class Value, class Hash, class Equal >
  class RobinIter
  {
    friend class RobinTable<Key, Value, Hash, Equal>;

    using Table = RobinTable<Key, Value, Hash, Equal>;
    using Node = RobinNode< Key, Value >;
    
  public:
    RobinIter();
    
    Node& operator*() const noexcept;
    Node* operator->() const noexcept;

    RobinIter& operator++() noexcept;
    RobinIter operator++(int) noexcept;

    RobinIter& operator--() noexcept;
    RobinIter operator--(int) noexcept;

    bool operator==(const RobinIter& rhs) const noexcept;
    bool operator!=(const RobinIter& rhs) const noexcept;
    
  private:
    size_t index_ = 0;
    Table* table_ = nullptr;

    RobinIter(size_t, Table*);
  };

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal >::RobinIter():
    index_(0),
    table_(nullptr)
  {}

  template< class Key, class Value, class Hash, class Equal >
  RobinNode< Key, Value >& RobinIter< Key, Value, Hash, Equal >::operator*() const noexcept
  {
    return table_->slots_[index_];
  }
  
  template< class Key, class Value, class Hash, class Equal >
  RobinNode< Key, Value >* RobinIter< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal >& RobinIter< Key, Value, Hash, Equal >::operator++() noexcept
  {
    if (table_)
    {
      size_t cap = table_->slots_.size();
      ++index_;
      while (index_ < cap && !table_->slots_[index_].isOccupied_)
      {
        ++index;
      }
    }

    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinIter< Key, Value, Hash, Equal >::operator++(int) noexcept
  {
    RobinIter tmp = *this;
    ++(*this);

    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal >& RobinIter< Key, Value, Hash, Equal >::operator--() noexcept
  {
    if (table_)
    {
      while (index_ > 0)
      {
        --index;
        if (table_->slots_[index_].isOccupied_)
        {
          break;
        }
      }
    }

    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinIter< Key, Value, Hash, Equal >::operator--(int) noexcept
  {
    RobinIter tmp = *this;
    --(*this);

    return tmp;
  }
}
#endif
