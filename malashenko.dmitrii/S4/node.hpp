#ifndef BSTREENODE
#define BSTREENODE
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
  private:
    Key key_;
    Value value_;
    Node* right_;
    Node* left_;
    Node* parent_;

    template< class Compare >
    friend class BSTree;

    friend class BSTreeIter< Key, Value >;
    friend class BSTreeCIter< Key, Value >;
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
  node_t root = *this;
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
  node_t root = *this;
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
#endif