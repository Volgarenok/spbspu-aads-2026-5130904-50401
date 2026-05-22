#ifndef TREEITERATORS_HPP
#define TREEITERATORS_HPP
#include <stdexcept>
#include <utility>

#include "treeNode.hpp"

namespace zhuravleva
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  class BSTIterator
  {
    friend class BSTConstIterator< Key, Value >;
  public:
    BSTIterator() noexcept;
    explicit BSTIterator(TreeNode< Key, Value >* node) noexcept;
    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;
    bool operator==(const BSTIterator& other) const noexcept;
    bool operator!=(const BSTIterator& other) const noexcept;
    BSTIterator& operator++();
    BSTIterator operator++(int);

  private:
    TreeNode< Key, Value >* current_;
  };
}

#endif
