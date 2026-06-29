#ifndef MY_NODE_HPP
#define MY_NODE_HPP

namespace zubarev
{
  namespace detail
  {
    template< class Key, class Value >
    struct NodeHashTable
    {

      NodeHashTable();
      NodeHashTable(Key, Value, bool);

      Key key;
      Value val;
      bool is_val;
    };
  }
  template< class Key, class Value >
  detail::NodeHashTable< Key, Value >::NodeHashTable():
    key(),
    val(),
    is_val(false)
  {}
  template< class Key, class Value >
  detail::NodeHashTable< Key, Value >::NodeHashTable(Key key, Value val, bool is_val):
    key(key),
    val(val),
    is_val(is_val)
  {}
}

#endif
