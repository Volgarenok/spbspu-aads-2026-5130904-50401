#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <utility>

namespace zhuravleva
{
  namespace detail
  {
    template< class Key, class Value >
    struct TreeNode
    {
      std::pair< Key, Value > data;
      TreeNode< Key, Value >* parent;
      TreeNode< Key, Value >* left;
      TreeNode< Key, Value >* right;

      TreeNode(const Key& key, const Value& value,
        TreeNode< Key, Value >* parentNode = nullptr):
        data(key, value),
        parent(parentNode),
        left(nullptr),
        right(nullptr)
      {}
    };
  }
}

#endif
