namespace levkin {
  template < typename T > class Queue
  {
  public:
    void push(T rhs);
    T drop();
  };
}
