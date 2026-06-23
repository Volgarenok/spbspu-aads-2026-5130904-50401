#ifndef ROBIN_NODE_HPP
#define ROBIN_NODE_HPP

namespace donkeev
{
  template<class Key, class Value>
  struct RobinNode
  {
    Key key_;
    Value value_;
    bool isOccupied_;
    int psl_;
    
    RobinNode();

    RobinNode(const RobinNode&);
    RobinNode(RobinNode&&) noexcept;
    
    RobinNode(const Key&, const Value&, int);

    RobinNode& operator=(const RobinNode&);
  
    void swap(RobinNode&);
  };
}

#endif
