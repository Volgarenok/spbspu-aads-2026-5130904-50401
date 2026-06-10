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
  public:
    LIter(Node< T >* n = nullptr, Node< T >* h = nullptr) : node_(n), head_(h) {}
    T& operator*() noexcept
    {
      return node_ -> data;
    }
    LIter& operator++() noexcept
    {
      if (!node_)
      {
        return *this;
      }
      node_ = node_ -> next;
      if (node_ == head_)
      {
        node_ = nullptr;
      }
      return *this;
    }
    bool operator!=(const LIter& other) const noexcept
    {
      return node_ != other.node;
    }
    bool operator==(const LIter& other) const noexcept
    {
      return node_ == other.node;
    }
    T* operator->() noexcept
    {
      return &(node_->data);
    }
  private:
    Node< T >* node_;
    Node< T >* head_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
  public:
    LCIter(Node< T >* n = nullptr, Node< T >* h = nullptr) : node_(n), head_(h) {}
    const T& operator*() const noexcept
    {
      return node_ -> data;
    }
    LCIter& operator++() noexcept
    {
      if (!node_)
      {
        return *this;
      }
      node_ = node_ -> next;
      if (node_ == head_)
      {
        node_ = nullptr;
      }
      return *this;
    }
    bool operator!=(const LCIter& other) const noexcept
    {
      return node_ != other.node;
    }
    bool operator==(const LCIter& other) const noexcept
    {
      return node_ == other.node;
    }
    const T* operator->() const noexcept
    {
      return &(node_->data);
    }
  private:
    Node< T >* node_;
    Node< T >* head_;
  };

  template < class T >
  class List
  {
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
  private:
    Node< T >* head_;
  };

  template< class T >
  List< T >::List() : head_(nullptr) {}
  template< class T >
  List< T >::List(const List& other) : head_(nullptr)
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
    if (!head_)
    {
      head_ = new_node;
      head_ -> next = head_;
      return;
    }
    Node< T >* last = head_;
    while (last -> next != head_)
    {
      last = last -> next;
    }
    new_node -> next = head_;
    last -> next = new_node;
    head_ = new_node;
  }
  template< class T >
  void List< T >::push_front(T&& value)
  {
    Node< T >* new_node = new Node< T >{std::move(value), nullptr};
    if (!head_)
    {
      head_ = new_node;
      head_->next = head_;
      return;
    }
    Node< T >* last = head_;
    while (last->next != head_)
    {
      last = last->next;
    }
    new_node->next = head_;
    last->next = new_node;
    head_ = new_node;
  }
  template < class T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* new_node = new Node< T >{value,nullptr};
    if(!head_)
    {
      head_ = new_node;
      head_->next = head_;
      return;
    }
    Node< T >* last = head_;
    while (last ->next != head_)
    {
      last = last -> next;
    }
    last -> next = new_node;
    new_node -> next = head_;
  }
  template< class T >
  void List< T >::push_back(T&& value)
  {
    Node< T >* new_node = new Node< T >{std::move(value), nullptr};
    if (!head_)
    {
      head_ = new_node;
      head_->next = head_;
      return;
    }
    Node< T >* last = head_;
    while (last->next != head_)
    {
      last = last->next;
    }
    last->next = new_node;
    new_node->next = head_;
  }
  template < class T >
  void List< T >::pop_front() noexcept
  {
    if (!head_)
    {
      return;
    }
    if (head_ -> next == head_)
    {
      delete head_;
      head_ = nullptr;
      return;
    }
    Node< T >* last = head_;
    while (last -> next != head_)
    {
      last = last -> next;
    }
    Node< T >* temp = head_;
    head_ = head_ -> next;
    last -> next = head_;
    delete temp;
  }

  template < class T >
  T& List< T >::front()
  {
    if (!head_)
    {
      throw std::logic_error("empty list");
    }
    return head_ -> data;
  }

  template< class T >
  const T& List< T >::front() const
  {
    if (!head_)
    {
      throw std::logic_error("empty list");
    }
    return head_->data;
  }

  template < class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head_,head_);
  }
  template < class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr,head_);
  }

  template < class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(head_,head_);
  }
  template < class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(nullptr,head_);
  }

  template < class T >
  void List< T >::clear() noexcept
  {
    while (head_)
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
    return LIter< T >(new_node, head_);
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
    if (temp == head_)
    {
      if (head_->next == head_)
      {
        delete head_;
        head_ = nullptr;
        return;
      }
      head_ = head_ -> next;
    }
    pos.node -> next = temp -> next;
    delete temp;
  }
  template < class T >
  bool List< T >::empty() const noexcept
  {
    return head_ == nullptr;
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
