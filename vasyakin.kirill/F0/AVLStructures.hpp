#ifndef AVL_STRUCTURES_HPP
#define AVL_STRUCTURES_HPP
#include "AVLTree.hpp"
#include "vector.hpp"

namespace vasyakin
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLMap
  {
  public:
    using iterator = typename AVLTree< Key, Value, Compare >::iterator;
    using const_iterator = typename AVLTree< Key, Value, Compare >::const_iterator;
    using value_type = std::pair< const Key, Value >;

    AVLMap(const AVLMap&) = default;
    AVLMap(AVLMap&&) noexcept = default;

    explicit AVLMap(const Compare& cmp = Compare());
    AVLMap(std::initializer_list< value_type > init, const Compare& cmp = Compare());

    ~AVLMap() noexcept = default;
    AVLMap& operator=(const AVLMap&) = default;
    AVLMap& operator=(AVLMap&&) noexcept = default;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    Value& operator[](const Key& key);

    iterator find(const Key& key) noexcept;
    const_iterator find(const Key& key) const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    size_t count(const Key& key) const noexcept;
    bool has(const Key& key) const noexcept;

    std::pair< iterator, bool > insert(const value_type& value);

    void clear() noexcept;
    size_t erase(const Key& key);

    iterator lower_bound(const Key& key) noexcept;
    const_iterator lower_bound(const Key& key) const noexcept;

    iterator upper_bound(const Key& key) noexcept;
    const_iterator upper_bound(const Key& key) const noexcept;

    std::pair< iterator, iterator > equal_range(const Key& key) noexcept;
    std::pair< const_iterator, const_iterator > equal_range(const Key& key) const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
  private:
    AVLTree< Key, Value, Compare > tree_;
  };

  template< class Key, class Value, class Compare >
  AVLMap< Key, Value, Compare >::AVLMap(const Compare& cmp):
    tree_(cmp)
  {}

  template< class Key, class Value, class Compare >
  AVLMap< Key, Value, Compare >::AVLMap(
    std::initializer_list< value_type > init, const Compare& cmp):
    tree_(cmp)
  {
    for (auto it = init.begin(); it != init.end(); ++it)
    {
      const auto& pair = *it;
      tree_.insert(pair.first, pair.second);
    }
  }

  template< class Key, class Value, class Compare >
  Value& AVLMap< Key, Value, Compare >::at(const Key& key)
  {
    return tree_.at(key);
  }

  template< class Key, class Value, class Compare >
  const Value& AVLMap< Key, Value, Compare >::at(const Key& key) const
  {
    return tree_.at(key);
  }

  template< class Key, class Value, class Compare >
  Value& AVLMap< Key, Value, Compare >::operator[](const Key& key)
  {
    if (!tree_.has(key))
    {
      tree_.insert(key, Value{});
    }

    return tree_.at(key);
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::iterator
  AVLMap< Key, Value, Compare >::find(const Key& key) noexcept
  {
    return tree_.find(key);
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::const_iterator
  AVLMap< Key, Value, Compare >::find(const Key& key) const noexcept
  {
    return tree_.find(key);
  }

  template< class Key, class Value, class Compare >
  bool AVLMap< Key, Value, Compare >::empty() const noexcept
  {
    return tree_.empty();
  }

  template< class Key, class Value, class Compare >
  size_t AVLMap< Key, Value, Compare >::size() const noexcept
  {
    return tree_.size();
  }

  template< class Key, class Value, class Compare >
  size_t AVLMap< Key, Value, Compare >::count(const Key& key) const noexcept
  {
    return tree_.has(key) ? 1 : 0;
  }

  template< class Key, class Value, class Compare >
  bool AVLMap< Key, Value, Compare >::has(const Key& key) const noexcept
  {
    return tree_.has(key);
  }

  template< class Key, class Value, class Compare >
  std::pair< typename AVLMap< Key, Value, Compare >::iterator, bool >
    AVLMap< Key, Value, Compare >::insert(const value_type& value)
  {
    auto it = tree_.find(value.first);
    if (it != tree_.end())
    {
      return {it, false};
    }

    tree_.insert(value.first, value.second);
    return {tree_.find(value.first), true};
  }

  template< class Key, class Value, class Compare >
  void AVLMap< Key, Value, Compare >::clear() noexcept
  {
    tree_.clear();
  }

  template< class Key, class Value, class Compare >
  size_t AVLMap< Key, Value, Compare >::erase(const Key& key)
  {
    return tree_.remove(key) ? 1 : 0;
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::iterator
  AVLMap< Key, Value, Compare >::lower_bound(const Key& key) noexcept
  {
    return tree_.lower_bound(key);
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::const_iterator
  AVLMap< Key, Value, Compare >::lower_bound(const Key& key) const noexcept
  {
    return tree_.lower_bound(key);
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::iterator
  AVLMap< Key, Value, Compare >::upper_bound(const Key& key) noexcept
  {
    return tree_.upper_bound(key);
  }
  
  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::const_iterator
  AVLMap< Key, Value, Compare >::upper_bound(const Key& key) const noexcept
  {
    return tree_.upper_bound(key);
  }

  template< class Key, class Value, class Compare >
  std::pair< typename AVLMap< Key, Value, Compare >::iterator,
    typename AVLMap< Key, Value, Compare >::iterator >
    AVLMap< Key, Value, Compare >::equal_range(const Key& key) noexcept
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Compare >
  std::pair< typename AVLMap< Key, Value, Compare >::const_iterator,
    typename AVLMap< Key, Value, Compare >::const_iterator >
    AVLMap< Key, Value, Compare >::equal_range(const Key& key) const noexcept
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::iterator
  AVLMap< Key, Value, Compare >::begin() noexcept
  {
    return tree_.begin();
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::iterator
  AVLMap< Key, Value, Compare >::end() noexcept
  {
    return tree_.end();
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::const_iterator
  AVLMap< Key, Value, Compare >::begin() const noexcept
  {
    return tree_.cbegin();
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::const_iterator
  AVLMap< Key, Value, Compare >::end() const noexcept
  {
    return tree_.cend();
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::const_iterator
  AVLMap< Key, Value, Compare >::cbegin() const noexcept
  {
    return tree_.cbegin();
  }

  template< class Key, class Value, class Compare >
  typename AVLMap< Key, Value, Compare >::const_iterator
  AVLMap< Key, Value, Compare >::cend() const noexcept
  {
    return tree_.cend();
  }

  template< class Key, class Compare = std::less< Key > >
  class AVLSet
  {
  public:
    using iterator = typename AVLTree< Key, Key, Compare >::const_iterator;
    using const_iterator = iterator;
    using value_type = Key;

    AVLSet(const AVLSet&) = default;
    AVLSet(AVLSet&&) noexcept = default;

    explicit AVLSet(const Compare& cmp = Compare());
    AVLSet(std::initializer_list< value_type > init, const Compare& cmp = Compare());

    ~AVLSet() noexcept = default;
    AVLSet& operator=(const AVLSet&) = default;
    AVLSet& operator=(AVLSet&&) noexcept = default;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    size_t erase(const Key& key);

    std::pair< iterator, bool > insert(const value_type& value);

    size_t count(const Key& key) const noexcept;
    bool has(const Key& key) const noexcept;
    iterator find(const Key& key) const noexcept;

    iterator lower_bound(const Key& key) const noexcept;
    iterator upper_bound(const Key& key) const noexcept;

    std::pair< iterator, iterator > equal_range(const Key& key) const noexcept;

    iterator begin() const noexcept;
    iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
  private:
    AVLTree< Key, Key, Compare > tree_;
  };

  template< class Key, class Compare >
  AVLSet< Key, Compare >::AVLSet(const Compare& cmp):
    tree_(cmp)
  {}

  template< class Key, class Compare >
  AVLSet< Key, Compare >::AVLSet(
    std::initializer_list< value_type > init, const Compare& cmp):
    tree_(cmp)
  {
    for (auto it = init.begin(); it != init.end(); ++it)
    {
      const auto& val = *it;
      tree_.insert(val, val);
    }
  }

  template< class Key, class Compare >
  bool AVLSet< Key, Compare >::empty() const noexcept
  {
    return tree_.empty();
  }

  template< class Key, class Compare >
  size_t AVLSet< Key, Compare >::size() const noexcept
  {
    return tree_.size();
  }

  template< class Key, class Compare >
  void AVLSet< Key, Compare >::clear() noexcept
  {
    tree_.clear();
  }

  template< class Key, class Compare >
  size_t AVLSet< Key, Compare >::erase(const Key& key)
  {
    return tree_.remove(key) ? 1 : 0;
  }

  template< class Key, class Compare >
  std::pair< typename AVLSet< Key, Compare >::iterator, bool >
    AVLSet< Key, Compare >::insert(const value_type& value)
  {
    auto it = tree_.find(value);
    if (it != tree_.end())
    {
      return {it, false};
    }

    tree_.insert(value, value);
    return {tree_.find(value), true};
  }

  template< class Key, class Compare >
  size_t AVLSet< Key, Compare >::count(const Key& key) const noexcept
  {
    return tree_.has(key) ? 1 : 0;
  }

  template< class Key, class Compare >
  bool AVLSet< Key, Compare >::has(const Key& key) const noexcept
  {
    return tree_.has(key);
  }

  template< class Key, class Compare >
  typename AVLSet< Key, Compare >::iterator
  AVLSet< Key, Compare >::find(const Key& key) const noexcept
  {
    return tree_.find(key);
  }

  template< class Key, class Compare >
  typename AVLSet< Key, Compare >::iterator
  AVLSet< Key, Compare >::lower_bound(const Key& key) const noexcept
  {
    return tree_.lower_bound(key);
  }

  template< class Key, class Compare >
  typename AVLSet< Key, Compare >::iterator
  AVLSet< Key, Compare >::upper_bound(const Key& key) const noexcept
  {
    return tree_.upper_bound(key);
  }

  template< class Key, class Compare >
  std::pair< typename AVLSet< Key, Compare >::iterator,
    typename AVLSet< Key, Compare >::iterator >
    AVLSet< Key, Compare >::equal_range(const Key& key) const noexcept
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Compare >
  typename AVLSet< Key, Compare >::iterator
  AVLSet< Key, Compare >::begin() const noexcept
  {
    return tree_.cbegin();
  }

  template< class Key, class Compare >
  typename AVLSet< Key, Compare >::iterator
  AVLSet< Key, Compare >::end() const noexcept
  {
    return tree_.cend();
  }

  template< class Key, class Compare >
  typename AVLSet< Key, Compare >::const_iterator
  AVLSet< Key, Compare >::cbegin() const noexcept
  {
    return tree_.cbegin();
  }

  template< class Key, class Compare >
  typename AVLSet< Key, Compare >::const_iterator
  AVLSet< Key, Compare >::cend() const noexcept
  {
    return tree_.cend();
  }

  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLMultiMap
  {
  public:
    using iterator = typename AVLTree< Key, topit::Vector< Value >, Compare >::iterator;
    using const_iterator = typename AVLTree< Key, topit::Vector< Value >, Compare >::const_iterator;
    using value_type = std::pair< const Key, Value >;

    AVLMultiMap(const AVLMultiMap&) = default;
    AVLMultiMap(AVLMultiMap&&) noexcept = default;

    explicit AVLMultiMap(const Compare& cmp = Compare());
    AVLMultiMap(std::initializer_list< value_type > init, const Compare& cmp = Compare());

    ~AVLMultiMap() noexcept = default;
    AVLMultiMap& operator=(const AVLMultiMap&) = default;
    AVLMultiMap& operator=(AVLMultiMap&&) noexcept = default;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    size_t erase(const Key& key);

    iterator insert(const value_type& value);

    size_t count(const Key& key) const noexcept;
    bool has(const Key& key) const noexcept;

    iterator find(const Key& key) noexcept;
    const_iterator find(const Key& key) const noexcept;

    iterator lower_bound(const Key& key) noexcept;
    const_iterator lower_bound(const Key& key) const noexcept;

    iterator upper_bound(const Key& key) noexcept;
    const_iterator upper_bound(const Key& key) const noexcept;

    std::pair< iterator, iterator > equal_range(const Key& key) noexcept;
    std::pair< const_iterator, const_iterator > equal_range(const Key& key) const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
  private:
    AVLTree< Key, topit::Vector< Value >, Compare > tree_;
  };

  template< class Key, class Compare = std::less< Key > >
  class AVLMultiSet
  {
  public:
    using iterator = typename AVLTree< Key, topit::Vector< Key >, Compare >::const_iterator;
    using const_iterator = iterator;
    using value_type = Key;

    AVLMultiSet(const AVLMultiSet&) = default;
    AVLMultiSet(AVLMultiSet&&) noexcept = default;

    explicit AVLMultiSet(const Compare& cmp = Compare());
    AVLMultiSet(std::initializer_list< value_type > init, const Compare& cmp = Compare());

    ~AVLMultiSet() noexcept = default;
    AVLMultiSet& operator=(const AVLMultiSet&) = default;
    AVLMultiSet& operator=(AVLMultiSet&&) noexcept = default;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    size_t erase(const Key& key);

    iterator insert(const value_type& value);

    size_t count(const Key& key) const noexcept;
    bool has(const Key& key) const noexcept;
    iterator find(const Key& key) const noexcept;

    iterator lower_bound(const Key& key) const noexcept;
    iterator upper_bound(const Key& key) const noexcept;

    std::pair< iterator, iterator > equal_range(const Key& key) const noexcept;

    iterator begin() const noexcept;
    iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
  private:
    AVLTree< Key, topit::Vector< Key >, Compare > tree_;
  };
}

#endif
