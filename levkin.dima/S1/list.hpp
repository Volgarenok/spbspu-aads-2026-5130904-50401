namespace levkin
{
  template < class T > class List;
  template < class T > class LIter;

  template < class T > struct Node {
    T val;
    Node< T >*prev, *next;
  };

  template < class T > class List
  {

  public:
    LIter< T > begin() { return LIter< T >(pseudo->next); }

    LIter< T > end() { return LIter< T >(pseudo); }

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

    List(T val) : List() { insertBack(val); }

    ~List()
    {
      Node< T >* curr = pseudo->next;
      while (curr != pseudo) {
        Node< T >* nxt = curr->next;
        delete curr;
        curr = nxt;
      }
      delete pseudo;
    }

    List(const List< T >& a) = delete;
    List(List< T >&& a) = delete;
    List< T >& operator=(const List< T >& a) = delete;
    List< T >& operator=(List< T >&& a) = delete;

  private:
    Node< T >* pseudo;
  };

  template < class T > class LIter
  {
    friend class List< T >;

  public:
    LIter< T >(Node< T >* node) : curr(node) {}

    T& operator*() { return curr->val; }

    LIter& operator++()
    {
      if (curr)
        curr = curr->next;
      return *this;
    }

    T* operator->() { return &(curr->val); }
    LIter operator++(int)
    {
      LIter temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const LIter& other) { return curr == other.curr; }
    bool operator!=(const LIter& other) const { return !(*this == other); }

  private:
    Node< T >* curr = nullptr;
  };

  template < class T > class LCIter
  {
    friend class List< T >;
  };
}
