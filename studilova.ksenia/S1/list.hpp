#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace studilova
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T data;
      Node* next;
      Node* prev;
    };
  }

  template< class T >
  class LIter;

  template< class T >
  class CLIter;

  template< class T >
  class List
  {
    public:
      List();
      List(const List& other);
      List(List&& other) noexcept;
      ~List();

      List& operator=(const List& other);
      List& operator=(List&& other) noexcept;

      void swap(List& other) noexcept;

      bool empty() const noexcept;
      void clear() noexcept;
      size_t size() const noexcept;

      T& front();
      const T& front() const;
      T& back();
      const T& back() const;

      void popFront() noexcept;
      void popBack() noexcept;

      void pushFront(const T& value);
      void pushFront(T&& value);
      void pushBack(const T& value);
      void pushBack(T&& value);
      void insert(LIter< T > pos, const T& value);
      void insert(LIter< T > pos, T&& value);

      LIter< T > begin() noexcept;
      LIter< T > end() noexcept;

      CLIter< T > begin() const noexcept;
      CLIter< T > end() const noexcept;

    private:
      detail::Node< T >* head_;
      size_t size_;
  };

  template< class T >
  List< T >::List() :
    head_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List& other) :
    head_(nullptr),
    size_(0)
  {
    try
    {
      detail::NodeБ T >* curr = other.head_;

      for (size_t i = 0; i < other.size_; ++i)
      {
        pushBack(curr->data);
        curr = curr->next;
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::List(List&& other) noexcept :
    head_(other.head_),
    size_(other.size_)
  {
    other.head_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other)
    {
      List< T > temp(other);
      swap(temp);
    }

    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      clear();

      head_ = other.head_;
      size_ = other.size_;

      other.head_ = nullptr;
      other.size_ = 0;
    }

    return *this;
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      popFront();
    }
  }

  template< class T >
  size_t List< T >::size() const noexcept
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
  void List< T >::popFront() noexcept
  {
    if (empty())
    {
      return;
    }

    detail::Node< T >* temp = head_;

    if (size_ == 1)
    {
      delete temp;
      head_ = nullptr;
      size_ = 0;
    } else {
      detail::Node< T >* tail = head_->prev;

      head_ = head_->next;

      head_->prev = tail;
      tail->next = head_;

      delete temp;
      size_--;
    }
  }

  template< class T >
  void List< T >::popBack() noexcept
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
      detail::Node< T >* tail = head_->prev;
      detail::Node< T >* new_tail = tail->prev;

      new_tail->next = head_;
      head_->prev = new_tail;

      delete tail;
      size_--;
    }
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    detail::Node< T >* node = new detail::Node< T >{value, nullptr, nullptr};

    if(empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;

      head_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pushFront(T&& value)
  {
    detail::Node< T >* node = new detail::Node< T >{ std::move(value), nullptr, nullptr };

    if (empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

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
    detail::Node< T >* node = new detail::Node< T >{value, nullptr, nullptr};

    if(empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;
    }
    size_++;
  }

  template< class T >
  void List< T >::pushBack(T&& value)
  {
    detail::Node< T >* node = new detail::Node< T >{ std::move(value), nullptr, nullptr };

    if (empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;
    }
    ++size_;
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

    detail::Node< T >* curr = pos.node_;
    detail::Node< T >* prev = curr->prev;

    detail::Node< T >* node = new detail::Node< T >{value, curr, prev};

    prev->next = node;
    curr->prev = node;

    ++size_;
  }

  template< class T >
  void List< T >::insert(LIter< T > pos, T&& value)
  {
    if (!pos.node_)
    {
      pushBack(std::move(value));
      return;
    }
    if (pos.node_ == head_)
    {
      pushFront(std::move(value));
      return;
    }

    detail::Node< T >* curr = pos.node_;
    detail::Node< T >* prev = curr->prev;
    detail::Node< T >* node = new detail::Node< T >{ std::move(value), curr, prev };

    prev->next = node;
    curr->prev = node;
    ++size_;
  }

  template< class T >
  class LIter
  {
    public:
      T& operator*() const;

      LIter& operator++();
      LIter& operator--();

      bool operator==(const LIter& other) const;
      bool operator!=(const LIter& other) const;

    private:
      detail::Node< T >* node_;

      explicit LIter(detail::Node< T >* node = nullptr);

      friend class List< T >;
  };

  template< class T >
  LIter< T >::LIter(detail::Node< T >* node) :
    node_(node)
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
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  class CLIter
  {
    public:
      const T& operator*() const;

      CLIter& operator++();
      CLIter& operator--();

      bool operator==(const CLIter& other) const;
      bool operator!=(const CLIter& other) const;

    private:
      detail::Node< T >* node_;

      explicit CLIter(detail::Node< T >* node = nullptr);

      friend class List< T >;
  };

  template< class T >
  CLIter< T >::CLIter(detail::Node< T >* node) :
    node_(node)
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
  CLIter< T > List< T >::begin() const noexcept
  {
    return CLIter< T >(head_);
  }

  template< class T >
  CLIter< T > List< T >::end() const noexcept
  {
    return CLIter< T >(nullptr);
  }
}

#endif
