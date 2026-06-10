#include <stdexcept>
#include <utility>
#include <memory>

namespace ulanova
{
  namespace detail
  {
    template < class T >
    struct Node
    {
      T data;
      Node< T >* next;
    };
  }
  template < class T >
  class List;

  template< class T >
  class LIter
  {
  public:
    T& operator*() noexcept;
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    bool operator!=(const LIter& other) const noexcept;
    bool operator==(const LIter& other) const noexcept;
    T* operator->() noexcept;
  private:
    detail::Node< T >* node_;
    detail::Node< T >* head_;
    LIter(detail::Node< T >* n = nullptr, detail::Node< T >* h = nullptr) noexcept;
    friend class List< T >;
  };

  template< class T >
  class LCIter
  {
  public:
    const T& operator*() const noexcept;
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    bool operator!=(const LCIter& other) const noexcept;
    bool operator==(const LCIter& other) const noexcept;
    const T* operator->() const noexcept;
  private:
    const detail::Node< T >* node_;
    const detail::Node< T >* head_;
    LCIter(const detail::Node< T >* n = nullptr, const detail::Node< T >* h = nullptr) noexcept;
    friend class List< T >;
  };

  template < class T >
  class List
  {
  public:
    List() noexcept;
    List(const List& other);
    List(List&& other) noexcept;
    ~List() noexcept;

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    void push_front(const T& value);
    void push_front(T&& value);

    void push_back(const T& value);
    void push_back(T&& value);

    void pop_front() noexcept;

    LIter< T > insert_after(LIter< T > pos, const T& value);
    LIter< T > insert_after(LIter< T > pos, T&& value);

    void erase_after(LIter< T > pos) noexcept;

    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;

    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;

    void clear() noexcept;
    void swap(List< T >& other) noexcept;

    T& front();

    void splice_after(LIter< T > pos, List< T >& other) noexcept;
    void splice_after(LIter< T > pos, List< T >& other, LIter< T > before) noexcept;
  private:
    detail::Node< T >* head_;
  };

  template < class T >
  T& LIter< T>::operator*() noexcept
  {
    return node_->data;
  }

  template < class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    if (!node_)
    {
      return *this;
    }
    node_ = node_->next;
    if (node_ == head_)
    {
      node_ = nullptr;
    }
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T > result(*this);
    ++(*this);
    return result;
  }

  template < class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return node_ != other.node_;
  }

  template < class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return node_ == other.node_;
  }

  template < class T >
  T* LIter< T >::operator->() noexcept
  {
    return std::addressof(node_->data);
  }

  template< class T >
  LIter< T >::LIter(detail::Node< T >* n, detail::Node< T >* h) noexcept:
    node_(n),
    head_(h)
  {}

  template < class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    return node_->data;
  }

  template < class T >
  LCIter< T> & LCIter< T >::operator++() noexcept
  {
    if (!node_)
    {
      return *this;
    }
    node_ = node_->next;
    if (node_ == head_)
    {
      node_ = nullptr;
    }
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T > result(*this);
    ++(*this);
    return result;
  }

  template < class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return node_ != other.node_;
  }

  template < class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return node_ == other.node_;
  }

  template < class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T >* n, const detail::Node< T >* h) noexcept:
    node_(n),
    head_(h)
  {}
  template< class T >
  List< T >::List() noexcept :
    head_(nullptr)
  {}
  template< class T >
  List< T >::List(const List& other):
    head_(nullptr)
  {
    try
    {
      for (LCIter< T > it = other.cbegin(); it != other.cend(); ++it)
      {
        push_back(*it);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    head_(std::exchange(other.head_, nullptr))
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != &other)
    {
      List< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    if (this != &other)
    {
      clear();
      head_ = other.head_;
      other.head_ = nullptr;
    }
    return *this;
  }

  template < class T >
  void List< T >::push_front(const T& value)
  {
    detail::Node< T >* new_node = new detail::Node< T >{value, nullptr};
    if (!head_)
    {
      head_ = new_node;
      head_->next = head_;
      return;
    }
    detail::Node< T >* last = head_;
    while (last->next != head_)
    {
      last = last->next;
    }
    new_node->next = head_;
    last->next = new_node;
    head_ = new_node;
  }

  template< class T >
  void List< T >::push_front(T&& value)
  {
    detail::Node< T >* new_node = new detail::Node< T >{std::forward< T >(value), nullptr};
    if (!head_)
    {
      head_ = new_node;
      head_->next = head_;
      return;
    }
    detail::Node< T >* last = head_;
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
    detail::Node< T >* new_node = new detail::Node< T >{value,nullptr};
    if(!head_)
    {
      head_ = new_node;
      head_->next = head_;
      return;
    }
    detail::Node< T >* last = head_;
    while (last ->next != head_)
    {
      last = last->next;
    }
    last->next = new_node;
    new_node->next = head_;
  }

  template< class T >
  void List< T >::push_back(T&& value)
  {
    detail::Node< T >* new_node = new detail::Node< T >{std::move(value), nullptr};
    if (!head_)
    {
      head_ = new_node;
      head_->next = head_;
      return;
    }
    detail::Node< T >* last = head_;
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
    detail::Node< T >* last = head_;
    while (last->next != head_)
    {
      last = last->next;
    }
    erase_after(LIter< T >(last, head_));
  }

  template < class T >
  T& List< T >::front()
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

  template< class T >
  void ulanova::List< T >::swap(List< T >& other) noexcept
  {
    std::swap(head_, other.head_);
  }

  template < class T >
  LIter< T > List< T >::insert_after(LIter< T > pos, const T& value)
  {
    if (!pos.node_)
    {
      return end();
    }
    detail::Node< T >* new_node = new detail::Node< T >{value, pos.node_->next};
    pos.node_->next = new_node;
    return LIter< T >(new_node, head_);
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > pos, T&& value)
  {
    if (!pos.node_)
    {
      return end();
    }
    detail::Node< T >* new_node = new detail::Node< T >{std::move(value), pos.node_->next};
    pos.node_->next = new_node;
    return LIter< T >(new_node, head_);
  }

  template < class T >
  void List< T >::erase_after(LIter< T > pos) noexcept
  {
    if (!pos.node_)
    {
      return;
    }
    detail::Node< T >* temp = pos.node_->next;
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
      head_ = head_->next;
    }
    pos.node_->next = temp->next;
    delete temp;
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List< T >& other) noexcept
  {
    if (this == std::addressof(other) || !other.head_)
    {
      return;
    }

    if (!head_)
    {
      head_ = std::exchange(other.head_, nullptr);
      return;
    }

    if (!pos.node_)
    {
      return;
    }

    detail::Node< T >* other_last = other.head_;
    while (other_last->next != other.head_)
    {
      other_last = other_last->next;
    }

    other_last->next = pos.node_->next;
    pos.node_->next = other.head_;
    other.head_ = nullptr;
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List< T >& other, LIter< T > before) noexcept
  {
    if (!pos.node_ || !before.node_ || !other.head_)
    {
      return;
    }

    detail::Node< T >* moved = before.node_->next;

    if (moved == other.head_)
    {
      other.head_ = other.head_->next;
    }

    if (moved == before.node_)
    {
      other.head_ = nullptr;
    }
    else
    {
      before.node_->next = moved->next;
    }

    moved->next = pos.node_->next;
    pos.node_->next = moved;
  }
}
