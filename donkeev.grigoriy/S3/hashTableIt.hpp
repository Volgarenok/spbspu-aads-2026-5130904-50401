#ifndef HASHTABLEIT_HPP
#define HASHTABLEIT_HPP

#include "Node.hpp"

namespace donkeev
{
  template<class Key, class Value, class Hash, class Equal>
  class HashTable;

  template<class Key, class Value, class Hash, class Equal>
  struct HTIt
  {
    HTIt() = default;
    explicit HTIt(donkeev::Node< Key, Value >*);

    HTIt< Key, Value, Hash, Equal >& operator++() noexcept;
    HTIt< Key, Value, Hash, Equal > operator++(int) noexcept;

    HTIt< Key, Value, Hash, Equal >& operator--() noexcept;
    HTIt< Key, Value, Hash, Equal > operator--(int) noexcept;

    std::pair< Key, Value >& operator*() const noexcept;
    std::pair< Key, Value >* operator->() const noexcept;

    bool operator==(const HTIt< Key, Value, Hash, Equal >&) const noexcept;
    bool operator!=(const HTIt< Key, Value, Hash, Equal >&) const noexcept;
    bool operator>(const HTIt< Key, Value, Hash, Equal >&) const noexcept;
    bool operator>=(const HTIt< Key, Value, Hash, Equal >&) const noexcept;
    bool operator<(const HTIt< Key, Value, Hash, Equal >&) const noexcept;
    bool operator<=(const HTIt< Key, Value, Hash, Equal >&) const noexcept;
  private:
    donkeev::Node< Key, Value >* node_;
  };
}
#endif
