template< class T > class List;

template< class T >
class Node {
  friend class List< T >;
  T val;
  Node< T > * next;
  Node< T > * prev;
  Node(T a);
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
  void push_front(T a);
  LIter< T > begin();
  LIter< T > end();
};

template< class T >
Node< T >::Node(T a) : val(a), next(nullptr), prev(nullptr)
{}

template< class T >
List< T >::List() : head(nullptr), size_(0)
{}

template< class T >
LIter< T > List< T >::begin() {
  return head;
}

template< class T >
LIter< T > List< T >::end() {
  return nullptr;
}

template< class T >
void List< T >::push_front(T a)
{
  if (!head) {
    head = new Node< T >(a);
    head->next = head;
    head->prev = head;
    size_ = 1;
  } else {
    Node< T > * temp = new Node< T >(a);;
    temp->next = head;
    temp->prev = head->prev;
    head->prev->next = temp;
    head->prev = temp;
    head = temp;
    size_++;
  }
}