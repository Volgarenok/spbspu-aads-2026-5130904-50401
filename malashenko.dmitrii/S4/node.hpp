#ifndef BSTREENODE
#define BSTREENODE
#include <algorithm>
#include <cstddef>
namespace malashenko
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTreeIter;

  template< class Key, class Value >
  class BSTreeCIter;

  template< class Key, class Value>
  class Node {
  public:
    using node_t = Node< Key, Value >;
    Node(const Key& key, const Value& value);
    node_t* minimum(node_t* fakeLeaf);
    node_t* maximum(node_t* fakeLeaf);
    node_t* root();
    size_t height(node_t* fakeLeaf) const;
  private:
    Key key_;
    Value value_;
    Node* right_;
    Node* left_;
    Node* parent_;

    template <class K, class V, class C>
    friend class BSTree;

    template <class K, class V>
    friend class BSTreeIter;

    template <class K, class V>
    friend class BSTreeCIter;
  };
}

template< class Key, class Value >
malashenko::Node< Key, Value >::Node(const Key& key, const Value& value):
  key_(key),
  value_(value),
  right_(nullptr),
  left_(nullptr),
  parent_(nullptr)
{}

template< class Key, class Value >
malashenko::Node< Key, Value >* malashenko::Node< Key, Value >::minimum(node_t* fakeLeaf)
{
  node_t* root = this;
  if (!root)
  {
    return root;
  }

  while (root->left_ && root->left_ != fakeLeaf)
  {
    root = root->left_;
  }
  return root;
}


template< class Key, class Value >
malashenko::Node< Key, Value >* malashenko::Node< Key, Value >::maximum(node_t* fakeLeaf)
{
  node_t* root = this;
  if (!root)
  {
    return root;
  }

  while (root->right_ && root->right_ != fakeLeaf)
  {
    root = root->right_;
  }
  return root;
}
template< class Key, class Value >
malashenko::Node< Key, Value >* malashenko::Node< Key, Value >::root()
{
  node_t* root = this;
  while (root->parent_)
  {
    root = root->parent_;
  }
  return root;
}


template< class Key, class Value >
size_t malashenko::Node< Key, Value >::height(node_t* fakeLeaf) const
{
  if (this == fakeLeaf)
  {
    return 0;
  }
  if (left_ == fakeLeaf && right_ == fakeLeaf)
  {
    return 1;
  }
  return std::max(left_->height(fakeLeaf), right_->height(fakeLeaf)) + 1;
}

#endif
