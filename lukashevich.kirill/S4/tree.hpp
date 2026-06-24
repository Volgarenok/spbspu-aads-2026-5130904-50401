#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "citer_tree.hpp"
#include "iter_tree.hpp"
#include "node_tree.hpp"

namespace lukashevich
{
  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree & other);
    BSTree(BSTree && other);
    ~BSTree();

    BSTree & operator=(const BSTree & other);
    BSTree & operator=(BSTree && other);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    bool empty() const;
    iterator find(const Key & key);
    const_iterator find(const Key & key) const;
    bool contains(const Key & key) const;

    void push(const Key & key, const Value & value);
    Value & at(const Key & key);
    const Value & at(const Key & key) const;
    Value get(const Key & key) const;
    Value drop(const Key & key);
    void clear();

    const_iterator rotateLeft(const_iterator position);
    const_iterator rotateRight(const_iterator position);
    const_iterator rotateLargeLeft(const_iterator position);
    const_iterator rotateLargeRight(const_iterator position);

    std::size_t height() const;
    std::size_t height(const_iterator position) const;

  private:
    using Node = TreeNode< Key, Value >;

    TreeNodeBase fakeRoot_;
    Compare compare_;

    TreeNodeBase * root();
    const TreeNodeBase * root() const;
    void resetFakeRoot();
    void setRoot(TreeNodeBase * node);
    void swapTree(BSTree & other);

    static Node * asNode(TreeNodeBase * node);
    static const Node * asNode(const TreeNodeBase * node);
    static TreeNodeBase * minimum(TreeNodeBase * node);
    static const TreeNodeBase * minimum(const TreeNodeBase * node);

    TreeNodeBase * findNode(const Key & key);
    const TreeNodeBase * findNode(const Key & key) const;

    void replaceSubtree(TreeNodeBase * oldRoot, TreeNodeBase * newRoot);
    void eraseNode(TreeNodeBase * node);

    static void destroySubtree(TreeNodeBase * node);
    static TreeNodeBase * cloneSubtree(const TreeNodeBase * node, TreeNodeBase * parent);
    static std::size_t subtreeHeight(const TreeNodeBase * node);

    void validatePosition(const_iterator position) const;
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    fakeRoot_(),
    compare_()
  {
    resetFakeRoot();
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree & other):
    fakeRoot_(),
    compare_(other.compare_)
  {
    resetFakeRoot();
    setRoot(cloneSubtree(other.root(), &fakeRoot_));
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree && other):
    fakeRoot_(),
    compare_(std::move(other.compare_))
  {
    resetFakeRoot();
    setRoot(other.root());
    other.resetFakeRoot();
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > &
      BSTree< Key, Value, Compare >::operator=(const BSTree & other)
  {
    if (this != &other)
    {
      BSTree copy(other);
      swapTree(copy);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > &
      BSTree< Key, Value, Compare >::operator=(BSTree && other)
  {
    if (this != &other)
    {
      BSTree moved(std::move(other));
      swapTree(moved);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator
      BSTree< Key, Value, Compare >::begin()
  {
    TreeNodeBase * node = minimum(root());
    return iterator(node == nullptr ? &fakeRoot_ : node, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::begin() const
  {
    const TreeNodeBase * node = minimum(root());
    return const_iterator(node == nullptr ? &fakeRoot_ : node, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator
      BSTree< Key, Value, Compare >::end()
  {
    return iterator(&fakeRoot_, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::end() const
  {
    return const_iterator(&fakeRoot_, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const
  {
    return root() == nullptr;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator
      BSTree< Key, Value, Compare >::find(const Key & key)
  {
    TreeNodeBase * node = findNode(key);
    return node == nullptr ? end() : iterator(node, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::find(const Key & key) const
  {
    const TreeNodeBase * node = findNode(key);
    return node == nullptr ? end() : const_iterator(node, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::contains(const Key & key) const
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(
      const Key & key, const Value & value)
  {
    TreeNodeBase * parent = &fakeRoot_;
    TreeNodeBase * current = root();
    bool insertLeft = false;

    while (current != nullptr)
    {
      parent = current;
      const Key & currentKey = asNode(current)->value_.first;
      if (compare_(key, currentKey))
      {
        insertLeft = true;
        current = current->left_;
      }
      else if (compare_(currentKey, key))
      {
        insertLeft = false;
        current = current->right_;
      }
      else
      {
        asNode(current)->value_.second = value;
        return;
      }
    }

    Node * node = new Node(key, value);
    node->parent_ = parent;
    if (parent == &fakeRoot_)
    {
      fakeRoot_.parent_ = node;
    }
    else if (insertLeft)
    {
      parent->left_ = node;
    }
    else
    {
      parent->right_ = node;
    }
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::at(const Key & key)
  {
    TreeNodeBase * node = findNode(key);
    if (node == nullptr)
    {
      throw std::out_of_range("BSTree key not found");
    }
    return asNode(node)->value_.second;
  }

  template< class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::at(const Key & key) const
  {
    const TreeNodeBase * node = findNode(key);
    if (node == nullptr)
    {
      throw std::out_of_range("BSTree key not found");
    }
    return asNode(node)->value_.second;
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::get(const Key & key) const
  {
    return at(key);
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(const Key & key)
  {
    TreeNodeBase * node = findNode(key);
    if (node == nullptr)
    {
      throw std::out_of_range("BSTree key not found");
    }
    Value value(asNode(node)->value_.second);
    eraseNode(node);
    return value;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
  {
    destroySubtree(root());
    resetFakeRoot();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::rotateLeft(const_iterator position)
  {
    validatePosition(position);
    TreeNodeBase * lifted = const_cast< TreeNodeBase * >(position.node_);
    TreeNodeBase * lowered = lifted->parent_;
    if (lowered == &fakeRoot_ || lowered->right_ != lifted)
    {
      throw std::invalid_argument("Invalid left rotation");
    }

    TreeNodeBase * grandParent = lowered->parent_;
    TreeNodeBase * middle = lifted->left_;
    if (grandParent == &fakeRoot_)
    {
      fakeRoot_.parent_ = lifted;
    }
    else if (grandParent->left_ == lowered)
    {
      grandParent->left_ = lifted;
    }
    else
    {
      grandParent->right_ = lifted;
    }

    lifted->parent_ = grandParent;
    lifted->left_ = lowered;
    lowered->parent_ = lifted;
    lowered->right_ = middle;
    if (middle != nullptr)
    {
      middle->parent_ = lowered;
    }
    return const_iterator(lifted, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::rotateRight(const_iterator position)
  {
    validatePosition(position);
    TreeNodeBase * lifted = const_cast< TreeNodeBase * >(position.node_);
    TreeNodeBase * lowered = lifted->parent_;
    if (lowered == &fakeRoot_ || lowered->left_ != lifted)
    {
      throw std::invalid_argument("Invalid right rotation");
    }

    TreeNodeBase * grandParent = lowered->parent_;
    TreeNodeBase * middle = lifted->right_;
    if (grandParent == &fakeRoot_)
    {
      fakeRoot_.parent_ = lifted;
    }
    else if (grandParent->left_ == lowered)
    {
      grandParent->left_ = lifted;
    }
    else
    {
      grandParent->right_ = lifted;
    }

    lifted->parent_ = grandParent;
    lifted->right_ = lowered;
    lowered->parent_ = lifted;
    lowered->left_ = middle;
    if (middle != nullptr)
    {
      middle->parent_ = lowered;
    }
    return const_iterator(lifted, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::rotateLargeLeft(
          const_iterator position)
  {
    validatePosition(position);
    TreeNodeBase * lifted = const_cast< TreeNodeBase * >(position.node_);
    TreeNodeBase * parent = lifted->parent_;
    if (parent == &fakeRoot_)
    {
      throw std::invalid_argument("Invalid large left rotation");
    }
    TreeNodeBase * grandParent = parent->parent_;
    if (grandParent == &fakeRoot_
        || grandParent->right_ != parent || parent->left_ != lifted)
    {
      throw std::invalid_argument("Invalid large left rotation");
    }

    TreeNodeBase * upperParent = grandParent->parent_;
    TreeNodeBase * leftMiddle = lifted->left_;
    TreeNodeBase * rightMiddle = lifted->right_;
    if (upperParent == &fakeRoot_)
    {
      fakeRoot_.parent_ = lifted;
    }
    else if (upperParent->left_ == grandParent)
    {
      upperParent->left_ = lifted;
    }
    else
    {
      upperParent->right_ = lifted;
    }

    lifted->parent_ = upperParent;
    lifted->left_ = grandParent;
    lifted->right_ = parent;
    grandParent->parent_ = lifted;
    parent->parent_ = lifted;
    grandParent->right_ = leftMiddle;
    parent->left_ = rightMiddle;
    if (leftMiddle != nullptr)
    {
      leftMiddle->parent_ = grandParent;
    }
    if (rightMiddle != nullptr)
    {
      rightMiddle->parent_ = parent;
    }
    return const_iterator(lifted, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::rotateLargeRight(
          const_iterator position)
  {
    validatePosition(position);
    TreeNodeBase * lifted = const_cast< TreeNodeBase * >(position.node_);
    TreeNodeBase * parent = lifted->parent_;
    if (parent == &fakeRoot_)
    {
      throw std::invalid_argument("Invalid large right rotation");
    }
    TreeNodeBase * grandParent = parent->parent_;
    if (grandParent == &fakeRoot_
        || grandParent->left_ != parent || parent->right_ != lifted)
    {
      throw std::invalid_argument("Invalid large right rotation");
    }

    TreeNodeBase * upperParent = grandParent->parent_;
    TreeNodeBase * leftMiddle = lifted->left_;
    TreeNodeBase * rightMiddle = lifted->right_;
    if (upperParent == &fakeRoot_)
    {
      fakeRoot_.parent_ = lifted;
    }
    else if (upperParent->left_ == grandParent)
    {
      upperParent->left_ = lifted;
    }
    else
    {
      upperParent->right_ = lifted;
    }

    lifted->parent_ = upperParent;
    lifted->left_ = parent;
    lifted->right_ = grandParent;
    parent->parent_ = lifted;
    grandParent->parent_ = lifted;
    parent->right_ = leftMiddle;
    grandParent->left_ = rightMiddle;
    if (leftMiddle != nullptr)
    {
      leftMiddle->parent_ = parent;
    }
    if (rightMiddle != nullptr)
    {
      rightMiddle->parent_ = grandParent;
    }
    return const_iterator(lifted, &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  std::size_t BSTree< Key, Value, Compare >::height() const
  {
    return subtreeHeight(root());
  }

  template< class Key, class Value, class Compare >
  std::size_t BSTree< Key, Value, Compare >::height(
      const_iterator position) const
  {
    validatePosition(position);
    if (position.node_ == &fakeRoot_)
    {
      return 0;
    }
    return subtreeHeight(position.node_);
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase * BSTree< Key, Value, Compare >::root()
  {
    return fakeRoot_.parent_ == &fakeRoot_ ? nullptr : fakeRoot_.parent_;
  }

  template< class Key, class Value, class Compare >
  const TreeNodeBase * BSTree< Key, Value, Compare >::root() const
  {
    return fakeRoot_.parent_ == &fakeRoot_ ? nullptr : fakeRoot_.parent_;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::resetFakeRoot()
  {
    fakeRoot_.parent_ = &fakeRoot_;
    fakeRoot_.left_ = nullptr;
    fakeRoot_.right_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::setRoot(TreeNodeBase * node)
  {
    fakeRoot_.parent_ = node == nullptr ? &fakeRoot_ : node;
    if (node != nullptr)
    {
      node->parent_ = &fakeRoot_;
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swapTree(BSTree & other)
  {
    using std::swap;
    swap(compare_, other.compare_);
    TreeNodeBase * thisRoot = root();
    TreeNodeBase * otherRoot = other.root();
    setRoot(otherRoot);
    other.setRoot(thisRoot);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node *
      BSTree< Key, Value, Compare >::asNode(TreeNodeBase * node)
  {
    return static_cast< Node * >(node);
  }

  template< class Key, class Value, class Compare >
  const typename BSTree< Key, Value, Compare >::Node *
      BSTree< Key, Value, Compare >::asNode(const TreeNodeBase * node)
  {
    return static_cast< const Node * >(node);
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase * BSTree< Key, Value, Compare >::minimum(TreeNodeBase * node)
  {
    while (node != nullptr && node->left_ != nullptr)
    {
      node = node->left_;
    }
    return node;
  }

  template< class Key, class Value, class Compare >
  const TreeNodeBase * BSTree< Key, Value, Compare >::minimum(
      const TreeNodeBase * node)
  {
    while (node != nullptr && node->left_ != nullptr)
    {
      node = node->left_;
    }
    return node;
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase * BSTree< Key, Value, Compare >::findNode(const Key & key)
  {
    TreeNodeBase * current = root();
    while (current != nullptr)
    {
      const Key & currentKey = asNode(current)->value_.first;
      if (compare_(key, currentKey))
      {
        current = current->left_;
      }
      else if (compare_(currentKey, key))
      {
        current = current->right_;
      }
      else
      {
        return current;
      }
    }
    return nullptr;
  }

  template< class Key, class Value, class Compare >
  const TreeNodeBase * BSTree< Key, Value, Compare >::findNode(
      const Key & key) const
  {
    const TreeNodeBase * current = root();
    while (current != nullptr)
    {
      const Key & currentKey = asNode(current)->value_.first;
      if (compare_(key, currentKey))
      {
        current = current->left_;
      }
      else if (compare_(currentKey, key))
      {
        current = current->right_;
      }
      else
      {
        return current;
      }
    }
    return nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::replaceSubtree(
      TreeNodeBase * oldRoot, TreeNodeBase * newRoot)
  {
    TreeNodeBase * parent = oldRoot->parent_;
    if (parent == &fakeRoot_)
    {
      setRoot(newRoot);
      return;
    }
    if (parent->left_ == oldRoot)
    {
      parent->left_ = newRoot;
    }
    else
    {
      parent->right_ = newRoot;
    }
    if (newRoot != nullptr)
    {
      newRoot->parent_ = parent;
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::eraseNode(TreeNodeBase * node)
  {
    if (node->left_ == nullptr)
    {
      replaceSubtree(node, node->right_);
    }
    else if (node->right_ == nullptr)
    {
      replaceSubtree(node, node->left_);
    }
    else
    {
      TreeNodeBase * successor = minimum(node->right_);
      if (successor->parent_ != node)
      {
        replaceSubtree(successor, successor->right_);
        successor->right_ = node->right_;
        successor->right_->parent_ = successor;
      }
      replaceSubtree(node, successor);
      successor->left_ = node->left_;
      successor->left_->parent_ = successor;
    }
    delete asNode(node);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::destroySubtree(TreeNodeBase * node)
  {
    if (node != nullptr)
    {
      destroySubtree(node->left_);
      destroySubtree(node->right_);
      delete asNode(node);
    }
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase * BSTree< Key, Value, Compare >::cloneSubtree(
      const TreeNodeBase * node, TreeNodeBase * parent)
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    const Node * source = asNode(node);
    Node * copy = new Node(source->value_.first, source->value_.second);
    copy->parent_ = parent;
    try
    {
      copy->left_ = cloneSubtree(node->left_, copy);
      copy->right_ = cloneSubtree(node->right_, copy);
    }
    catch (...)
    {
      destroySubtree(copy);
      throw;
    }
    return copy;
  }

  template< class Key, class Value, class Compare >
  std::size_t BSTree< Key, Value, Compare >::subtreeHeight(
      const TreeNodeBase * node)
  {
    if (node == nullptr)
    {
      return 0;
    }
    const std::size_t leftHeight = subtreeHeight(node->left_);
    const std::size_t rightHeight = subtreeHeight(node->right_);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::validatePosition(
      const_iterator position) const
  {
    if (position.fakeRoot_ != &fakeRoot_
        || position.node_ == nullptr || position.node_ == &fakeRoot_)
    {
      throw std::invalid_argument("Invalid tree iterator");
    }
  }
}

#endif
