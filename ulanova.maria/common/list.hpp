#ifndef LIST_HPP
#define LIST_HPP
#include <stdexcept>
#include <utility>

namespace ulanova
{
  template < class T >
  struct Node
  {
    T data;
    Node< T >* next;
  };
  template < class T >
  class List;

  template< class T >
  class LIter
  {
    friend class List< T >;
  private:
    Node< T >* node;
    Node< T >* head;
  public:
    LIter(Node< T >* n = nullptr, Node< T >* h = nullptr) : node(n), head(h) {}
    T& operator*() noexcept
    {
      return node -> data;
    }
    LIter& operator++() noexcept
    {
      if (!node)
      {
        return *this;
      }
      node = node -> next;
      if (node == head)
      {
        node = nullptr;
      }
      return *this;
    }
    bool operator!=(const LIter& other) const noexcept
    {
      return node != other.node;
    }
    bool operator==(const LIter& other) const noexcept
    {
      return node == other.node;
    }
    T* operator->() noexcept
    {
      return &(node->data);
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
  private:
    Node< T >* node;
    Node< T >* head;
  public:
    LCIter(Node< T >* n = nullptr, Node< T >* h = nullptr) : node(n), head(h) {}
    const T& operator*() const noexcept
    {
      return node -> data;
    }
    LCIter& operator++() noexcept
    {
      if (!node)
      {
        return *this;
      }
      node = node -> next;
      if (node == head)
      {
        node = nullptr;
      }
      return *this;
    }
    bool operator!=(const LCIter& other) const noexcept
    {
      return node != other.node;
    }
    bool operator==(const LCIter& other) const noexcept
    {
      return node == other.node;
    }
    const T* operator->() const noexcept
    {
      return &(node->data);
    }
  };

  template < class T >
  class List
  {
  private:
    Node< T >* head;
  public:
    List();
    ~List() noexcept;
    List(const List& other);

    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front() noexcept;

    LIter< T > insert_after(LIter< T > pos, const T& value) noexcept;
    void erase_after(LIter< T > pos) noexcept;

    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;

    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;

    void clear() noexcept;

    T& front();
    const T& front() const;
    bool empty() const noexcept;

    template< class... Args >
    void emplace_front(Args&&... args);

    template< class... Args >
    void emplace_back(Args&&... args);
  };

  template< class T >
  List< T >::List() : head(nullptr) {}
  template< class T >
  List< T >::List(const List& other) : head(nullptr)
  {
    for (LCIter< T > it = other.cbegin(); it != other.cend(); ++it)
    {
      push_back(*it);
    }
  }


  template < class T >
  void List< T >::push_front(const T& value)
  {
    Node< T >* new_node = new Node< T >{value,nullptr};
    if (!head)
    {
      head = new_node;
      head -> next = head;
      return;
    }
    Node< T >* last = head;
    while (last -> next != head)
    {
      last = last -> next;
    }
    new_node -> next = head;
    last -> next = new_node;
    head = new_node;
  }
  template< class T >
  void List< T >::push_front(T&& value)
  {
    Node< T >* new_node = new Node< T >{std::move(value), nullptr};
    if (!head)
    {
      head = new_node;
      head->next = head;
      return;
    }
    Node< T >* last = head;
    while (last->next != head)
    {
      last = last->next;
    }
    new_node->next = head;
    last->next = new_node;
    head = new_node;
  }
  template < class T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* new_node = new Node< T >{value,nullptr};
    if(!head)
    {
      head = new_node;
      head->next = head;
      return;
    }
    Node< T >* last = head;
    while (last ->next != head)
    {
      last = last -> next;
    }
    last -> next = new_node;
    new_node -> next = head;
  }
  template< class T >
  void List< T >::push_back(T&& value)
  {
    Node< T >* new_node = new Node< T >{std::move(value), nullptr};
    if (!head)
    {
      head = new_node;
      head->next = head;
      return;
    }
    Node< T >* last = head;
    while (last->next != head)
    {
      last = last->next;
    }
    last->next = new_node;
    new_node->next = head;
  }
  template < class T >
  void List< T >::pop_front() noexcept
  {
    if (!head)
    {
      return;
    }
    if (head -> next == head)
    {
      delete head;
      head = nullptr;
      return;
    }
    Node< T >* last = head;
    while (last -> next != head)
    {
      last = last -> next;
    }
    Node< T >* temp = head;
    head = head -> next;
    last -> next = head;
    delete temp;
  }

  template < class T >
  T& List< T >::front()
  {
    if (!head)
    {
      throw std::logic_error("empty list");
    }
    return head -> data;
  }

  template< class T >
  const T& List< T >::front() const
  {
    if (!head)
    {
      throw std::logic_error("empty list");
    }
    return head->data;
  }

  template < class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head,head);
  }
  template < class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr,head);
  }

  template < class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(head,head);
  }
  template < class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(nullptr,head);
  }

  template < class T >
  void List< T >::clear() noexcept
  {
    while (head)
    {
      pop_front();
    }
  }
  template < class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template < class T >
  LIter< T > List< T >::insert_after(LIter< T > pos, const T& value) noexcept
  {
    if (!pos.node)
    {
      return end();
    }
    Node< T >* new_node = new Node< T >{value, pos.node -> next};
    pos.node -> next = new_node;
    return LIter< T >(new_node, head);
  }
  template < class T >
  void List< T >::erase_after(LIter< T > pos) noexcept
  {
    if (!pos.node)
    {
      return;
    }
    Node< T >* temp = pos.node -> next;
    if (!temp)
    {
      return;
    }
    if (temp == head)
    {
      if (head->next == head)
      {
        delete head;
        head = nullptr;
        return;
      }
      head = head -> next;
    }
    pos.node -> next = temp -> next;
    delete temp;
  }
  template < class T >
  bool List< T >::empty() const noexcept
  {
    return head == nullptr;
  }

  template< class T >
  template< class... Args >
  void List< T >::emplace_front(Args&&... args)
  {
    Node< T >* node = new Node< T >{ T(std::forward< Args >(args)...), head_ };
    head_ = node;
    if (tail_ == nullptr)
    {
      tail_ = head_;
    }
    ++size_;
  }

  template< class T >
  template< class... Args >
  void List< T >::emplace_back(Args&&... args)
  {
    Node< T >* node = new Node< T >{ T(std::forward< Args >(args)...), nullptr };
    if (tail_ == nullptr)
    {
      head_ = node;
      tail_ = node;
    }
    else
    {
      tail_->next = node;
      tail_ = node;
    }
    ++size_;
  }
}
#endif
