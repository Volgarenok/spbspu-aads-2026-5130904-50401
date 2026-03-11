template < class T > class List;
namespace levkin
{

  template < class T > struct Node {
    T val;
    Node< T >*prev, *next;
  };

  template < class T > class List
  {

  public:
    Node< T >* begin()
    {
      return pseudo->next;
    }

    Node< T >* end()
    {
      return pseudo;
    }

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

    List(T val) : List()
    {

      insertBack(val);
    }

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
    
    
    List(const List<T>& a) = delete; 
    List(List<T>&& a) = delete; 
    List<T>& operator=(const List<T>& a) = delete; 
    List<T>& operator=(List<T>&& a) = delete;

  private:
    Node< T >* pseudo;
  };
  
  template < class T > class LIter
  {
    friend class List< T >;

  public:
  };

  template < class T > class LCIter
  {
    friend class List< T >;
  };
}
