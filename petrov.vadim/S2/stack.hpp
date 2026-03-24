#include "common/STRUCTS.hpp"

namespace petrov
{
  template<typename T>
  class Stack {
  public:
    Stack();
    Stack(const Stack<T>& s);
    Stack(Stack<T>&& s);

    Stack<T>& operator=(const Stack<T>& s);
    Stack<T>& operator=(Stack<T>&& s);

    void push(const T& rhs);
    T drop();

    const T& top() const;
    void pop();
    bool empty() const;
    size_t getSize() const;
    void clear();
    ~Stack();

  private:
    List<T> list_;
  };
}
