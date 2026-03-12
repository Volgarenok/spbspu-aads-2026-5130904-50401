#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>
#include "node.hpp"
#include "iterator.hpp"

namespace zhuravleva
{

  template<class T>
  class List
  {
  private:

    Node< T >* fake;

    Node< T >* createFake();

  public:

    List();
    ~List();

    List(const List& other);
    List(List&& other);

    List& operator=(const List& other);
    List& operator=(List&& other);

    LIter< T > begin() noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LCIter< T > cend() const noexcept;

    LIter< T > AddStart(const T& value);

    LIter< T > beforeStart();

    bool empty() const noexcept;

    void deleteStart() noexcept;
    void deleteEnd() noexcept;
    void deleteAfter(LIter< T > pos);

    LIter< T > addAfter(LIter< T > pos, const T& value);

    void clear() noexcept;
  };


  template< class T >
  bool List< T >::empty() const noexcept
  {
    return fake -> next == fake;
  }


  template<class T>
  Node<T>* List<T>::createFake()
  {
    Node<T>* node = new Node<T>(T());
    node->next = node;
    return node;
  }


  template<class T>
  List<T>::List()
  {
    fake = createFake();
  }


  template<class T>
  List<T>::~List()
  {
    clear();
    delete fake;
  }


  template<class T>
  List<T>::List(const List& other)
  {
    fake = createFake();

    Node<T>* cur = other.fake->next;
    Node<T>* last = fake;

    while(cur != other.fake)
    {
      Node<T>* node = new Node<T>(cur->data, fake);
      last->next = node;
      last = node;
      cur = cur->next;
    }
  }


  template<class T>
  List<T>::List(List&& other)
  {
    fake = other.fake;
    other.fake = createFake();
  }


  template<class T>
  List<T>& List<T>::operator=(const List& other)
  {
    if(this != &other)
    {
      clear();

      Node<T>* cur = other.fake->next;
      Node<T>* last = fake;

      while(cur != other.fake)
      {
        Node<T>* node = new Node<T>(cur->data, fake);
        last->next = node;
        last = node;
        cur = cur->next;
      }
    }
    return *this;
  }


  template<class T>
  List<T>& List<T>::operator=(List&& other)
  {
    if(this != &other)
    {
      clear();
      delete fake;

      fake = other.fake;
      other.fake = createFake();
    }

    return *this;
  }


  template<class T>
  LIter<T> List<T>::begin() noexcept
  {
    if(fake->next == fake)
    {
      return LIter<T>(nullptr);
    }
    else
    {
      return LIter<T>(fake->next);
    }
  }


  template<class T>
  LCIter<T> List<T>::cbegin() const noexcept
  {
    if(fake->next == fake)
    {
      return LCIter<T>(nullptr);
    }
    else
    {
      return LCIter<T>(fake->next);
    }
  }


  template<class T>
  LIter<T> List<T>::end() noexcept
  {
    return LIter<T>(nullptr);
  }

  template<class T>
  LCIter<T> List<T>::cend() const noexcept
  {
    return LCIter<T>(nullptr);
  }


  template<class T>
  LIter<T> List<T>::AddStart(const T& value)
  {
    Node<T>* node = new Node<T>(value, fake->next);
    fake->next = node;
    return LIter<T>(node);
  }


  template<class T>
  void List<T>::deleteStart() noexcept
  {
    if(fake->next != fake)
    {
      Node<T>* tmp = fake->next;
      fake->next = tmp->next;
      delete tmp;
    }
  }

  template< class T >
  void List<T>::deleteAfter(LIter< T > pos)
  {
    if(pos.current && pos.current -> next != fake)
    {
      Node< T >* tmp = pos.current -> next;
      pos.current -> next = tmp -> next;
      delete tmp;
    }
  }

  template<class T>
  void List<T>::deleteEnd() noexcept
  {
    if(fake->next != fake)
    {
      Node<T>* prev = fake;
      Node<T>* cur = fake->next;

      while(cur->next != fake)
      {
        prev = cur;
        cur = cur->next;
      }

      prev->next = fake;
      delete cur;
    }
  }


  template<class T>
  LIter<T> List<T>::addAfter(LIter<T> pos, const T& value)
  {
    if(!pos.current)
    {
      throw std::runtime_error("invalid iterator");
    }

    Node<T>* node = new Node<T>(value, pos.current->next);
    pos.current->next = node;

    return LIter<T>(node);
  }

  template<class T>
  LIter<T> List<T>::beforeStart()
  {
    return LIter< T >(fake);
  }

  template<class T>
  void List<T>::clear() noexcept
  {
    Node<T>* cur = fake->next;

    while(cur != fake)
    {
      Node<T>* tmp = cur;
      cur = cur->next;
      delete tmp;
    }

    fake->next = fake;
  }


}

#endif
