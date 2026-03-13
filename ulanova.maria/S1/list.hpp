template <class T>
struct Node
{
  T data;
  Node<T>* next;
};
template <class T>
class List;

template<class T>
class LIter
{
  friend class List<T>;
private:
  Node<T>* node;
  Node<T>* head;
public:
  LIter(Node<T>* n = nullptr, Node<T>* h = nullptr) : node(n), head(h) {}
  T& operator*() noexcept
  {
    return node -> data;
  }
  LIter& operator++() noexcept
  {
    node = node -> next;
    if (node == head)
    {
      node = nullptr;
    }
    return *this;
  }
  bool operator!=(const LIter& other) const noexcept
  {
    return node != other.node;
  }
  T* operator->() noexcept
  {
    return &(node->data);
  }
};

template<class T>
class LCIter
{
  friend class List<T>;
private:
  Node<T>* node;
  Node<T>* head;
public:
  LCIter(Node<T>* n = nullptr, Node<T>* h = nullptr) : node(n), head(h) {}
  const T& operator*() const noexcept
  {
    return node -> data;
  }
  LCIter& operator++() noexcept
  {
    node = node -> next;
    if (node == head)
    {
      node = nullptr;
    }
    return *this;
  }
  bool operator!=(const LCIter& other) const noexcept
  {
    return node != other.node;
  }
  T* operator->() const noexcept
  {
    return &(node->data);
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

  LIter<T> insert_after(LIter<T> pos, const T& value) noexcept;
  void erase_after(LIter<T> pos) noexcept;

  LIter<T> begin() noexcept;
  LIter<T> end() noexcept;

  LCIter<T> cbegin() const noexcept;
  LCIter<T> cend() const noexcept;

  void clear();

  T& front();
};

template<class T>
List<T>::List() : head(nullptr) {}

template <class T>
void List<T>::push_front(const T& value) noexcept
{
  Node<T>* new_node = new Node<T>{value,nullptr};
  if (!head)
  {
    head = new_node;
    head -> next = head;
    return;
  }
  Node<T>* last = head;
  while (last -> next != head)
  {
    last = last -> next;
  }
  new_node -> next = head;
  last -> next = new_node;
  head = new_node;
}

template <class T>
void List<T>::pop_front() noexcept
{
  if (!head)
  {
    return;
  }
  if (head -> next == head)
  {
    delete head;
    head = nullptr;
    return;
  }
  Node<T>* last = head;
  while (last -> next != head)
  {
    last = last -> next;
  }
  Node<T>* temp = head;
  head = head -> next;
  last -> next = head;
  delete temp;
}

template <class T>
T& List<T>::front()
{
  return head -> data;
}

template <class T>
LIter<T> List<T>::begin() noexcept
{
  return LIter<T>(head,head);
}
template <class T>
LIter<T> List<T>::end() noexcept
{
  return LIter<T>(nullptr,head);
}

template <class T>
LCIter<T> List<T>::cbegin() const noexcept
{
  return LCIter<T>(head,head);
}
template <class T>
LCIter<T> List<T>::cend() const noexcept
{
  return LCIter<T>(nullptr,head);
}

template <class T>
void List<T>::clear()
{
  while (head)
  {
    pop_front();
  }
}
template <class T>
List<T>::~List()
{
  clear();
}

template <class T>
LIter<T> List<T>::insert_after(LIter<T> pos, const T& value) noexcept
{
  if (!pos.node)
  {
    return end();
  }
  Node<T>* new_node = new Node<T>{value, pos.node -> next};
  pos.node -> next = new_node;
  return LIter<T>(new_node);
}
template <class T>
void List<T>::erase_after(LIter<T> pos) noexcept
{
  Node<T>* temp = pos.node -> next;
  if (!temp)
  {
    return;
  }
  pos.node -> next = temp -> next;
  delete temp;
}