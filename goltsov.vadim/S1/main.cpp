#include <iostream>

template< class T >
class List;
template< class T >
class LIter
{
  friend class List< T >;
  T value;
  List< T >* next;
public:
  bool hasNext() const
  {
    return next != nullptr;
  }
  LIter next()
  {
    return {next->value, next->next};
  }
  T& operator*() const
  {
    return value;
  }
};
template< class T >
class LCIter
{
  friend class List< T >;
  const T value;
  const List< T > const * next;
public:
  bool hasNext() const
  {
    return next != nullptr;
  }
  LIter next()
  {
    return {next->value, next->next};
  }
  T& operator*() const
  {
    return value;
  }
};
template< class T >
class List
{
  struct Node
  {
    T value;
    List< T >* next;
  }
  Node* fake;
  Node* createFake();
  Node* rmFake();
  size_t size;
public:
  List();
  ~List();
  List(List& other) const;
  List(List&& other);
  List< T >& operator=(const List< T >& other) const;
  List< T >& operator=(List< T >&& other) const;

  LIter< T > begin() const;
  LIter< T > end() const;
  void push_start(const List< T >& other);
  void push_back(const List< T >& other);
  List< T >& pop_start();
  List< T >& pop_end();
  void insert(const T& a);
  void clear();
};


int main()
{

}