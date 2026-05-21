#ifndef HASHTABLEIT_HPP
#define HASHTABLEIT_HPP

#include "iostream"
#include "top-it-vector.hpp"
#include "Node.hpp"

namespace donkeev
{
  template<class Key, class Value, class Hash, class Equal>
  class HashTable;

  template<class Key, class Value, class Hash, class Equal>
  struct HTIt
  {
    HTIt() = default;

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
    const topit::Vector< donkeev::Node< Key, Value > >* vector_;
    size_t currentId_;
    size_t dataSize_;

    explicit HTIt(const topit::Vector< donkeev::Node< Key, Value > >*, size_t);
  };

  template<class Key, class Value, class Hash, class Equal>
  HTIt< Key, Value, Hash, Equal >::HTIt(const topit::Vector< donkeev::Node< Key, Value > >* data, size_t currentId):
    vector_(data),
    currentId_(currentId),
    dataSize_(vector_->getSize())
  {}

  template<class Key, class Value, class Hash, class Equal>
  HTIt< Key, Value, Hash, Equal >& HTIt< Key, Value, Hash, Equal >::operator++() noexcept
  {
    ++currentId_;
    while (currentId_ < dataSize_ && (*vector_)[currentId_].isEmpty())
    {
      ++currentId_;
    }

    return *this;
  }
  template<class Key, class Value, class Hash, class Equal>
  HTIt< Key, Value, Hash, Equal > HTIt< Key, Value, Hash, Equal >::operator++(int) noexcept
  {
    HTIt< Key, Value, Hash, Equal > tmp = *this;

    this->operator++();
    return tmp;
  }

  template<class Key, class Value, class Hash, class Equal>
  HTIt< Key, Value, Hash, Equal >& HTIt< Key, Value, Hash, Equal >::operator--() noexcept
  {
    --currentId_;
    while (currentId_ < dataSize_ && (*vector_)[currentId_].isEmpty())
    {
      --currentId_;
    }

    return *this;
  }
  template<class Key, class Value, class Hash, class Equal>
  HTIt< Key, Value, Hash, Equal > HTIt< Key, Value, Hash, Equal >::operator--(int) noexcept
  {
    HTIt< Key, Value, Hash, Equal > tmp = *this;

    this->operator--();
    return tmp;
  }

  template<class Key, class Value, class Hash, class Equal>
  std::pair< Key, Value >& HTIt< Key, Value, Hash, Equal >::operator*() const noexcept
  {
    return (*vector_)[currentId_].data_;
  }
  template<class Key, class Value, class Hash, class Equal>
  std::pair< Key, Value >* HTIt< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return  &((*vector_)[currentId_]);
  }

  template<class Key, class Value, class Hash, class Equal>
  bool HTIt< Key, Value, Hash, Equal >::operator==(const HTIt< Key, Value, Hash, Equal >& otherIt) const noexcept
  {
    return currentId_ == otherIt.currentId_ && vector_ == otherIt.vector_;
  }
  template<class Key, class Value, class Hash, class Equal>
  bool HTIt< Key, Value, Hash, Equal >::operator==(const HTIt< Key, Value, Hash, Equal >& otherIt) const noexcept
  {
    return !(this == otherIt);
  }
  template<class Key, class Value, class Hash, class Equal>
  bool HTIt< Key, Value, Hash, Equal >::operator>(const HTIt< Key, Value, Hash, Equal >& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ > otherIt.currentId_;
  }
  template<class Key, class Value, class Hash, class Equal>
  bool HTIt< Key, Value, Hash, Equal >::operator>=(const HTIt< Key, Value, Hash, Equal >& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ >= otherIt.currentId_;
  }
  template<class Key, class Value, class Hash, class Equal>
  bool HTIt< Key, Value, Hash, Equal >::operator<(const HTIt< Key, Value, Hash, Equal >& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ < otherIt.currentId_;
  }
  template<class Key, class Value, class Hash, class Equal>
  bool HTIt< Key, Value, Hash, Equal >::operator<=(const HTIt< Key, Value, Hash, Equal >& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ <= otherIt.currentId_;
  }
}
#endif
