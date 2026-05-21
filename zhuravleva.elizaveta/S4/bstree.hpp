#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <functional>
#include <cstddef>
#include <utility>
#include "treeNode.hpp"

namespace zhuravleva
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using valueType = std::pair< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using constIterator = BSTConstIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree();

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    constIterator cbegin() const noexcept;
    constIterator cend() const noexcept;

    iterator find(const Key& key) noexcept;
    constIterator find(const Key& key) const noexcept;

    bool contains(const Key& key) const noexcept;
    void push(const Key& key, const Value& value);

    Value& get(const Key& key);
    const Value& get(const Key& key) const;

    void drop(const Key& key);
    void clear() noexcept;
    size_t height() const noexcept;
    size_t height(constIterator it) const noexcept;

    constIterator rotateLeft(constIterator it) noexcept;
    constIterator rotateRight(constIterator it) noexcept;
    constIterator rotateLargeLeft(constIterator it) noexcept;
    constIterator rotateLargeRight(constIterator it) noexcept;

  private:
    TreeNode< Key, Value >* root_;
    size_t size_;
    Compare compare_;
  };
}
#endif
