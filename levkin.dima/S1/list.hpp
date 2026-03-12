#ifndef LIST
#define LIST
#include <utility>

namespace levkin {
  template < class T > class List;
  template < class T > class LIter;
  template < class T > class LCIter;
  template < class T > struct Node {
    T val;
    Node< T >*prev, *next;
  };

  template < class T > class List
  {

  public:
    LIter< T > begin() { return LIter< T >(pseudo->next); }
    LCIter< T > begin() const { return cbegin(); }
    LCIter< T > cbegin() const { return LCIter< T >(pseudo->next); }

    LIter< T > end() { return LIter< T >(pseudo); }
    LCIter< T > end() const { return cend(); }
    LCIter< T > cend() const { return LCIter< T >(pseudo); }

    void insertBack(T val)
    {
      Node< T >* lst = pseudo->prev;
      Node< T >* newLst = new Node< T >{val, lst, pseudo};
      lst->next = newLst;
      pseudo->prev = newLst;
    }

    List() : pseudo(new Node< T >())
    {
      pseudo->next = pseudo;
      pseudo->prev = pseudo;
    }
    void erase(LIter< T > from, LIter< T > to)
    {
      while (from != to) {
        erase(from++);
      }
    }

    void erase(LIter< T > pos)
    {
      if (pos == end())
        return;
      __eraseFast(pos);
    }

    void clear() { erase(begin(), end()); }
    List(T val) : List() { insertBack(val); }

    ~List()
    {
      if (pseudo == nullptr) {
        return;
      }
      clear();
      delete pseudo;
    }

    List(const List< T >& a) : List()
    {
      try {

        for (LCIter< T > i = a.cbegin(); i != a.cend(); i++) {
          insertBack(*i);
        }
      } catch (...) {
        clear();
        throw;
      }
    };

    List(List< T >&& a) noexcept : List(a.pseudo) { a.pseudo = nullptr; };
    List< T >& operator=(const List< T >& a) = delete;
    List< T >& operator=(List< T >&& a) = delete;

  private:
    void swap(List< T >& a) { std::swap(a.pseudo, this); }
    List(Node< T >* pseudo_node) : pseudo(pseudo_node) {}
    void __eraseFast(LIter< T > pos)
    {
      Node< T >* toDelete = pos.curr;
      toDelete->prev->next = toDelete->next;
      toDelete->next->prev = toDelete->prev;

      delete toDelete;
    }

    Node< T >* pseudo;
  };

  template < class T > class LCIter
  {
    friend class List< T >;
    friend class LIter< T >;

  public:
    LCIter< T >(Node< T >* node) : curr(node) {}
    LCIter< T >(LIter< T > it) : curr(it.curr) {}

    const T& operator*() const { return curr->val; }

    LCIter& operator++()
    {
      if (curr)
        curr = curr->next;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter temp = *this;
      ++(*this);
      return temp;
    }

    LCIter& operator--()
    {
      if (curr)
        curr = curr->prev;
      return *this;
    }

    LCIter operator--(int)
    {
      LCIter temp = *this;
      --(*this);
      return temp;
    }

    T const* operator->() const { return &(curr->val); }
    bool operator==(const LCIter& other) const { return curr == other.curr; }
    bool operator!=(const LCIter& other) const { return !(*this == other); }

    bool operator==(const LIter< T >& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const LIter< T >& other) const
    {
      return curr != other.curr;
    }

  private:
    Node< T >* curr = nullptr;
  };
  template < class T > class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  public:
    LIter< T >(Node< T >* node) : curr(node) {}

    T& operator*() { return curr->val; }

    LIter& operator++()
    {
      if (curr)
        curr = curr->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter temp = *this;
      ++(*this);
      return temp;
    }

    LIter& operator--()
    {
      if (curr)
        curr = curr->prev;
      return *this;
    }

    LIter operator--(int)
    {
      LIter temp = *this;
      --(*this);
      return temp;
    }

    T* operator->() { return &(curr->val); }
    bool operator==(const LIter& other) const { return curr == other.curr; }
    bool operator!=(const LIter& other) const { return !(*this == other); }

    bool operator==(const LCIter< T >& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const LCIter< T >& other) const
    {
      return curr != other.curr;
    }

  private:
    Node< T >* curr = nullptr;
  };

}


#endif
