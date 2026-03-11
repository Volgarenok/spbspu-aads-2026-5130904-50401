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
    Node<T> * begin(){
      return pseudo->next;
    }
    Node <T> * end(){
      return pseudo;
    }
    
    List() : pseudo(new Node< T >())
    {
      pseudo->next = pseudo;
      pseudo->prev = pseudo;
    }
    // List(T val) : pseudo(new Node< T >())
    // {
    //   pseudo->next = pseudo;
    //   pseudo->prev = pseudo;
    // constructor through insertafter
      
      
    // }

  private:
    Node< T >* pseudo;
  };
}

template < class T > class LIter
{
  friend class List< T >;

public:
};

template < class T > class LCIter
{
  friend class List< T >;
};
