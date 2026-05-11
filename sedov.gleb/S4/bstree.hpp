#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include "bstiterators.hpp"
#include "treenode.hpp"

namespace sedov
{
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree & other);
    BSTree(BSTree && other) noexcept;
    ~BSTree();

    BSTree & operator=(const BSTree & other);
    BSTree & operator=(BSTree && other) noexcept;

    bool empty() const;
    size_t size() const;
    void swap(BSTree & other) noexcept;

    void clear() noexcept;

    friend class BSTConstIterator< Key, Value >;
    friend class BSTIterator< Key, Value >;

  private:
    TreeNode< Key, Value > * root_;
    size_t size_;
    Compare comp_;

    static void initFakeLeaf();
    TreeNode< Key, Value > * clone(TreeNode< Key, Value > * src, TreeNode< Key, Value > * parent);
    void clearImpl(TreeNode< Key, Value > * node) noexcept;
  };
}

template < class Key, class Value, class Compare >
void sedov::BSTree< Key, Value, Compare >::initFakeLeaf()
{
  if (TreeNode< Key, Value >::fakeLeaf == nullptr)
  {
    TreeNode< Key, Value >::fakeLeaf = new TreeNode< Key, Value >(Key(), Value(), nullptr);
    TreeNode< Key, Value >::fakeLeaf->left_ = TreeNode< Key, Value >::fakeLeaf;
    TreeNode< Key, Value >::fakeLeaf->right_ = TreeNode< Key, Value >::fakeLeaf;
    TreeNode< Key, Value >::fakeLeaf->parent_ = TreeNode< Key, Value >::fakeLeaf;
  }
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::BSTree():
  root_(nullptr),
  size_(0),
  comp_()
{
  initFakeLeaf();
  root_ = TreeNode< Key, Value >::fakeLeaf;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::BSTree(const BSTree & other):
  root_(nullptr),
  size_(0),
  comp_(other.comp_)
{
  initFakeLeaf();
  root_ = TreeNode< Key, Value >::fakeLeaf;
  root_ = clone(other.root_, TreeNode< Key, Value >::fakeLeaf);
  size_ = other.size_;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::BSTree(BSTree && other) noexcept:
  root_(other.root_),
  size_(other.size_),
  comp_(std::move(other.comp_))
{
  initFakeLeaf();
  other.root_ = TreeNode< Key, Value >::fakeLeaf;
  other.size_ = 0;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare > & sedov::BSTree< Key, Value, Compare >::operator=(const BSTree & other)
{
  if (this != std::addressof(other))
  {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare > & sedov::BSTree< Key, Value, Compare >::operator=(BSTree && other) noexcept
{
  if (this != std::addressof(other))
  {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    comp_ = std::move(other.comp_);
    other.root_ = TreeNode< Key, Value >::fakeLeaf;
    other.size_ = 0;
  }
  return *this;
}

template < class Key, class Value, class Compare >
bool sedov::BSTree< Key, Value, Compare >::empty() const
{
  return size_ == 0;
}

template < class Key, class Value, class Compare >
size_t sedov::BSTree< Key, Value, Compare >::size() const
{
  return size_;
}

template < class Key, class Value, class Compare >
void sedov::BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template < class Key, class Value, class Compare >
void sedov::BSTree< Key, Value, Compare >::clearImpl(TreeNode< Key, Value > * node) noexcept
{
  if (node->isFake())
  {
    return;
  }
  clearImpl(node->left_);
  clearImpl(node->right_);
  delete node;
}

template < class Key, class Value, class Compare >
void sedov::BSTree< Key, Value, Compare >::clear() noexcept
{
  clearImpl(root_);
  root_ = TreeNode< Key, Value >::fakeLeaf;
  size_ = 0;
}

template < class Key, class Value, class Compare >
sedov::TreeNode< Key, Value > *
sedov::BSTree< Key, Value, Compare >::clone(TreeNode< Key, Value > * src,
                                              TreeNode< Key, Value > * parent)
{
  if (src->isFake())
  {
    return TreeNode< Key, Value >::fakeLeaf;
  }
  TreeNode< Key, Value > * n = new TreeNode< Key, Value >(src->key_, src->value_, parent);
  n->left_ = clone(src->left_, n);
  n->right_ = clone(src->right_, n);
  return n;
}

#endif
