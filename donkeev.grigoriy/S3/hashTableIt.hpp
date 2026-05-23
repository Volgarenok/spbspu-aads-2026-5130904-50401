#ifndef HASHTABLEIT_HPP
#define HASHTABLEIT_HPP

#include "iostream"
#include "top-it-vector.hpp"
#include "HTNode.hpp"

namespace donkeev
{
  template<class Key, class Value, class Hash, class Equal>
  class HashTable;

  template<class Key, class Value>
  struct HTIt
  {
    HTIt() = default;

    HTIt<Key, Value>& operator++() noexcept;
    HTIt<Key, Value> operator++(int) noexcept;

    HTIt<Key, Value>& operator--() noexcept;
    HTIt<Key, Value> operator--(int) noexcept;

    std::pair<Key, Value>& operator*() const noexcept;
    std::pair<Key, Value>* operator->() const noexcept;

    bool operator==(const HTIt<Key, Value>&) const noexcept;
    bool operator!=(const HTIt<Key, Value>&) const noexcept;
    bool operator>(const HTIt<Key, Value>&) const noexcept;
    bool operator>=(const HTIt<Key, Value>&) const noexcept;
    bool operator<(const HTIt<Key, Value>&) const noexcept;
    bool operator<=(const HTIt<Key, Value>&) const noexcept;
  private:
    const topit::Vector<donkeev::HTNode<Key, Value>>* vector_;
    size_t currentId_;
    size_t dataSize_;

    explicit HTIt(const topit::Vector<donkeev::HTNode<Key, Value>>*, size_t);
  };

  template<class Key, class Value>
  struct HTCIt
  {
    HTCIt() = default;

    HTCIt<Key, Value>& operator++() noexcept;
    HTCIt<Key, Value> operator++(int) noexcept;

    HTCIt<Key, Value>& operator--() noexcept;
    HTCIt<Key, Value> operator--(int) noexcept;

    const std::pair<Key, Value>& operator*() const noexcept;
    const std::pair<Key, Value>* operator->() const noexcept;

    bool operator==(const HTCIt<Key, Value>&) const noexcept;
    bool operator!=(const HTCIt<Key, Value>&) const noexcept;
    bool operator>(const HTCIt<Key, Value>&) const noexcept;
    bool operator>=(const HTCIt<Key, Value>&) const noexcept;
    bool operator<(const HTCIt<Key, Value>&) const noexcept;
    bool operator<=(const HTCIt<Key, Value>&) const noexcept;
  private:
    const topit::Vector<donkeev::HTNode<Key, Value>>* vector_;
    size_t currentId_;
    size_t dataSize_;

    explicit HTCIt(const topit::Vector<donkeev::HTNode<Key, Value>>*, size_t);
  };

  template<class Key, class Value>
  HTIt<Key, Value>::HTIt(const topit::Vector<donkeev::HTNode<Key, Value>>* data, size_t currentId):
    vector_(data),
    currentId_(currentId),
    dataSize_(vector_->getSize())
  {}

  template<class Key, class Value>
  HTIt<Key, Value>& HTIt<Key, Value>::operator++() noexcept
  {
    ++currentId_;
    while (currentId_ < dataSize_ && (*vector_)[currentId_].isEmpty())
    {
      ++currentId_;
    }

    return *this;
  }

  template<class Key, class Value>
  HTIt<Key, Value> HTIt<Key, Value>::operator++(int) noexcept
  {
    HTIt<Key, Value> tmp = *this;

    this->operator++();
    return tmp;
  }

  template<class Key, class Value>
  HTIt<Key, Value>& HTIt<Key, Value>::operator--() noexcept
  {
    --currentId_;
    while (currentId_ < dataSize_ && (*vector_)[currentId_].isEmpty())
    {
      --currentId_;
    }

    return *this;
  }

  template<class Key, class Value>
  HTIt<Key, Value> HTIt<Key, Value>::operator--(int) noexcept
  {
    HTIt<Key, Value> tmp = *this;

    this->operator--();
    return tmp;
  }

