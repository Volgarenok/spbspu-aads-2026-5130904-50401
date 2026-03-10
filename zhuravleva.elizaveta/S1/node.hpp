#ifndef NODE_HPP
#define NODE_HPP

template<class T>
struct Node
{
  T data;
  Node<T>* next;

  Node(const T& value, Node<T>* n = nullptr):
    data(value),
    next(n)
  {}
};

#endif
