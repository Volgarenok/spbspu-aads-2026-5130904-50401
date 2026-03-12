#include "node.hpp"
#include <iostream>
#include <string>
#include <vector>

template < typename T >
class Stack
{
private:
  Node* head_;
  // Node* tail_;

public:
  void push(const T& rhs);
  T drop();
  const T& top();
  bool empty();
  size_t size();
};
