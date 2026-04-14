#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>

namespace petrov
{
  template< class T >
  struct node
  {
    T val;
    node<T>* next;
    node<T>* prev;
    node(const T& v, node<T>* n = nullptr, node<T>* p = nullptr) : val(v), next(n), prev(p)
    {}
  };

  template<class T>
  class LIter;

  template<class T>
  class LCIter;

  template<class T>
  class List
  {
    node<T>* h;
    node<T>* t;
    size_t s;
  public:
    ~List();
    List();
    List(const List<T>& l);
    List(List<T>&& l) noexcept;
    List(size_t s, const T& init);

    List<T>& operator=(List<T>&& l) noexcept;
    List<T>& operator=(const List<T>& l);

    bool IsEmpty() const noexcept;
    size_t getSize() const noexcept;
    LIter<T> begin();
    LIter<T> end();
    LCIter<T> begin() const;
    LCIter<T> end() const;

    void push_back(const T& d);
    void push_front(const T& d);
    void pop_back();
    void pop_front();
    void clear();
  };

  template<class T>
  class LIter
  {
    friend class List<T>;
    node<T>* cur;
    LIter(node<T>* n);
  public:
    bool operator==(const LIter<T>& i) const;
    bool operator!=(const LIter<T>& i) const;
    T& operator*() const;
    LIter<T>& operator++();
    LIter<T>& operator--();
    T* operator->() const;
  };

  template<class T>
  class LCIter
  {
    friend class List<T>;
    const node<T>* cur;
    LCIter(const node<T>* n);
  public:
    bool operator==(const LCIter<T>& i) const;
    bool operator!=(const LCIter<T>& i) const;
    const T& operator*() const;
    LCIter<T>& operator++();
    LCIter<T>& operator--();
    const T* operator->() const;
  };

  template<class T>
  List<T>::List():
    h(nullptr),
    t(nullptr),
    s(0)
  {}

  template<class T>
  bool LCIter<T>::operator==(const LCIter<T>& i) const
  {
    return !(cur != i.cur);
  }

  template<class T>
  bool LCIter<T>::operator!=(const LCIter<T>& i) const
  {
    return cur != i.cur;
  }

  template<class T>
  const T& LCIter<T>::operator*() const
  {
    return cur->val;
  }

  template<class T>
  const T* LCIter<T>::operator->() const
  {
    return &(cur->val);
  }

  template<class T>
  LCIter<T>& LCIter<T>::operator++()
  {
    if (cur != nullptr)
    {
      cur = cur->next;
    }
    return *this;
  }

  template<class T>
  LCIter<T>& LCIter<T>::operator--()
  {
    if (cur != nullptr)
    {
      cur = cur->prev;
    }
    return *this;
  }

  template<class T>
  bool LIter<T>::operator==(const LIter<T>& i) const
  {
    return !(cur != i.cur);
  }

  template<class T>
  bool LIter<T>::operator!=(const LIter<T>& i) const
  {
    return cur != i.cur;
  }

  template<class T>
  LIter<T>::LIter(node<T>* n):
    cur(n)
  {}

  template<class T>
  LCIter<T>::LCIter(const node<T>* n):
    cur(n)
  {}

  template<class T>
  void List<T>::clear()
  {
    if (h != nullptr)
    {
      while (h->next != nullptr)
      {
        node<T>* promej = h->next;
        delete h;
        h = promej;
      }
      delete h;
    }
    h = nullptr;
    t = nullptr;
    s = 0;
  }

  template<class T>
  List<T>::~List()
  {
    clear();
  }

  template<class T>
  void List<T>::pop_front()
  {
    if (!IsEmpty())
    {
      node<T>* new_h = h->next;
      delete h;
      h = new_h;
      if (h == nullptr)
      {
        t = nullptr;
        s = 0;
        return;
      }
      h->prev = nullptr;
      s--;
    }
  }

  template<class T>
  void List<T>::pop_back()
  {
    if (!IsEmpty())
    {
      node<T>* new_t = t->prev;
      delete t;
      t = new_t;
      if (t == nullptr)
      {
        h = nullptr;
        s = 0;
        return;
      }
      t->next = nullptr;
      s--;
    }
  }

  template<class T>
  bool List<T>::IsEmpty() const noexcept
  {
    return (!s || h == nullptr);
  }

  template<class T>
  size_t List<T>::getSize() const noexcept
  {
    return s;
  }

  template<class T>
  void List<T>::push_back(const T& d)
  {
    if (IsEmpty())
    {
      h = new petrov::node<T>(d);
      h->next = nullptr;
      h->prev = nullptr;
      t = h;
      s = 1;
      return;
    }
    t->next = new node<T>(d);
    t->next->prev = t;
    t = t->next;
    t->next = nullptr;
    s++;
  }

  template<class T>
  void List<T>::push_front(const T& d)
  {
    if (IsEmpty())
    {
      h = new node<T>(d);
      h->next = nullptr;
      h->prev = nullptr;
      t = h;
      s = 1;
      return;
    }
    h->prev = new node<T>(d);
    h->prev->next = h;
    h = h->prev;
    h->prev = nullptr;
    s++;
  }

  template<class T>
  LIter<T> List<T>::begin()
  {
    return LIter(h);
  }

  template<class T>
  LIter<T> List<T>::end()
  {
    return LIter<T>(nullptr);
  }

  template<class T>
  LIter<T>& LIter<T>::operator++()
  {
    if (cur != nullptr)
    {
      cur = cur->next;
    }
    return *this;
  }

  template<class T>
  LIter<T>& LIter<T>::operator--()
  {
    if (cur != nullptr)
    {
      cur = cur->prev;
    }
    return *this;
  }

  template<class T>
  T& LIter<T>::operator*() const
  {
    return cur->val;
  }

  template<class T>
  T* LIter<T>::operator->() const
  {
    return &(cur->val);
  }

  template<class T>
  List<T>::List(const List<T>& l):
    h(nullptr),
    t(nullptr),
    s(0)
  {
    node<T>* nod = l.h;
    while (nod != nullptr)
    {
      push_back(nod->val);
      nod = nod->next;
    }
  }

  template<class T>
  List<T>& List<T>::operator=(const List<T>& l)
  {
    if (this != &l)
    {
      clear();
      node<T>* now = l.h;
      while (now != nullptr)
      {
        push_back(now->val);
        now = now->next;
      }
    }
    return *this;
  }

  template<class T>
  List<T>::List(List<T>&& l) noexcept:
    h(l.h),
    t(l.t),
    s(l.s)
  {
    l.h = nullptr;
    l.t = nullptr;
    l.s = 0;
  }

  template<class T>
  List<T>& List<T>::operator=(List<T>&& l) noexcept
  {
    if (this != &l)
    {
      clear();
      h = l.h;
      s = l.s;
      t = l.t;
      l.h = nullptr;
      l.t = nullptr;
      l.s = 0;
    }
    return *this;
  }

  template<class T>
  List<T>::List(size_t s, const T& init):
    h(nullptr),
    t(nullptr),
    s(0)
  {
    for (size_t i = 0; i < s; ++i)
    {
      push_back(init);
    }
  }

  template<class T>
  LCIter<T> List<T>::begin() const
  {
    return LCIter<T>(h);
  }

  template<class T>
  LCIter<T> List<T>::end() const
  {
    return LCIter<T>(nullptr);
  }
};

#endif

