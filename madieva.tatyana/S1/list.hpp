namespace madieva {
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
  public:
    LIter< T >(Node< T > * a);
    void operator++();
    void operator--();
    bool operator==(LIter< T > & a) const;
    bool operator!=(LIter< T > & a) const;
    T& operator*();
  };

  template< class T >
  class LCIter {
    friend class List< T >;
    const Node< T > * it;
  public:
    LCIter< T >(const Node< T > * a);
    void operator++();
    void operator--();
    bool operator==(LCIter< T > & a) const;
    bool operator!=(LCIter< T > & a) const;
    const T& operator*() const;
  };

  template< class T > 
  class List {
    Node< T > * head;
    size_t size_;
    public:
    List();
    ~List();
    List(const List& a);
    List& operator=(const List& a);
    void clear();
    void push_front(T a);
    void push_back(T a);
    void pop_front();
    void pop_back();
    LIter< T > begin();
    LCIter< T > begin() const;
  };

  template< class T >
  Node< T >::Node(T a, Node< T > * n, Node< T > * p) : val(a), next(n), prev(p)
  {}

  template< class T >
  LIter< T >::LIter(Node< T > * a) :it(a)
  {}

  template< class T >
  void LIter< T >::operator++()
  {
    assert(it);
    it = it->next;
  }

  template< class T >
  void LIter< T >::operator--()
  {
    assert(it);
    it = it->prev;
  }

  template< class T >
  bool LIter< T >::operator==(LIter< T > & a) const
  {
    return(it == a.it);
  }

  template< class T >
  bool LIter< T >::operator!=(LIter< T > & a) const
  {
    return(it != a.it);
  }

  template< class T >
  T& LIter< T >::operator*()
  {
    assert(it);
    return it->val;
  }

  template< class T >
  LCIter< T >::LCIter(const Node< T > * a) : it(a)
  {}

  template< class T >
  void LCIter< T >::operator++()
  {
    assert(it);
    it = it->next;
  }

  template< class T >
  void LCIter< T >::operator--()
  {
    assert(it);
    it = it->prev;
  }

  template< class T >
  bool LCIter< T >::operator==(LCIter< T > & a) const
  {
    return(it == a.it);
  }

  template< class T >
  bool LCIter< T >::operator!=(LCIter< T > & a) const
  {
    return(it != a.it);
  }

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    assert(it);
    return it->val;
  }


  template< class T >
  List< T >::List() : head(nullptr), size_(0)
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >::List(const List< T > & a) : head(nullptr), size_(0)
  {
    Node< T > * temp = a.head;
    for (size_t i = 0; i < a.size_; ++i) {
      this->push_back(temp->val);
      temp = temp->next;
    }
  }

  template< class T >
  List< T > & List< T >::operator=(const List< T > & a)
  {
    if(this == &a) {
      return *(this);
    }
    this->clear();
    Node< T > * temp = a.head;
    for (size_t i = 0; i < a.size_; ++i) {
      this->push_back(temp->val);
      temp = temp->next;
    }
    return *(this);
  }


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
  LCIter< T > List< T >::begin() const {
    return LCIter< T > (head);
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

  template< class T >
  void List< T >::push_back(T a)
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
      size_++;
    }
  }

  template< class T >
  void List< T >::pop_front()
  {
    if(size_) {
      if(size_ == 1) {
        delete head;
        head = nullptr;
        size_ = 0;
      } else {
        head->next->prev = head->prev;
        head->prev->next = head->next;
        Node< T > * a = head->next;
        delete head;
        head = a;
        size_--;
      }
    }
  }

  template< class T >
  void List< T >::pop_back()
  {
    if (size_) {
      if(size_ == 1) {
        delete head;
        head= nullptr;
        size_ = 0;
      } else{
        Node< T > * a = head->prev;
        a->next->prev = a->prev;
        a->prev->next = a->next;
        delete a;
        size_--;
      }
    }
  }
}