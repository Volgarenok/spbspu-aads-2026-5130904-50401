#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>

#include <liter.hpp>
#include <lciter.hpp>

namespace chernov {
  template< class T >
  struct Node {
    T data;
    Node< T > * next;
  };

  template< class T >
  class List {
  public:
    List();
    List(const List< T > & list);
    List(List< T > && list) noexcept;
    ~List() noexcept;
    List< T > & operator=(const List< T > & list);
    List< T > & operator=(List< T > && list) noexcept;
    T & first() const;
    LIter< T > beforeBegin() const noexcept;
    LCIter< T > cbeforeBegin() const noexcept;
    LIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
    LIter< T > insertAfter(LIter< T > pos, const T & value);
    LIter< T > insertAfter(LIter< T > pos, T && value);
    LIter< T > eraseAfter(LIter< T > pos);
    LIter< T > eraseAfter(LIter< T > first, LIter< T > last);
    void pushFront(const T & value);
    void pushFront(T && value);
    void popFront();
  private:
    Node< T > * fake_;
    size_t size_;
    Node< T > * createFake();
    void removeFake() noexcept;
  };

  template< class T >
  Node< T > * List< T >::createFake()
  {
    fake_ = new Node<T>();
    return fake_;
  }

  template< class T >
  void List< T >::removeFake() noexcept
  {
    delete fake_;
  }

  template< class T >
  List< T >::List():
    fake_(nullptr),
    size_(0)
  {
    createFake();
    fake_->next = fake_;
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
    removeFake();
  }

  template< class T >
  List< T >::List(const List< T > & list):
    fake_(nullptr),
    size_(0)
  {
    createFake();
    fake_->next = fake_;
    if (!list.empty()) {
      LIter< T > pos = beforeBegin();
      LIter< T > iter = list.begin();
      do {
        pos = insertAfter(pos, *iter);
        ++iter;
      } while (iter != list.begin());
    }
  }

  template< class T >
  List< T >::List(List< T > && list) noexcept:
    fake_(list.fake_),
    size_(list.size_)
  {
    list.fake_ = nullptr;
    list.size_ = 0;
  }

  template< class T >
  List< T > & List< T >::operator=(const List< T > & list)
  {
    if (this == &list) {
      return *this;
    }
    clear();
    if (!list.empty()) {
      LIter< T > pos = beforeBegin();
      LIter< T > iter = list.begin();
      do {
        pos = insertAfter(pos, *iter);
        ++iter;
      } while (iter != list.begin());
    }
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(List< T > && list) noexcept
  {
    if (this == &list) {
      return *this;
    }
    clear();
    removeFake();
    fake_ = list.fake_;
    size_ = list.size_;
    list.fake_ = nullptr;
    list.size_ = 0;
    return *this;
  }

  template< class T >
  T & List< T >::first() const
  {
    return fake_->next->data;
  }

  template< class T >
  LIter< T > List< T >::beforeBegin() const noexcept
  {
    return {fake_, fake_};
  }

  template< class T >
  LCIter< T > List< T >::cbeforeBegin() const noexcept
  {
    return {fake_, fake_};
  }

  template< class T >
  LIter< T > List< T >::begin() const noexcept
  {
    return {fake_->next, fake_};
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return {fake_->next, fake_};
  }

  template< class T >
  LIter< T > List< T >::end() const noexcept
  {
    return {fake_, fake_};
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return {fake_, fake_};
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void List< T >::clear()
  {
    if (fake_ == nullptr) {
      return;
    }
    Node< T > * node = fake_->next;
    while (node != fake_) {
      Node< T > * next = node->next;
      delete node;
      node = next;
    }
    fake_->next = fake_;
    size_ = 0;
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > pos, const T & value)
  {
    Node< T > * node = new Node< T >{value, pos.ptr->next};
    pos.ptr->next = node;
    ++size_;
    return {node, fake_};
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > pos, T && value)
  {
    Node< T > * node = new Node< T >{std::move(value), pos.ptr->next};
    pos.ptr->next = node;
    ++size_;
    return {node, fake_};
  }

  template< class T >
  LIter< T > List< T >::eraseAfter(LIter< T > pos)
  {
    if (pos.ptr == nullptr || (pos.ptr == fake_ && fake_->next == fake_)) {
      return end();
    }
    Node< T > * del_node = pos.ptr->next;
    if (del_node == fake_) {
      pos.ptr = fake_;
      del_node = fake_->next;
      if (del_node == fake_) {
        return end();
      }
    }
    pos.ptr->next = del_node->next;
    delete del_node;
    --size_;
    return {pos.ptr->next, fake_};
  }

  template< class T >
  LIter< T > List< T >::eraseAfter(LIter< T > first, LIter< T > last)
  {
    if (first == last) {
      return last;
    }
    Node< T > * prev = first.ptr;
    Node< T > * curr = prev->next;
    bool crossed_fake = false;
    while (curr != last.ptr) {
      if (curr == fake_) {
        crossed_fake = true;
        curr = fake_->next;
        if (curr == last.ptr) {
          break;
        }
      }
      Node< T > * next = curr->next;
      delete curr;
      --size_;
      curr = next;
    }
    prev->next = last.ptr;
    if (crossed_fake) {
      last.ptr->next = fake_;
      fake_->next = first.ptr;
    }
    return last;
  }

  template< class T >
  void List< T >::pushFront(const T & value)
  {
    insertAfter(beforeBegin(), value);
  }

  template< class T >
  void List< T >::pushFront(T && value)
  {
    insertAfter(beforeBegin(), std::move(value));
  }

  template< class T >
  void List< T >::popFront()
  {
    eraseAfter(beforeBegin());
  }
}

#endif
