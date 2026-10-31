#ifndef BSNODE_HPP
#define BSNODE_HPP

#include <cassert>
#include <utility>
namespace khasnulin
{

  template < class Key, class Value > struct Node;
  template < class Key, class Value > struct NodeBase
  {
    NodeBase *parent;
    Node< Key, Value > *left;
    Node< Key, Value > *right;

    NodeBase() noexcept;
  };

  template < class Key, class Value >
  NodeBase< Key, Value >::NodeBase() noexcept:
      parent(nullptr),
      left(nullptr),
      right(nullptr)
  {
  }

  template < class Key, class Value > struct Node : NodeBase< Key, Value >
  {
    using value_pair = std::pair< const Key, Value >;
    value_pair val;

    explicit Node(const value_pair &rhs);
    explicit Node(value_pair &&rhs);

    Node(const Node &) = delete;
    Node &operator=(const Node &) = delete;
  };

  template < class Key, class Value >
  Node< Key, Value >::Node(const value_pair &rhs):
      NodeBase< Key, Value >(),
      val(rhs)
  {
  }

  template < class Key, class Value >
  Node< Key, Value >::Node(value_pair &&rhs):
      NodeBase< Key, Value >(),
      val(std::move(rhs))
  {
  }

  // Вызывать с Node* или NodeBase*
  template < class NodePtr > NodePtr minNode(NodePtr node) noexcept
  {
    assert(node != nullptr && "node can't be nullptr");
    while (node->left)
    {
      node = node->left;
    }
    return node;
  }

  // Вызывать с Node* или NodeBase*
  template < class NodePtr > NodePtr maxNode(NodePtr node) noexcept
  {
    assert(node != nullptr && "node can't be nullptr");
    while (node->right)
    {
      node = node->right;
    }
    return node;
  }

  // Вызывать с NodeBase*
  template < class NodePtr > NodePtr nextNode(NodePtr node) noexcept
  {
    assert(node != nullptr && "node can't be nullptr");
    if (node->right)
    {
      return minNode(node->right);
    }
    NodePtr prev = node;
    node = node->parent;
    while (node && node->right == prev)
    {
      prev = node;
      node = node->parent;
    }
    return node;
  }

  // Вызывать с NodeBase*
  template < class NodePtr > NodePtr prevNode(NodePtr node) noexcept
  {
    assert(node != nullptr && "node can't be nullptr");
    if (node->left)
    {
      return maxNode(node->left);
    }
    NodePtr prev = node;
    node = node->parent;
    while (node && node->left == prev)
    {
      prev = node;
      node = node->parent;
    }
    return node;
  }

  template < class Key, class Value >
  void setLeft(NodeBase< Key, Value > *node, Node< Key, Value > *child) noexcept
  {
    assert(node != nullptr);
    node->left = child;
    if (child)
    {
      child->parent = node;
    }
  }

  template < class Key, class Value >
  void setRight(NodeBase< Key, Value > *node, Node< Key, Value > *child) noexcept
  {
    assert(node != nullptr);
    node->right = child;
    if (child)
    {
      child->parent = node;
    }
  }

  template < class Key, class Value >
  void replaceChild(NodeBase< Key, Value > *parent, const Node< Key, Value > *oldChild,
                    Node< Key, Value > *newChild) noexcept
  {
    assert(parent != nullptr && oldChild != nullptr);
    assert(oldChild->parent == parent);
    if (parent->left == oldChild)
    {
      setLeft(parent, newChild);
    }
    else
    {
      setRight(parent, newChild);
    }
  }
}

#endif
