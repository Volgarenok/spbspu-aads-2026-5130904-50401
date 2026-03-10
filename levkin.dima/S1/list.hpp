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
