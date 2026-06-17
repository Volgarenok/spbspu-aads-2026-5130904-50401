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
