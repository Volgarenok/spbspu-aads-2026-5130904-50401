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
    LIter() noexcept;
    LIter(Node< T >* p) noexcept;
    bool hasNext() const noexcept;
    LIter< T > next() const noexcept;
    T& operator*() const noexcept;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    CNode< T >* ptr;
  public:
    LCIter() noexcept;
    LCIter(CNode< T >* p) noexcept;
    bool hasNext() const noexcept;
    LCIter< T > next() const noexcept;
    const T& operator*() const noexcept;
  };

  template< class T >
  class List
  {
    Node< T >* fake;
    Node< T >* createFake();
    void rmFake() noexcept;
  public:
    List();
    ~List();
    List(const List< T >& other);
    List(List< T >&& other);
    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other);

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LIter< T > getLast() noexcept;
    LCIter< T > getLast() const noexcept;
    void push_start(const T& a);
    void pop_start() noexcept;
    void pop_end() noexcept;
    void insert(LIter< T >& i, const T& a);
    void insert(LIter< T >& i, const T&& a);
    void clear() noexcept;
  };
}

namespace goltsov
{
  template< class T >
  LIter< T >::LIter() noexcept:
    ptr(nullptr)
  {}
  template< class T >
  LIter< T >::LIter(Node< T >* p) noexcept:
    ptr(p)
  {}
  template< class T >
  bool LIter< T >::hasNext() const noexcept
  {
    return ptr != nullptr;
  }
  template< class T >
  LIter< T > LIter< T >::next() const noexcept
  {
    return {ptr->next};
  }
  template< class T >
  T& LIter< T >::operator*() const noexcept
  {
    return ptr->value;
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    ptr(nullptr)
  {}
  template< class T >
  LCIter< T >::LCIter(CNode< T >* p) noexcept:
    ptr(p)
  {}
  template< class T >
  bool LCIter< T >::hasNext() const noexcept
  {
    return ptr != nullptr;
  }
  template< class T >
  LCIter< T > LCIter< T >::next() const noexcept
  {
    return {ptr->next};
  }
  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    return {ptr->value};
  }

  template< class T >
  Node< T >* List< T >::createFake()
  {
    Node< T >* el = new Node< T > {T(), nullptr};
    return el;
  }
  template< class T >
  void List< T >::rmFake() noexcept
  {
    delete fake;
  }
  template< class T >
  List< T >::List()
  {
    fake = createFake();
    fake->next = nullptr;
  }
  template< class T >
  List< T >::~List()
  {
    clear();
    rmFake();
  }
  template< class T >
  List< T >::List(const List< T >& other)
  {
    fake = createFake();
    fake->next = nullptr;
    if (!other.fake->next)
    {
      return;
    }
    Node< T >* now_old = other.fake->next;
    Node< T >* now_new = new Node< T >{now_old->value, nullptr};
    fake->next = now_new;
    now_old = now_old->next;
    while(now_old != nullptr)
    {
        now_new->next = new Node< T >{now_old->value, nullptr};
        now_new = now_new->next;
        now_old = now_old->next;
    }
  }
  template< class T >
  List< T >::List(List< T >&& other)
  {
    fake = createFake();
    fake->next = other.fake->next;
    other.fake->next = nullptr;
  }
  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != &other)
    {
      clear();
      fake->next = nullptr;
      if (other.fake->next)
      {
        Node< T >* now_old = other.fake->next;
        Node< T >* now_new = new Node< T > {now_old->value, nullptr};
        fake->next = now_new;
        now_old = now_old->next;
        while(now_old != nullptr)
        {
          now_new->next = new Node< T > {now_old->value, nullptr};
          now_new = now_new->next;
          now_old = now_old->next;
        }
      }
    }
    return *this;
  }
  template< class T >
  List< T >& List< T >::operator=(List< T >&& other)
  {
    if (this != & other)
    {
      clear();
      fake->next = other.fake->next;
      other.fake->next = nullptr;
    }
    return * this;
  }
  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return {fake->next};
  }
  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return {const_cast< const Node< T >* >(fake->next)};
  }
  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return {nullptr};
  }
  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return {nullptr};
  }
  template< class T >
  LIter< T > List< T >::getLast() noexcept
  {
    LIter< T > now = this->begin();
    while(now.hasNext() && now.next() != nullptr)
    {
      now = now.next();
    }
    return now;
  }
  template< class T >
  LCIter< T > List< T >::getLast() const noexcept
  {
    LCIter< T > now = this->begin();
    while(now.hasNext() && now.next() != nullptr)
    {
      now = now.next();
    }
    return now;
  }
  template< class T >
  void List< T >::push_start(const T& a)
  {
    Node< T >* new_el = new Node< T > {a, fake->next};
    fake->next = new_el;
  }
  template< class T >
  void List< T >::pop_start() noexcept
  {
    Node< T >* n = fake->next->next;
    delete fake->next;
    fake->next = n;
  }
  template< class T >
  void List< T >::pop_end() noexcept
  {
    Node< T >* now = fake;
    if (now->next == nullptr)
    {
      return;
    }
    while (now->next->next != nullptr)
    {
      now = now->next;
    }
    delete now->next;
    now->next = nullptr;
  }
  template< class T >
  void List< T >::insert(LIter< T >& i, const T& a)
  {
    if (i.ptr == nullptr)
    {
      push_start(a);
      i = {fake};
    }
    else
    {
      Node< T >* new_el = new Node< T > {a, i.next().ptr};
      i.ptr->next = new_el;
    }
  }
  template< class T >
  void List< T >::insert(LIter< T >& i, const T&& a)
  {
    if (i.ptr == nullptr)
    {
      push_start(a);
      i = {fake};
    }
    else
    {
      Node< T >* new_el = new Node< T > {a, i.next().ptr};
      i.ptr->next = new_el;
    }
  }
  template< class T >
  void List< T >::clear() noexcept
  {
    while (fake->next != nullptr)
    {
      Node< T >* temp = fake->next;
      fake->next = temp->next;
      delete temp;
    }
  }
}
#endif
