#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace sedov
{
  namespace detail
  {
    template < class Key, class Value >
    struct TreeNode
    {
      std::pair< Key, Value > data;
      TreeNode * left;
      TreeNode * right;
      TreeNode * parent;

      static TreeNode * fakeLeaf;

      TreeNode(const Key & key, const Value & value, TreeNode * parent);
      TreeNode(Key && key, Value && value, TreeNode * parent);

      bool isFake() const noexcept;
      static void initFakeLeaf();
    };

    template < class Key, class Value >
    TreeNode< Key, Value > * TreeNode< Key, Value >::fakeLeaf = nullptr;
  }
}

template < class Key, class Value >
sedov::detail::TreeNode< Key, Value >::TreeNode(const Key & key, const Value & value,
    sedov::detail::TreeNode< Key, Value > * parent):
  data(key, value),
  left(fakeLeaf),
  right(fakeLeaf),
  parent(parent)
{}

template < class Key, class Value >
sedov::detail::TreeNode< Key, Value >::TreeNode(Key && key, Value && value,
    sedov::detail::TreeNode< Key, Value > * parent):
  data(std::forward< Key >(key), std::forward< Value >(value)),
  left(fakeLeaf),
  right(fakeLeaf),
  parent(parent)
{}

template < class Key, class Value >
bool sedov::detail::TreeNode< Key, Value >::isFake() const noexcept
{
  return this == fakeLeaf;
}

template < class Key, class Value >
void sedov::detail::TreeNode< Key, Value >::initFakeLeaf()
{
  if (fakeLeaf == nullptr)
  {
    fakeLeaf = new TreeNode(Key(), Value(), nullptr);
    fakeLeaf->left = fakeLeaf;
    fakeLeaf->right = fakeLeaf;
    fakeLeaf->parent = fakeLeaf;
  }
}

#endif
