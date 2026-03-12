#include <vector>
#include <string>
#include <iostream>
template <typename T>
class Stack
{
private:
  T* data;
  
public:
  void push(const T& rhs);
  T drop();
  const T& top();
  bool empty();
  size_t size();
};
