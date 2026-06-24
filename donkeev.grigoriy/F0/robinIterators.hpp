#ifndef ROBINITERATORS_HPP
#define ROBINITERATORS_HPP

#include <cstddef>

#include "robinNode.hpp"

namespace donkeev
{
  template< class Key, class Value, class Hash, class Equal >
  class RobinTable;

  template< class Key, class Value >
  class RobinIter
  {
    friend class RobinTable<Key, Value, Hash, Equal>;

    using Table = RobinTable<Key, Value, Hash, Equal>;
    using Node = RobinNode< Key, Value >;
    
  public:
    RobinIter();
    
    Node& operator*() const;
    Node* operator->() const noexcept;

    RobinIter& operator++();
    RobinIter operator++(int);

    RobinIter& operator--();
    RobinIter operator--(int);

    bool operator==(const RobinIter& rhs) const;
    bool operator!=(const RobinIter& rhs) const;
    
  private:
    size_t index_ = 0;
    Table* table_ = nullptr;

    RobinIter(size_t, Table*);
  };

}
#endif
