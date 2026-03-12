namespace zubarev
{
  template <typename T>
  struct Node
  {
    T data;
    Node *next;

    Node(const T &v, Node *n = nullptr) : val(v),
                                          next(n) {};
  };
}
