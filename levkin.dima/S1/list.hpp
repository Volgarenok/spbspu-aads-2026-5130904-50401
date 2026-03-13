#ifndef LIST
#define LIST
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace levkin {
  template < class T > class List;
  template < class T > class LIter;
  template < class T > class LCIter;
  struct NodeBase {
    NodeBase *prev, *next;
  };
  template < class T > struct Node : public NodeBase {
    T val;
  };

  template < class T > class List
  {

  public:
    LIter< T > begin()
    {
      return LIter< T >(pseudo->next);
    }
    LCIter< T > begin() const
    {
      return cbegin();
    }
    LCIter< T > cbegin() const
    {
      return LCIter< T >(pseudo->next);
    }

    LIter< T > end()
    {
      return LIter< T >(pseudo);
    }
    LCIter< T > end() const
    {
      return cend();
    }
    LCIter< T > cend() const
    {
      return LCIter< T >(pseudo);
    }

    void pushFront(T val)
    {
      insertAfter(LIter< T >(pseudo), val);
    }
    void pushBack(T val)
    {
      insertAfter(LIter< T >(pseudo->prev), val);
    }

    List() : pseudo(new NodeBase())
    {
      pseudo->next = pseudo;
      pseudo->prev = pseudo;
    }

    size_t size() const
    {
      auto it = cbegin();
      size_t count = 0;
      for (; it != cend(); ++it) {
        count++;
      }
      return count;
    }

    void clearAndInit(size_t size, T val)
    {
      clear();
      for (size_t i = 0; i < size; ++i) {
        this->pushBack(val);
      }
    }

    void erase(LIter< T > from, LIter< T > to)
    {
      while (from != to) {
        erase(from++);
      }
    }

    LIter< T > erase(LIter< T > pos)
    {
      if (pos == end())
        return pos;
      return __eraseFast(pos);
    }
    void popFront()
    {
      if (begin() != end()) {
        erase(begin());
      }
    };
    void popBack()
    {
      if (begin() != end()) {
        erase(LIter< T >(pseudo->prev));
      }
    };
    LIter< T > insertAfter(LIter< T > it, const T& val)
    {
      if (it.curr == nullptr)
        throw std::out_of_range("out of bounds or null. wierd");
      Node< T >* newNode = new Node< T >();
      newNode->val = val;
      newNode->prev = it.curr;
      newNode->next = it.curr->next;

      newNode->next->prev = newNode;
      it.curr->next = newNode;
      return LIter< T >(newNode);
    }
    void clear()
    {
      while (pseudo->next != pseudo) {
        __eraseFast(LIter< T >(pseudo->next));
      }
    }
    List(T val) : List()
    {
      pushBack(val);
    }

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
          pushBack(*i);
        }
      } catch (...) {
        clear();
        throw;
      }
    };

    List(List< T >&& a) noexcept : List(a.pseudo)
    {
      a.pseudo = nullptr;
    };
    List< T >& operator=(List< T > a)
    {
      a.swap(*this);
      return *this;
    }

  private:
    void swap(List< T >& a)
    {
      std::swap(a.pseudo, this->pseudo);
    }
    List(NodeBase* pseudo_node) : pseudo(pseudo_node)
    {
    }
    LIter< T > __eraseFast(LIter< T > pos)
    {
      NodeBase* toDelete = pos.curr;
      toDelete->prev->next = toDelete->next;
      toDelete->next->prev = toDelete->prev;

      NodeBase* nxt = toDelete->next;
      delete static_cast< Node< T >* >(toDelete);
      return LIter< T >(nxt);
    }

    NodeBase* pseudo;
  };

  template < class T > class LCIter
  {
    friend class List< T >;
    friend class LIter< T >;

  public:
    LCIter(NodeBase* node = nullptr) : curr(node)
    {
    }
    LCIter(LIter< T > it) : curr(it.curr)
    {
    }
    const T& operator*() const
    {
      return static_cast< Node< T >* >(curr)->val;
    }

    LCIter& operator++()
    {
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
      curr = curr->prev;
      return *this;
    }

    LCIter operator--(int)
    {
      LCIter temp = *this;
      --(*this);
      return temp;
    }

    T const* operator->() const
    {
      return &static_cast< Node< T >* >(curr)->val;
    }
    bool operator==(const LCIter& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const LCIter& other) const
    {
      return !(*this == other);
    }

    bool operator==(const LIter< T >& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const LIter< T >& other) const
    {
      return curr != other.curr;
    }

  private:
    NodeBase* curr = nullptr;
  };
  template < class T > class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  public:
    LIter(NodeBase* node) : curr(node)
    {
    }
    T& operator*()
    {
      return static_cast< Node< T >* >(curr)->val;
    }

    LIter& operator++()
    {
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
      curr = curr->prev;
      return *this;
    }

    LIter operator--(int)
    {
      LIter temp = *this;
      --(*this);
      return temp;
    }

    T* operator->()
    {
      return &(static_cast< Node< T >* >(curr)->val);
    }
    bool operator==(const LIter& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const LIter& other) const
    {
      return !(*this == other);
    }

    bool operator==(const LCIter< T >& other) const
    {
      return curr == other.curr;
    }
    bool operator!=(const LCIter< T >& other) const
    {
      return curr != other.curr;
    }

  private:
    NodeBase* curr = nullptr;
  };

}

#endif
