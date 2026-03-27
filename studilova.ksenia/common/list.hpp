#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include<stdexcept>

namespace studilova
{
  template< class T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;
  };

  template< class T >
  class LIter;

  template< class T >
  class CLIter;

  template< class T >
  class List
  {
    private:
      Node< T >* head_;
      size_t size_;

    public:
      List();
      ~List();
      List(const List& other);
      List& operator=(const List& other);

      bool empty() const;
      void clear();
      size_t size() const;

      T& front();
      const T& front() const;
      T& back();
      const T& back() const;

      void popFront();
      void popBack();
      void pushFront(const T& value);
      void pushBack(const T& value);
      void insert(LIter< T > pos, const T& value);

      LIter< T > begin();
      LIter< T > end();

      CLIter< T > begin() const;
      CLIter< T > end() const;
  };

  template< class T >
  List< T >::List()
    : head_(nullptr), size_(0)
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >::List(const List& other)
    : head_(nullptr), size_(0)
  {
    if (other.empty())
    {
      return;
    }
    Node< T >*curr = other.head_;

    for (size_t i = 0; i < other.size_; ++i)
    {
      pushBack(curr->data);
      curr = curr->next;
    }
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this == &other)
    {
      return *this;
    }
    clear();
    if (other.empty())
    {
      return *this;
    }

    Node< T >* curr = other.head_;
    for (size_t i = 0; i < other.size_; ++i)
    {
      pushBack(curr->data);
      curr = curr->next;
    }
    return *this;
  }

  template< class T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template< class T >
  void List< T >::clear()
  {
    while(!empty())
    {
      popFront();
    }
  }

  template< class T >
  size_t List< T >::size()const
  {
    return size_;
  }

  template< class T >
  T& List< T >::front()
  {
    if(empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->data;
  }

  template< class T >
  const T& List< T >::front() const
  {
    if(empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->data;
  }

  template< class T >
  T& List< T >::back()
  {
    if(empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->prev->data;
  }

  template< class T >
  const T& List< T >::back() const
  {
    if(empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->prev->data;
  }

  template< class T >
  void List< T >::popFront()
  {
    if (empty())
    {
      return;
    }

    Node< T >* temp = head_;

    if (size_ == 1)
    {
      delete temp;
      head_ = nullptr;
      size_ = 0;
    } else {
      Node< T >* tail = head_->prev;

      head_ = head_->next;

      head_->prev = tail;
      tail->next = head_;

      delete temp;
      size_--;
    }
  }

  template< class T >
  void List< T >::popBack()
  {
    if (empty())
    {
      return;
    }

    if (size_ == 1)
    {
      delete head_;
      head_ = nullptr;
      size_ = 0;
    } else {
      Node< T >* tail = head_->prev;
      Node< T >* new_tail = tail->prev;

      new_tail->next = head_;
      head_->prev = new_tail;

      delete tail;
      size_--;
    }
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    Node< T >* node = new Node< T >{value, nullptr, nullptr};

    if(empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;

      head_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    Node< T >* node = new Node< T >{value, nullptr, nullptr};

    if(empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;
    }
    size_++;
  }

  template< class T >
  void List< T >::insert(LIter< T > pos, const T& value)
  {
    if (pos.node_ == nullptr)
    {
      pushBack(value);
      return;
    }
    if (pos.node_ == head_)
    {
      pushFront(value);
      return;
    }

    Node< T >* curr = pos.node_;
    Node< T >* prev = curr->prev;

    Node< T >* node = new Node< T >{value, curr, prev};

    prev->next = node;
    curr->prev = node;

    ++size_;
  }

  template< class T >
  class LIter
  {
    friend class List< T >;

  private:
    Node< T >* node_;

  public:
    LIter(Node< T >* node = nullptr);

    T& operator*() const;

    LIter& operator++();
    LIter& operator--();

    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;
  };

  template< class T >
  LIter< T >::LIter(Node< T >* node)
    : node_(node)
  {}

  template< class T >
  T& LIter< T >::operator*() const
  {
    if (node_ == nullptr)
    {
      throw std::out_of_range("Iterator dereference error");
    }
    return node_->data;
  }

  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LIter< T >& LIter< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const
  {
    return node_ == other.node_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const
  {
    return !(*this == other);
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  class CLIter
  {
    friend class List< T >;

  private:
    Node< T >* node_;

  public:
    CLIter(Node< T >* node = nullptr);

    const T& operator*() const;

    CLIter& operator++();
    CLIter& operator--();

    bool operator==(const CLIter& other) const;
    bool operator!=(const CLIter& other) const;
  };

  template< class T >
  CLIter< T >::CLIter(Node< T >* node)
    : node_(node)
  {}

  template< class T >
  const T& CLIter< T >::operator*() const
  {
    if (node_ == nullptr)
    {
      throw std::out_of_range("Iterator dereference error");
    }
    return node_->data;
  }

  template< class T >
  CLIter< T >& CLIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  CLIter< T >& CLIter< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  bool CLIter< T >::operator==(const CLIter& other) const
  {
    return node_ == other.node_;
  }

  template< class T >
  bool CLIter< T >::operator!=(const CLIter& other) const
  {
    return !(*this == other);
  }

  template< class T >
  CLIter< T > List< T >::begin() const
  {
    return CLIter< T >(head_);
  }

  template< class T >
  CLIter< T > List< T >::end() const
  {
    return CLIter< T >(nullptr);
  }
}

#endif
