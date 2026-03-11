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
    List() : pseudo(new Node< T >())
    {
      pseudo->next = pseudo;
      pseudo->prev = pseudo;
    }

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
