template< class T > class List;

template< class T >
class Node {
  friend class List< T >;
  T val;
  Node< T > * next;
  Node< T > * prev;
  Node(T a, Node< T > * n, Node< T > * p);
};

template< class T >
class LIter {
  friend class List< T >;
  Node< T > * it;
};

template< class T >
class LCIter {
  friend class List< T >;
  const Node< T > * it;
};

template< class T > 
class List {
  Node< T > * head;
  size_t size_;
  public:
  List();
  void clear();
  void push_front(T a);
  LIter< T > begin();
  LIter< T > end();
  LCIter< T > begin() const;
  LCIter< T > end() const;
};

template< class T >
Node< T >::Node(T a, Node< T > * n, Node< T > * p) : val(a), next(n), prev(p)
{}

template< class T >
List< T >::List() : head(nullptr), size_(0)
{}

template< class T >
void List< T >::clear()
{
  if (head) {
    Node< T > * end = head->prev;
    while (head != end) {
      Node< T > * temp = head->next;
      delete head;
      head = temp;
    }
    delete head;
    head = nullptr;
  }
}

template< class T >
LIter< T > List< T >::begin() {
  return LIter< T > (head);
}

template< class T >
LIter< T > List< T >::end() {
  return LIter< T > (nullptr);
}

template< class T >
LCIter< T > List< T >::begin() const {
  return LCIter< T > (head);
}

template< class T >
LCIter< T > List< T >::end() const {
  return  LCIter< T > (nullptr);
}

template< class T >
void List< T >::push_front(T a)
{
  if (!head) {
    head = new Node< T >(a, nullptr, nullptr);
    head->next = head;
    head->prev = head;
    size_ = 1;
  } else {
    Node< T > * temp = new Node< T >(a, head, head->prev);
    head->prev->next = temp;
    head->prev = temp;
    head = temp;
    size_++;
  }
}