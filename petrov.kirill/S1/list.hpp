#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>

namespace petrov {
  template<typename T>
  struct Node {
    T data;
    Node* next;
    Node(const T& val) : data(val), next(nullptr) {}
  };

  template<typename T>
  class List {
  public:
    Node<T>* head;
    Node<T>* tail;
    List() : head(nullptr), tail(nullptr) {}

    void push_back(const T& val) {
      Node<T>* newNode = new Node<T>(val);
      if (!head) head = tail = newNode;
      else {
        tail->next = newNode;
        tail = newNode;
      }
    }
  };
}
#endif