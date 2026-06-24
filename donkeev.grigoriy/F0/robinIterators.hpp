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

    bool operator==(const RobinIter&) const noexcept;
    bool operator!=(const RobinIter&) const noexcept;
    
  private:
    size_t index_ = 0;
    Table* table_ = nullptr;

    RobinIter(size_t, Table*) noexcept;
  };

  template< class Key, class Value, class Hash, class Equal >
  class RobinCIter
  {
    friend class RobinTable<Key, Value, Hash, Equal>;

    using Table = RobinTable<Key, Value, Hash, Equal>;
    using Node = RobinNode<Key, Value>;
    
  public:
    RobinCIter();
    
    const Node& operator*() const noexcept;
    const Node* operator->() const noexcept;

    RobinCIter& operator++() noexcept;
    RobinCIter operator++(int) noexcept;

    RobinCIter& operator--() noexcept;
    RobinCIter operator--(int) noexcept;

    bool operator==(const RobinCIter&) const noexcept;
    bool operator!=(const RobinCIter&) const noexcept;
    
  private:
    size_t index_ = 0;
    const Table* table_ = nullptr;

    RobinCIter(size_t, const Table*) noexcept;
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
      size_t cap = table_->slots_.getsize();
      ++index_;
      while (index_ < cap && !table_->slots_[index_].isOccupied_)
      {
        ++index_;
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
        --index_;
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

  template< class Key, class Value, class Hash, class Equal >
  bool RobinIter< Key, Value, Hash, Equal >::operator==(const RobinIter& rhs) const noexcept
  {
    return index_ == rhs.index_ && table_ == rhs.table_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool RobinIter< Key, Value, Hash, Equal >::operator!=(const RobinIter& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinIter<Key, Value, Hash, Equal>::RobinIter(size_t index, Table* table) noexcept:
    index_(index),
    table_(table)
  {}

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter<Key, Value, Hash, Equal>::RobinCIter():
    index_(0),
    table_(nullptr)
  {}

  template< class Key, class Value, class Hash, class Equal >
  const RobinNode<Key, Value>& RobinCIter<Key, Value, Hash, Equal>::operator*() const noexcept
  {
    return table_->slots_[index_];
  }

  template< class Key, class Value, class Hash, class Equal >
  const RobinNode<Key, Value>* RobinCIter<Key, Value, Hash, Equal>::operator->() const noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter<Key, Value, Hash, Equal>& RobinCIter<Key, Value, Hash, Equal>::operator++() noexcept
  {
    if (table_)
    {
      size_t cap = table_->slots_.getSize();
      ++index_;
      while (index_ < cap && !table_->slots_[index_].isOccupied_)
      {
        ++index_;
      }
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter<Key, Value, Hash, Equal> RobinCIter<Key, Value, Hash, Equal>::operator++(int) noexcept
  {
    RobinCIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter<Key, Value, Hash, Equal>& RobinCIter<Key, Value, Hash, Equal>::operator--() noexcept
  {
    if (table_)
    {
      while (index_ > 0)
      {
        --index_;
        if (table_->slots_[index_].isOccupied_)
        {
          break;
        }
      }
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter<Key, Value, Hash, Equal> RobinCIter<Key, Value, Hash, Equal>::operator--(int) noexcept
  {
    RobinCIter tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool RobinCIter<Key, Value, Hash, Equal>::operator==(const RobinCIter& rhs) const noexcept
  {
    return index_ == rhs.index_ && table_ == rhs.table_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool RobinCIter<Key, Value, Hash, Equal>::operator!=(const RobinCIter& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter<Key, Value, Hash, Equal>::RobinCIter(size_t index, const Table* table) noexcept:
    index_(index),
    table_(table)
  {}
}
#endif
