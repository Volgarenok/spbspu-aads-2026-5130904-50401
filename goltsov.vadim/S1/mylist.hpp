#ifndef MYLIST_HPP
#define MYLIST_HPP
namespace goltsov
{
  template< class T >
  struct Node
  {
    T value;
    Node< T >* next;
  };
  template< class T >
  struct CNode
  {
    const T value;
    const CNode< T >* next;
  };
  template< class T >
  class List;
  template< class T >
  class LIter
  {
    friend class List< T >;
    Node< T >* ptr;
  public:
    bool hasNext() const;
    LIter< T > next() const;
    T& operator*() const;
  };
  template< class T >
  class LCIter
  {
    friend class List< T >;
    CNode< T >* ptr;
  public:
    bool hasNext() const;
    LCIter< T > next() const;
    const T& operator*() const;
  };
  template< class T >
  class List
  {
    Node< T >* fake;
    Node< T >* createFake();
    void rmFake();
  public:
    List();
    ~List();
    List(List< T >& other);
    List(List< T >&& other);
    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other);

    LIter<T> begin();
    LCIter<T> begin() const;
    LIter<T> end();
    LCIter<T> end() const;
    void push_start(const T& a);
    void push_back(const T& a);
    void pop_start();
    void pop_end();
    void insert(LIter< T > i, const T& a);
    void clear();
  };
}
#endif
