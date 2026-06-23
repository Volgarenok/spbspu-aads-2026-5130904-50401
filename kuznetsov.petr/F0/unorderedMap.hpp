#ifndef KUZNETSOV_UNORDERED_MAP_HPP
#define KUZNETSOV_UNORDERED_MAP_HPP
#include <cstddef>
#include <stdexcept>
#include <HashTable.hpp>

namespace kuznetsov {
  template< class Key, class Value, class Hash, class Equal >
  struct unorderedMap {
  public:
    using table_t = HashTable< Key, Value, Hash, Equal >;
    using iterator = typename table_t::iterator;
    using const_iterator = typename table_t::const_iterator;

    void insert(const Key& k, const Value& v);
    void insert(const Key& k, Value&& v);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    bool contains(const Key& k) const noexcept;
    void erase(const Key& k);
    size_t size() const noexcept;
    bool empty() const noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

  private:
    table_t table_;
  };
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::unorderedMap< Key, Value, Hash, Equal >::insert(const Key& k, const Value& v)
{
  if (table_.contains(k)) {
    table_.at(k) = v;
    return;
  }
  try {
    table_.add(k, v);
  } catch (const std::logic_error&) {
    table_.rehash();
    table_.add(k, v);
  }
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::unorderedMap< Key, Value, Hash, Equal >::insert(const Key& k, Value&& v)
{
  if (table_.contains(k)) {
    table_.at(k) = v;
    return;
  }
  table_.add(k, std::forward< Value >(v));
}

template< class Key, class Value, class Hash, class Equal >
Value& kuznetsov::unorderedMap< Key, Value, Hash, Equal >::at(const Key& k)
{
  return table_.at(k);
}

template< class Key, class Value, class Hash, class Equal >
const Value& kuznetsov::unorderedMap< Key, Value, Hash, Equal >::at(const Key& k) const
{
  return table_.at(k);
}

template< class Key, class Value, class Hash, class Equal >
bool kuznetsov::unorderedMap< Key, Value, Hash, Equal >::contains(const Key& k) const noexcept
{
  return table_.contains(k);
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::unorderedMap< Key, Value, Hash, Equal >::erase(const Key& k)
{
  table_.remove(k);
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::unorderedMap< Key, Value, Hash, Equal >::size() const noexcept
{
  return table_.size();
}

template< class Key, class Value, class Hash, class Equal >
bool kuznetsov::unorderedMap< Key, Value, Hash, Equal >::empty() const noexcept
{
  return table_.size() == 0;
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::unorderedMap< Key, Value, Hash, Equal >::iterator
kuznetsov::unorderedMap< Key, Value, Hash, Equal >::begin()
{
  return table_.begin();
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::unorderedMap< Key, Value, Hash, Equal >::iterator
kuznetsov::unorderedMap< Key, Value, Hash, Equal >::end()
{
  return table_.end();
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::unorderedMap< Key, Value, Hash, Equal >::const_iterator
kuznetsov::unorderedMap< Key, Value, Hash, Equal >::begin() const
{
  return table_.cbegin();
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::unorderedMap< Key, Value, Hash, Equal >::const_iterator
kuznetsov::unorderedMap< Key, Value, Hash, Equal >::end() const
{
  return table_.cend();
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::unorderedMap< Key, Value, Hash, Equal >::const_iterator
kuznetsov::unorderedMap< Key, Value, Hash, Equal >::cbegin() const
{
  return table_.cbegin();
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::unorderedMap< Key, Value, Hash, Equal >::const_iterator
kuznetsov::unorderedMap< Key, Value, Hash, Equal >::cend() const
{
  return table_.cend();
}

#endif
