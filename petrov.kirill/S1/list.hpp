#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>
#include <memory>

namespace petrov
{
  template<class T>
  struct node
  {
    T val;
    node<T>* next;
    node<T>* prev;
  };
};
