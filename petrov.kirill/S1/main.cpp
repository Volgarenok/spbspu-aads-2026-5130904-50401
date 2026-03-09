#include <iostream>
#include <string>

namespace petrov
{
  template<class T>
  class List;
  template<class T>
  class LIter
  {
    List<T>* curr;
    LIter(List<T>* p) : curr(p)
    {}  
  public:
    T& operator*()
    {
      return curr->val;
    }
    LIter& operator++()
    {
      curr = curr->next;
      return *this;
    }
    bool operator==(LIter& LIt)
    {
      return curr == LIt.curr;
    }
    friend class List<T>;
  };
    template<class T>
  class List
  {
    public:
      T val;
      List<T>* next;
      LIter<T> begin()
      {
        return LIter(this);
      }
      LIter<T> end()
      {
        return LIter(nullptr);
      }
  };
}

int main()
{
  petrov::List<int> d;
  d.next = new petrov::List<int>;
  d.next->val = 1;
  d.val = 2;
  petrov::LIter<int> c = d.begin();
  std::cout << *c << "\n";
  ++c;
  std::cout << *c << "\n";
}

