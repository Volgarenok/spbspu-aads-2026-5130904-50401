#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>


namespace lukashevich
{
  template< class T >
  class Node
  {
    public:
      Node(T value, Node<T>* n, Node<T>* p):
        val(value),
        next(n),
        prev(p)
      {}

      T val;
      Node< T >* next;
      Node< T >* prev;
  };

  template< class T >
  class List
  {
    public:
      List();
      ~List();
      List(const List< T >& list);
      List< T >& operator=(List< T >& list);
      List(const List< T >&& list);
      List< T >& operator=(List< T >&& list);

      T& front();
      T& back();
      const T& front() const;
      const T& back() const;

      void pushFront(const T& value);
      void pushBack(const T& value);

      void clear();

      size_t size() const;

    private:
      Node< T >* fake_;
      size_t size_;
  };

  template< class T >
  Node< T >* makeFake()
  {
    Node< T >* fake = static_cast< Node< T >* >(::operator new(sizeof(Node< T >)));
    fake->next = nullptr;
    fake->prev = nullptr;
    return fake;
  }

  template< class T >
  void removeFake(Node< T >* fake) noexcept
  {
    ::operator delete(fake);
  }

  template< class T >
  List< T >::List():
    fake_(makeFake< T >()),
    size_(0)
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
    removeFake(fake_);
  }

  template< class T >
  void List< T >::clear()
  {
    Node< T >* cur = fake_->next;
    while (cur != nullptr) {
        Node< T >* next = cur->next;
        delete cur;
        cur = next;
    }
    fake_->next = nullptr;
    fake_->prev = nullptr;
    size_ = 0;
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    Node< T >* node = new Node< T >(value, fake_->next, fake_);

    if (fake_->next != nullptr) {
        fake_->next->prev = node;
    } else
        fake_->prev = node;

    fake_->next = node;
    size_++;
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    Node< T >* node = new Node< T >(value, nullptr, fake_->prev);

    if (fake_->prev != nullptr) {
        fake_->prev->next = node;
    } else
        fake_->next = node;

    fake_->prev = node;
    size_++;
  }

  template< class T >
  const T& List< T >::front() const
  {
    assert(fake_->next != nullptr);
    return fake_->next->val;
  }

  template< class T >
  T& List< T >::front()
  {
    assert(fake_->next != nullptr);
    return fake_->next->val;
  }

  template< class T >
  const T& List< T >::back() const
  {
    assert(fake_->prev != nullptr);
    return fake_->prev->val;
  }

  template< class T >
  T& List< T >::back()
  {
    assert(fake_->prev != nullptr);
    return fake_->prev->val;
  }
  
}

#endif
