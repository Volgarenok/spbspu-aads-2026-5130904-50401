template <class T>
struct Node
{
  T data;
  Node* next;
};
template <class T>
class List;

template<class T>
class LIter
{
  friend class List<T>;
  private:
    Node<T>* node;
  public:
    LIter(Node<T>* n = nullptr) : node(n) {}
    T& operator*()
    {
      return node -> data;
    }
    LIter& operator++()
    {
      node = node -> next;
      return *this;
    }
    bool operator!=(const LIter& other) const
    {
      return node != other.node;
    }
};


template <class T>
class List
{
  private:
    Node<T>* head;
  public:
    List();
    ~List();
    
    void push_front(const T& value) noexcept;
    void pop_front() noexcept;
    void insert_after() noexcept;
    void erase_after() noexcept;

    iterator begin();
    iterator end();

    T& front();
};