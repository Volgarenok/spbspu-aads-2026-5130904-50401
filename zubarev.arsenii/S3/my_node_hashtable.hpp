#ifndef MY_NODE_HPP
#define MY_NODE_HPP

namespace zubarev
{
  template < class Key, class Value >
  struct NodeHashTable
  {
    NodeHashTable();
    NodeHashTable(Key, Value, bool);

    Key key_;
    Value val_;
    bool is_val_;
  };
  template < class Key, class Value >
  NodeHashTable< Key, Value >::NodeHashTable():
    key_(),
    val_(),
    is_val_(false)
  {}
  template < class Key, class Value >
  NodeHashTable< Key, Value >::NodeHashTable(Key key, Value val, bool is_val):
    key_(key),
    val_(val),
    is_val_(is_val)
  {}
}

#endif
