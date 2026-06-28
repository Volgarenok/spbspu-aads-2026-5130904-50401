#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <memory>

namespace levkin {
  template < class T >
  class List;

  template < class T >
  class LIter;

  template < class T >
  class LCIter;

  namespace detail {
    struct NodeBase
    {
      NodeBase* prev;
      NodeBase* next;

      virtual ~NodeBase() = default;
    };

    template < class T >
    struct Node: public NodeBase
    {
      T val;
      ~Node() override = default;
    };
  }

  template < class T >
  class List
  {
    friend class LIter< T >;
    friend class LCIter< T >;

  public:
    List() noexcept;
    explicit List(const T& val);
    List(const List< T >& a);
    List(List< T >&& a) noexcept;
    ~List() = default;

    List< T >& operator=(List< T > a) noexcept;

    void swap(List< T >& a) noexcept;

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;

    void pushFront(const T& val);
    void pushBack(const T& val);
    void popFront() noexcept;
    void popBack() noexcept;

    LIter< T > insertAfter(LIter< T > it, const T& val);
    LIter< T > erase(LIter< T > pos) noexcept;
    void erase(LIter< T > from, LIter< T > to) noexcept;
    void clearAndInit(size_t size, const T& val);
    void clear() noexcept;
    size_t size() const noexcept;

  private:
    detail::NodeBase* pseudo_;

    explicit List(detail::NodeBase* pseudoNode) noexcept;

    static detail::Node< T >* castNode(detail::NodeBase* node) noexcept
    {
      return dynamic_cast< detail::Node< T >* >(node);
    }

    static const detail::Node< T >* castNode(const detail::NodeBase* node) noexcept
    {
      return dynamic_cast< const detail::Node< T >* >(node);
    }

    LIter< T > eraseFast(LIter< T > pos) noexcept;
  };

  template < class T >
  List< T >::List() noexcept:
    pseudo_(new detail::NodeBase())
  {
    pseudo_->next = pseudo_;
    pseudo_->prev = pseudo_;
  }

  template < class T >
  List< T >::List(const T& val):
    List()
  {
    pushBack(val);
  }

  template < class T >
  List< T >::List(const List< T >& a):
    List()
  {
    try {
      for (LCIter< T > i = a.cbegin(); i != a.cend(); ++i) {
        pushBack(*i);
      }
    } catch (...) {
      clear();
      delete pseudo_;
      throw;
    }
  }

  template < class T >
  List< T >::List(List< T >&& a) noexcept:
    pseudo_(std::exchange(a.pseudo_, nullptr))
  {}

  template < class T >
  List< T >& List< T >::operator=(List< T > a) noexcept
  {
    swap(a);
    return *this;
  }

  template < class T >
  void List< T >::swap(List< T >& a) noexcept
  {
    std::swap(a.pseudo_, this->pseudo_);
  }

