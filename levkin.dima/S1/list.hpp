template < class T > class List;
namespace levkin {

  template < class T > struct Node {
    T val;
    Node< T >*prev, *next;
  };

  template < class T > class List
  {
  private:
    Node< T >*head, *tail;
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
