#include "STRUCTS.hpp"

namespace petrov
{
  template< class T >
  LIter< T >::LIter():
    nd{nullptr}
  {}

  template< class T >
  LIter< T >::LIter(Node< T >* n):
    nd{n}
  {}

  template< class T >
  bool LIter< T >::hasNext() const
  {
    return nd != nullptr;
  }

  template< class T >
  LIter< T > LIter< T >::next() const
  {
    if(nd)
    {
      return nd->next;
    }
    else
    {
      throw;
    }
  }

  template< class T >
  T& LIter< T >::operator*() const
  {
    if (nd)
    {
      return nd->val;
    }
    else
    {
      throw;
    }
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& it) const
  {
    return nd == it.nd;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& it) const
  {
    return !(this == it);
  }

  template< class T >
  LCIter< T >::LCIter():
    nd{nullptr}
  {}

  template< class T >
  LCIter< T >::LCIter(const Node< T >* n):
    nd{n}
  {}

  template< class T >
  bool LCIter< T >::hasNext() const{
    return nd != nullptr;
  }

  template< class T >
  LCIter< T > LCIter< T >::next() const
  {
    if(nd)
    {
      return nd->next;
    }
    else
    {
      throw;
    }
  }

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    if(nd)
    {
      return nd->val;
    }
    else
    {
      throw;
    }
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& it) const
  {
    return nd == it.nd;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& it) const
  {
    return !(this == it);
  }

  template< class T >
  Node< T >* List< T >::mkFake()
  {
    Node< T >* f = new Node< T >;
    return f;
  }

  template< class T >
  void List< T >::delFake()
  {
    delete fake;
  }

  template< class T >
  List< T >::List():
    fake{mkFake()}
  {}

  template< class T >
  List< T >::List(const List< T >& l):
    fake{mkFake()}
  {
    Node< T >* nl = l.fake->next;
    Node< T >* n = fake;
    while(nl)
    {
      n->next = new Node< T >{nl->val, nullptr};
      n = n->next;
      nl = nl->next;
    }
  }

  template< class T >
  List< T >::List(List< T >&& l):
    fake{l.fake}
  {
    fake->next = l.fake->next;
    l.fake->next = nullptr;
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& l)
  {
    if (this != &l)
    {
      clear();
      fake->next = nullptr;
      if (l.fake->next)
      {
        Node< T >* nl = l.fake->next;
        Node< T >* n = fake;
        while(nl)
        {
          n->next = new Node< T >{nl->val, nullptr};
          n = n->next;
          nl = nl->next;
        }
      }
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& l)
  {
    if (this != &l)
    {
      clear();
      delFake();
      fake = l.fake->next;
      l.fake->next = nullptr;
    }
    return *this;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
    delFake();
  }

  template< class T >
  void List< T >::clear()
  {
    while(fake->next)
    {
      Node< T >* tmp = fake->next;
      fake->next = tmp->next;
      delete tmp;
    }
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return fake->next;
  }

  template< class T >
  LCIter< T > List< T >::begin() const
  {
    return fake->next;
  }

  template< class T >
  LIter< T > List< T >::end()
  {
    return nullptr;
  }

  template< class T >
  LCIter< T > List< T >::end() const
  {
    return nullptr;
  }

  template< class T >
  LIter< T > List< T >::getLast()
  {
    LIter< T > it = begin();
    while (it.hasNext())
    {
      it = it.next();
    }
    return it;
  }

  template< class T >
  LCIter< T > List< T >::getLast() const
  {
    LCIter< T > it = begin();
    while (it.hasNext())
    {
      it = it.next();
    }
    return it;
  }

  template< class T >
  LIter< T > List< T >::addStart(const T& a)
  {
    Node< T >* n = new Node< T >{a, fake->next};
    fake->next = n;
    return n;
  }

  template< class T >
  void List< T >::popStart()
  {
    if (fake->next)
    {
      Node< T >* tmp = fake->next->next;
      delete fake->next;
      fake->next = tmp;
    }
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > id, const T& a)
  {
    if(id.hasNext())
    {
      Node< T >* n = new Node< T > {a, id.next().nd};
      id.nd->next = n;
      id = id.nd->next;
      return LIter< T >(id);
    }
    else
    {
      throw;
    }
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > id, const T&& a)
  {
    if(id.hasNext())
    {
      Node< T >* n = new Node< T > {a, id.next().nd};
      id.nd->next = n;
      id = id.nd->next;
      return LIter< T >(id);
    }
    else
    {
      throw;
    }
  }
}
