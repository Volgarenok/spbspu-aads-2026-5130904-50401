#ifndef BSTREENODE
#define BSTREENODE

namespace malashenko
{
  template< class Key, class Value >
  class Node {
  public:
    Node(const Key& key, const Value& value);
  private:
    Key key_;
    Value value_;
    Node* right_;
    Node* left_;
    Node* parent_;
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

#endif