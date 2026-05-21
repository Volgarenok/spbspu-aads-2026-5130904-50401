#ifndef MY_NODE_TREE_HPP
#define MY_NODE_TREE_HPP
#include <iostream>
namespace zubarev
{

  template < class Key, class Value >
  struct BSTreeNode
  {
    using Node = BSTreeNode< Key, Value >;

    BSTreeNode();
    BSTreeNode(Key, Value);
    ~BSTreeNode();
    BSTreeNode(const BSTreeNode& node);
    BSTreeNode(BSTreeNode&& node) noexcept;
    BSTreeNode& operator=(const BSTreeNode& other);
    BSTreeNode& operator=(BSTreeNode&& other) noexcept;
    void swap(BSTreeNode& other) noexcept;

    std::pair< Key, Value > data_;
    BSTreeNode< Key, Value >* left_;
    BSTreeNode< Key, Value >* right_;
    BSTreeNode< Key, Value >* parent_;
  };
  template < class Key, class Value >
  void BSTreeNode< Key, Value >::swap(BSTreeNode& other) noexcept
  {
    std::swap(other.data_);
    std::swap(other.right_);
    std::swap(other.left_);
    std::swap(other.parent_);
  }

  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode():
    data_(std::make_pair(Key{}, Value{})),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
  {}

  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode(Key key, Value val):
    data_(std::make_pair(key, val)),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
  {}
  template < class Key, class Value >
  BSTreeNode< Key, Value >::~BSTreeNode()
  {}

  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode(const BSTreeNode& node):
    data_(node.data_),
    right_(node.right_),
    left_(node.left_),
    parent_(node.parent_)
  {}
  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode(BSTreeNode&& node) noexcept:
    data_(std::move(node.data_))
  {
    node.right_ = nullptr;
    node.left_ = nullptr;
    node.parent_ = nullptr;
  }
  template < class Key, class Value >
  BSTreeNode< Key, Value >& BSTreeNode< Key, Value >::operator=(const BSTreeNode& rhs)
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }
    BSTreeNode cpy(rhs);
    swap(cpy);
    return *this;
  }
  template < class Key, class Value >
  BSTreeNode< Key, Value >& BSTreeNode< Key, Value >::operator=(BSTreeNode&& rhs) noexcept
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    BSTreeNode cpy(std::move(rhs));
    swap(cpy);
    return *this;
  }

};

#endif
