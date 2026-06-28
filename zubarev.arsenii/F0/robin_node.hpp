#ifndef MY_ROBIN_NODE_HPP
#define MY_ROBIN_NODE_HPP

namespace zubarev
{
  template< class Key, class Value >
  struct RobinNode
  {

    RobinNode();
    RobinNode(Key, Value, bool, int);

    Key key_;
    Value val_;
    bool occupied_ = false;
    int psl_ = -1;
  };

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode():
    key_(),
    val_(),
    occupied_(false),
    psl_(-1)
  {}

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode(Key key, Value val, bool occupied, int psl):
    key_(key),
    val_(val),
    occupied_(occupied),
    psl_(psl)
  {}
}

#endif
