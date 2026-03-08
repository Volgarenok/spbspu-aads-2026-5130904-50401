template <class T>
struct Node
{
  T data;
  Node* next;
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
