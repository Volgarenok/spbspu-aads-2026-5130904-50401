namespace donkeev
{
  template< class Key, class Value >
  struct Node
  {
    Key key_;
    Value value_;
    bool isOccupied_;

    Node();
    Node(const Key&, const Value&);
    bool isEmpty();
    Value dropNode();
  };

  template< class Key, class Value >
  Node< Key, Value >::Node():
    key_(),
    value_(),
    isOccupied_(false)
  {}

  template< class Key, class Value >
  Node< Key, Value >::Node(const Key& key, const Value& value):
    key_(key),
    value_(value),
    isOccupied_(true)
  {}

  template< class Key, class Value >
  bool Node< Key, Value >::isEmpty()
  {
    return isOccupied_ == false;
  }

  template< class Key, class Value >
  Value Node< Key, Value >::dropNode()
  {
    Value tmp = value_;

    key_.~Key();
    value_.~Value();
    isOccupied_ = false;

    return tmp;
  }
}
