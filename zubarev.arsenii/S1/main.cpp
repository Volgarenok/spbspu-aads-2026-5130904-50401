#include <iostream>
template < class T >
class List;

template<class T>
class LIter {
    friend class List<T>;

private:
    typename List<T>::Node* ptr;
    LIter(typename List<T>::Node* p);

public:
    T& operator*();
    LIter<T> operator++();
    bool operator!=(const LIter&) const;
    bool operator==(const LIter&) const;
};
template<class T>
LIter<T>::LIter(typename List<T>::Node* p) : ptr(p) {}

template<class T>
T& LIter<T>::operator*() {
  return ptr->val;
}
template<class T>
LIter<T> LIter<T>::operator++() { 
  ptr = ptr->next;
  return *this;
}
template<class T>
bool LIter<T>::operator!=(const LIter& iter) const {
  return ptr!=iter.ptr;
}
template<class T>
bool LIter<T>::operator==(const LIter& iter) const {
  return ptr==iter.ptr;
}

template < class T >
class LCIter
{
  friend class List< T >;

private:
};

struct Data
{
  std::string name;
  List<int>* numbers;
};

template < class T >
class List
{

private:
  struct Node
  {
    T val;
    Node* next;
  };
  Node* head;
public:
  LIter<T> begin();
  void clear();
  LIter<T> end();
  bool empty() const;
  void pop_front(const T&);
  void push_front(const T&);

};

template <class T>
LIter<T> List<T>::begin() {
  return LIter(head);
}
template <class T>
LIter<T> List<T>::end() {
  return LIter(nullptr);
}
template <class T>
void List<T>::clear()
{
  LIter<T> it=begin();
  while (it!=end()) {
    LIter<T> tmp(it.ptr->next);
    delete it.ptr;
    it=tmp;
  }
  head=nullptr;
}
template <class T>
bool List<T>::empty() const {
  return begin()==end()
}

int main() {

}
