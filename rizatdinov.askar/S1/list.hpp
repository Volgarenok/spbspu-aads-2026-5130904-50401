#ifndef LIST_HPP
#define LIST_HPP

#include <initializer_list>
#include <utility>
#include <stdexcept>

namespace rizatdinov
{
  template< class Elem >
  struct Link
  {
    Elem val;
    Link* succ;
  };

  template< class Elem > class List;
  template< class Elem > class LCIter;

  template< class Elem >
  class LIter {
    friend class List< Elem >;
    friend class LCIter< Elem >;

    Link< Elem >* curr;
  public:
    LIter(): curr(nullptr) {}
    LIter(Link< Elem >* p): curr(p) {}

    LIter& operator++() { curr = curr->succ; return *this; }
    LIter operator++(int) {
      LIter temp = *this;
      curr = curr->succ;
      return temp;
    }
    Elem& operator*() { return curr->val; }
    bool operator==(const LIter& b) const { return curr == b.curr; }
    bool operator!=(const LIter& b) const { return curr != b.curr; }
  };

  template< class Elem >
  class LCIter {
    friend class List< Elem >;

    const Link< Elem >* curr;
  public:
    LCIter(): curr(nullptr) {}
    LCIter(const Link< Elem >* p): curr(p) {}
    LCIter(const LIter< Elem >& other) : curr(other.curr) {}

    LCIter& operator++() { curr = curr->succ; return *this; }
    LCIter operator++(int) {
      LCIter temp = *this;
      curr = curr->succ;
      return temp;
    }
    const Elem& operator*() const { return curr->val; }
    bool operator==(const LCIter& b) const { return curr == b.curr; }
    bool operator!=(const LCIter& b) const { return curr != b.curr; }
  };

  template< class Elem >
  class List {
    Link< Elem >* fake;
  public:
    using iterator = LIter< Elem >;
    using const_iterator = LCIter< Elem >;

    List();
    List(std::initializer_list< Elem > init);
    List(const List& other);
    List(List&& other);
    ~List();

    List& operator=(const List& other);
    List& operator=(List&& other);

    iterator before_begin();
    const_iterator before_begin() const;
    const_iterator cbefore_begin() const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    iterator insert_after(iterator p, const Elem& v);
    iterator insert_after(iterator p, Elem&& v);
    iterator erase_after(iterator p);

    void push_front(const Elem& v);
    void push_front(Elem&& v);
    void pop_front();

    bool is_empty() const;
    void clear();
  };

  template< class Elem >
  List< Elem >::List() {
    fake = new Link< Elem >;
    fake->succ = nullptr;
  }

  template< class Elem >
  List< Elem >::List(std::initializer_list< Elem > init) {
    fake = new Link< Elem >;
    fake->succ = nullptr;

    auto it = iterator(fake);
    for (auto i = init.begin(); i != init.end(); ++i) {
      it = insert_after(it, *i);
    }
  }

  template< class Elem >
  List< Elem >::List(const List& other) {
    fake = new Link< Elem >;
    fake->succ = nullptr;
    iterator ptr = iterator(fake);
    for (const_iterator bpr = other.begin(); bpr != other.end(); ++bpr) {
      ptr = insert_after(ptr, *bpr);
    }
  }

  template< class Elem >
  List<Elem>::List(List&& other) {
    fake = other.fake;
    other.fake = new Link< Elem >;
    other.fake->succ = nullptr;
  }

  template< class Elem >
  List<Elem>::~List() {
    clear();
    delete fake;
  }

  template< class Elem >
  List<Elem>& List<Elem>::operator=(const List& other) {
    if (this != &other) {
      clear();
      iterator ptr = iterator(fake);
      for (const_iterator bpr = other.begin(); bpr != other.end(); ++bpr) {
        ptr = insert_after(ptr, *bpr);
      }
    }
    return *this;
  }

  template< class Elem >
  List<Elem>& List<Elem>::operator=(List&& other) {
    if (this != &other) {
      clear();
      delete fake;
      fake = other.fake;
      other.fake = new Link< Elem >;
      other.fake->succ = nullptr;
    }
    return *this;
  }

  template< class Elem >
  typename List<Elem>::iterator List<Elem>::before_begin() {
    return iterator(fake);
  }

  template< class Elem >
  typename List<Elem>::const_iterator List<Elem>::before_begin() const {
    return const_iterator(fake);
  }

  template< class Elem >
  typename List<Elem>::const_iterator List<Elem>::cbefore_begin() const {
    return const_iterator(fake);
  }

  template< class Elem >
  typename List<Elem>::iterator List<Elem>::begin() {
    return iterator(fake->succ);
  }

  template< class Elem >
  typename List<Elem>::const_iterator List<Elem>::begin() const {
    return const_iterator(fake->succ);
  }

  template< class Elem >
  typename List<Elem>::const_iterator List<Elem>::cbegin() const {
    return const_iterator(fake->succ);
  }

  template< class Elem >
  typename List<Elem>::iterator List<Elem>::end() {
    return iterator(nullptr);
  }

  template< class Elem >
  typename List<Elem>::const_iterator List<Elem>::end() const {
    return const_iterator(nullptr);
  }

  template< class Elem >
  typename List<Elem>::const_iterator List<Elem>::cend() const {
    return const_iterator(nullptr);
  }

  template< class Elem >
  typename List<Elem>::iterator List<Elem>::insert_after(iterator p, const Elem& v) {
    if (p == end()) {
      throw std::out_of_range("error: can't insert");
    }
    Link<Elem>* s = new Link<Elem>{v, p.curr->succ};
    p.curr->succ = s;
    return iterator(s);
  }

  template< class Elem >
  typename List<Elem>::iterator List<Elem>::insert_after(iterator p, Elem&& v) {
    if (p == end()) {
      throw std::out_of_range("error: can't insert");
    }
    Link<Elem>* s = new Link<Elem>{std::move(v), p.curr->succ};
    p.curr->succ = s;
    return iterator(s);
  }

  template< class Elem >
  typename List<Elem>::iterator List<Elem>::erase_after(iterator p) {
    if (p == end() || p.curr->succ == nullptr) {
      throw std::out_of_range("error: can't erase");
    }
    Link<Elem>* s = p.curr->succ;
    p.curr->succ = s->succ;
    delete s;
    return iterator(p.curr->succ);
  }

  template< class Elem >
  void List<Elem>::push_front(const Elem& v) {
    insert_after(before_begin(), v);
  }

  template< class Elem >
  void List<Elem>::push_front(Elem&& v) {
    insert_after(before_begin(), std::move(v));
  }

  template< class Elem >
  void List<Elem>::pop_front() {
    erase_after(iterator(fake));
  }

  template< class Elem >
  bool List<Elem>::is_empty() const {
    return fake->succ == nullptr;
  }

  template< class Elem >
  void List<Elem>::clear() {
    while (fake->succ) {
      pop_front();
    }
  }
}

#endif
