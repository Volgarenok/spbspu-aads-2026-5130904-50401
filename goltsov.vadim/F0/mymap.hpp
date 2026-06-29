#ifndef MYMAP_HPP
#define MYMAP_HPP

#include "myrbt.hpp"
#include <utility>
#include <cstddef>

namespace goltsov
{
  template< class Key, class Value >
  using MapIterator = RBTIterator< Key, Value >;
  template< class Key, class Value >
  using MapConstIterator = RBTConstIterator< Key, Value >;

  template< class Key, class Value >
  class Map
  {
  public:
    Map();
    Map(const Map&) = default;
    Map(Map&&) noexcept = default;
    ~Map() = default;
    Map& operator=(const Map&) = default;
    Map& operator=(Map&&) noexcept = default;
    Value& operator[](const Key&);
    Value& at(const Key&);
    const Value& at(const Key&) const;
    MapIterator< Key, Value > getLast() noexcept;
    MapConstIterator< Key, Value > getLast() const noexcept;
    std::pair< MapIterator< Key, Value >, bool > insert(const std::pair< Key, Value >&);
    std::pair< MapIterator< Key, Value >, bool > insert(std::pair< Key, Value >&&);
    size_t erase(const Key&);
    void clear();
    MapIterator< Key, Value > find(const Key&);
    MapConstIterator< Key, Value > find(const Key&) const;
    bool contains(const Key&) const;
    size_t count(const Key&) const;
    bool empty() const;
    size_t size() const;
    MapIterator< Key, Value > begin();
    MapConstIterator< Key, Value > begin() const;
    MapConstIterator< Key, Value > cbegin() const;
    MapIterator< Key, Value > end();
    MapConstIterator< Key, Value > end() const;
    MapConstIterator< Key, Value > cend() const;
    void swap(Map< Key, Value >&) noexcept;
  private:
    goltsov::RBTree< Key, Value, std::less< Key > > data_;
  };
}

template< class Key, class Value >
goltsov::Map< Key, Value >::Map():
  data_(goltsov::RBTree< Key, Value, std::less< Key > >{})
{}
template< class Key, class Value >
Value& goltsov::Map< Key, Value>::operator[](const Key& k)
{
  return data_[k];
}
template< class Key, class Value >
Value& goltsov::Map< Key, Value >::at(const Key& k)
{
  return data_.at(k);
}
template< class Key, class Value >
const Value& goltsov::Map< Key, Value >::at(const Key& k) const
{
  return data_.at(k);
}
template< class Key, class Value >
goltsov::MapIterator< Key, Value > goltsov::Map< Key, Value >::getLast() noexcept
{
  return data_.getLast();
}
template< class Key, class Value >
goltsov::MapConstIterator< Key, Value > goltsov::Map< Key, Value >::getLast() const noexcept
{
  return data_.getLast();
}
template< class Key, class Value >
std::pair< goltsov::MapIterator< Key, Value >, bool > goltsov::Map< Key, Value >::insert(const std::pair< Key, Value >& k_v)
{
  return data_.insert(k_v);
}
template< class Key, class Value >
std::pair< goltsov::MapIterator< Key, Value >, bool > goltsov::Map< Key, Value >::insert(std::pair< Key, Value >&& k_v)
{
  return data_.insert(std::move(k_v));
}
template< class Key, class Value >
size_t goltsov::Map< Key, Value >::erase(const Key& k)
{
  return data_.erase(k);
}
template< class Key, class Value >
void goltsov::Map< Key, Value >::clear()
{
  data_.clear();
}
template< class Key, class Value >
goltsov::MapIterator< Key, Value > goltsov::Map< Key, Value >::find(const Key& k)
{
  return data_.find(k);
}
template< class Key, class Value >
goltsov::MapConstIterator< Key, Value > goltsov::Map< Key, Value >::find(const Key& k) const
{
  return data_.find(k);
}
template< class Key, class Value >
bool goltsov::Map< Key, Value >::contains(const Key& k) const
{
  return data_.find(k) != data_.end();
}
template< class Key, class Value >
size_t goltsov::Map< Key, Value >::count(const Key& k) const
{
  return (contains(k) ? 1 : 0);
}
template< class Key, class Value >
bool goltsov::Map< Key, Value >::empty() const
{
  return data_.empty();
}
template< class Key, class Value >
size_t goltsov::Map< Key, Value >::size() const
{
  return data_.size();
}
template< class Key, class Value >
goltsov::MapIterator< Key, Value > goltsov::Map< Key, Value >::begin()
{
  return data_.begin();
}
template< class Key, class Value >
goltsov::MapConstIterator< Key, Value > goltsov::Map< Key, Value >::begin() const
{
  return data_.begin();
}
template< class Key, class Value >
goltsov::MapConstIterator< Key, Value > goltsov::Map< Key, Value >::cbegin() const
{
  return data_.begin();
}
template< class Key, class Value >
goltsov::MapIterator< Key, Value > goltsov::Map< Key, Value >::end()
{
  return data_.end();
}
template< class Key, class Value >
goltsov::MapConstIterator< Key, Value > goltsov::Map< Key, Value >::end() const
{
  return data_.end();
}
template< class Key, class Value >
goltsov::MapConstIterator< Key, Value > goltsov::Map< Key, Value >::cend() const
{
  return data_.end();
}
template< class Key, class Value >
void goltsov::Map< Key, Value >::swap(Map< Key, Value >& other) noexcept
{
  data_.swap(other.data_);
}

#endif