  template < class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(pseudo_->next);
  }

  template < class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return cbegin();
  }

  template < class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(pseudo_->next);
  }

  template < class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(pseudo_);
  }

  template < class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return cend();
  }

  template < class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(pseudo_);
  }

  template < class T >
  void List< T >::pushFront(const T& val)
  {
    insertAfter(LIter< T >(pseudo_), val);
  }

  template < class T >
  void List< T >::pushBack(const T& val)
  {
    insertAfter(LIter< T >(pseudo_->prev), val);
  }

  template < class T >
  void List< T >::popFront() noexcept
  {
    if (begin() != end()) {
      erase(begin());
    }
  }

  template < class T >
  void List< T >::popBack() noexcept
  {
    if (begin() != end()) {
      erase(LIter< T >(pseudo_->prev));
    }
  }

  template < class T >
  LIter< T > List< T >::insertAfter(LIter< T > it, const T& val)
  {
    if (it.curr_ == nullptr) {
      throw std::out_of_range("out of bounds or null");
    }
    detail::Node< T >* newNode = new detail::Node< T >();
    newNode->val = val;
    newNode->prev = it.curr_;
    newNode->next = it.curr_->next;

    newNode->next->prev = newNode;
    it.curr_->next = newNode;
    return LIter< T >(newNode);
  }

  template < class T >
  LIter< T > List< T >::erase(LIter< T > pos) noexcept
  {
    if (pos == end()) {
      return pos;
    }
    return eraseFast(pos);
  }

  template < class T >
  void List< T >::erase(LIter< T > from, LIter< T > to) noexcept
  {
    while (from != to) {
      erase(from++);
    }
  }

  template < class T >
  void List< T >::clearAndInit(size_t size, const T& val)
  {
    clear();
    for (size_t i = 0; i < size; ++i) {
      this->pushBack(val);
    }
  }

  template < class T >
  void List< T >::clear() noexcept
  {
    if (pseudo_ == nullptr) {
      return;
    }
    while (pseudo_->next != pseudo_) {
      eraseFast(LIter< T >(pseudo_->next));
    }
  }

  template < class T >
  size_t List< T >::size() const noexcept
  {
    auto it = cbegin();
    size_t count = 0;
    for (; it != cend(); ++it) {
      count++;
    }
    return count;
  }

  template < class T >
  List< T >::List(detail::NodeBase* pseudoNode) noexcept:
    pseudo_(pseudoNode)
  {}

  template < class T >
  LIter< T > List< T >::eraseFast(LIter< T > pos) noexcept
  {
    detail::NodeBase* toDelete = pos.curr_;
    toDelete->prev->next = toDelete->next;
    toDelete->next->prev = toDelete->prev;

    detail::NodeBase* nxt = toDelete->next;
    delete castNode(toDelete);
    return LIter< T >(nxt);
  }

  template < class T >
  class LCIter
  {
    friend class List< T >;
    friend class LIter< T >;

  public:
    LCIter() noexcept:
      curr_(nullptr)
    {}

    LCIter(LIter< T > it) noexcept:
      curr_(it.curr_)
    {}

    const T& operator*() const noexcept
    {
      return List< T >::castNode(curr_)->val;
    }

    LCIter& operator++() noexcept
    {
      curr_ = curr_->next;
      return *this;
    }

    LCIter operator++(int) noexcept
    {
      LCIter temp = *this;
      ++(*this);
      return temp;
    }

    LCIter& operator--() noexcept
    {
      curr_ = curr_->prev;
      return *this;
    }

    LCIter operator--(int) noexcept
    {
      LCIter temp = *this;
      --(*this);
      return temp;
    }

    const T* operator->() const noexcept
    {
      return std::addressof(List< T >::castNode(curr_)->val);
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return curr_ == other.curr_;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return !(*this == other);
    }

    bool operator==(const LIter< T >& other) const noexcept
    {
      return curr_ == other.curr_;
    }

    bool operator!=(const LIter< T >& other) const noexcept
    {
      return curr_ != other.curr_;
    }

  private:
    const detail::NodeBase* curr_;

    explicit LCIter(const detail::NodeBase* node) noexcept:
      curr_(node)
    {}
  };

  template < class T >
  class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  public:
    LIter() noexcept:
      curr_(nullptr)
    {}

    T& operator*() noexcept
    {
      return List< T >::castNode(curr_)->val;
    }

    LIter& operator++() noexcept
    {
      curr_ = curr_->next;
      return *this;
    }

    LIter operator++(int) noexcept
    {
      LIter temp = *this;
      ++(*this);
      return temp;
    }

    LIter& operator--() noexcept
    {
      curr_ = curr_->prev;
      return *this;
    }

    LIter operator--(int) noexcept
    {
      LIter temp = *this;
      --(*this);
      return temp;
    }

    T* operator->() noexcept
    {
      return std::addressof(List< T >::castNode(curr_)->val);
    }

    bool operator==(const LIter& other) const noexcept
    {
      return curr_ == other.curr_;
    }

    bool operator!=(const LIter& other) const noexcept
    {
      return !(*this == other);
    }

    bool operator==(const LCIter< T >& other) const noexcept
    {
      return curr_ == other.curr_;
    }

    bool operator!=(const LCIter< T >& other) const noexcept
    {
      return curr_ != other.curr_;
    }

  private:
    detail::NodeBase* curr_;

    explicit LIter(detail::NodeBase* node) noexcept:
      curr_(node)
    {}
  };
}

#endif