  template<class Key, class Value>
  std::pair<Key, Value>& HTIt<Key, Value>::operator*() const noexcept
  {
    return (*vector_)[currentId_].data_;
  }

  template<class Key, class Value>
  std::pair<Key, Value>* HTIt<Key, Value>::operator->() const noexcept
  {
    return &((*vector_)[currentId_].data_);
  }

  template<class Key, class Value>
  bool HTIt<Key, Value>::operator==(const HTIt<Key, Value>& otherIt) const noexcept
  {
    return currentId_ == otherIt.currentId_ && vector_ == otherIt.vector_;
  }

  template<class Key, class Value>
  bool HTIt<Key, Value>::operator!=(const HTIt<Key, Value>& otherIt) const noexcept
  {
    return !(*this == otherIt);
  }

  template<class Key, class Value>
  bool HTIt<Key, Value>::operator>(const HTIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ > otherIt.currentId_;
  }

  template<class Key, class Value>
  bool HTIt<Key, Value>::operator>=(const HTIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ >= otherIt.currentId_;
  }

  template<class Key, class Value>
  bool HTIt<Key, Value>::operator<(const HTIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ < otherIt.currentId_;
  }

  template<class Key, class Value>
  bool HTIt<Key, Value>::operator<=(const HTIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ <= otherIt.currentId_;
  }

  template<class Key, class Value>
  HTCIt<Key, Value>& HTCIt<Key, Value>::operator++() noexcept
  {
    ++currentId_;
    while (currentId_ < dataSize_ && (*vector_)[currentId_].isEmpty())
    {
      ++currentId_;
    }

    return *this;
  }

  template<class Key, class Value>
  HTCIt<Key, Value> HTCIt<Key, Value>::operator++(int) noexcept
  {
    HTCIt<Key, Value> tmp = *this;

    this->operator++();
    return tmp;
  }

  template<class Key, class Value>
  HTCIt<Key, Value>& HTCIt<Key, Value>::operator--() noexcept
  {
    --currentId_;
    while (currentId_ < dataSize_ && (*vector_)[currentId_].isEmpty())
    {
      --currentId_;
    }

    return *this;
  }

  template<class Key, class Value>
  HTCIt<Key, Value> HTCIt<Key, Value>::operator--(int) noexcept
  {
    HTCIt<Key, Value> tmp = *this;

    this->operator--();
    return tmp;
  }

  template<class Key, class Value>
  const std::pair<Key, Value>& HTCIt<Key, Value>::operator*() const noexcept
  {
    return (*vector_)[currentId_].data_;
  }

  template<class Key, class Value>
  const std::pair<Key, Value>* HTCIt<Key, Value>::operator->() const noexcept
  {
    return &((*vector_)[currentId_].data_);
  }

  template<class Key, class Value>
  bool HTCIt<Key, Value>::operator==(const HTCIt<Key, Value>& otherIt) const noexcept
  {
    return currentId_ == otherIt.currentId_ && vector_ == otherIt.vector_;
  }

  template<class Key, class Value>
  bool HTCIt<Key, Value>::operator!=(const HTCIt<Key, Value>& otherIt) const noexcept
  {
    return !(*this == otherIt);
  }

  template<class Key, class Value>
  bool HTCIt<Key, Value>::operator>(const HTCIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ > otherIt.currentId_;
  }

  template<class Key, class Value>
  bool HTCIt<Key, Value>::operator>=(const HTCIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ >= otherIt.currentId_;
  }

  template<class Key, class Value>
  bool HTCIt<Key, Value>::operator<(const HTCIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ < otherIt.currentId_;
  }

  template<class Key, class Value>
  bool HTCIt<Key, Value>::operator<=(const HTCIt<Key, Value>& otherIt) const noexcept
  {
    return vector_ == otherIt.vector_ && currentId_ <= otherIt.currentId_;
  }
}
#endif
