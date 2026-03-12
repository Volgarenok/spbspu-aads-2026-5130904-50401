#include "node.hpp"
namespace zubarev
{
  template <typename T>

  class Queue
  {
  private:
    Node *head_;
    Node *tail_;

  public:
    void push(const T &rhs);
    T drop();
  };

}
