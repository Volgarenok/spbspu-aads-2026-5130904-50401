#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace vasyakin
{
  namespace detail
  {
    template< class Key, class Value >
    class Node
    {
    public:
      std::pair< const Key, Value > value_;
      Node* left_;
      Node* right_;
      Node* parent_;
      int height_;

      template< class K, class V >
      Node(K&& key, V&& value);
    };

    template< class Key, class Value >
    template< class K, class V >
    Node< Key, Value >::Node(K&& key, V&& value):
      value_(std::forward< K >(key), std::forward< V >(value)),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr),
      height_(1)
    {}

    template< class NodePtr >
    void findNextNode(NodePtr& node) noexcept
    {
      if (!node)
      {
        return;
      }

      if (node->right_)
      {
        node = node->right_;

        while (node->left_)
        {
          node = node->left_;
        }
      }
      else
      {
        NodePtr parent = node->parent_;
        while (parent && node == parent->right_)
        {
          node = parent;
          parent = parent->parent_;
        }

        node = parent;
      }
    }

    template< class NodePtr >
    void findPrevNode(NodePtr& node) noexcept
    {
      if (!node)
      {
        return;
      }

      if (node->left_)
      {
        node = node->left_;

        while (node->right_)
        {
          node = node->right_;
        }
      }
      else
      {
        NodePtr parent = node->parent_;
        while (parent && node == parent->left_)
        {
          node = parent;
          parent = parent->parent_;
        }

        node = parent;
      }
    }
  }
}

#endif
