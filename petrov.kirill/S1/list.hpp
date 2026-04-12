#ifndef LIST_HPP
#define LIST_HPP

namespace petrov
{
	template< class T >
	struct node
	{
		T val;
		node<T>*	next;
		node<T>*	prev;
	};
	
	template<class T>
	class List
	{
		node<T>* h;
		node<T>* t;
		size_t s;
	public:
		~List();
		List();
		List(const List<T>& l);
		List(List<T>&& l) noexcept;
		List(size_t s, const T& init);
		
		List<T>& operator=(List<T>&& l) noexcept;
		List<T>& operator=(const List<T>& l);

		bool IsEmpty() const noexcept;
		size_t getSize() const noexcept;
		ListIterator<T> begin();
		ListIterator<T> end();
		
		void push_back(const T& d);
		void push_front(const T& d);
		void pop_back();
		void pop_front();
	};
	template<class T>
	class ListIterator
	{
		friend class List<T>;
		node<T>* cur;
		ListIterator(node<T>* n);
	public:
		bool operator==(const ListIterator<T>& i) const;
		bool operator!=(const ListIterator<T>& i) const;
		T& operator*() const;
		ListIterator<T>& operator++();
		ListIterator<T>& operator--();
		T* operator->() const;
	};

	template<class T>
	class ListIteratorConst
	{
		friend class List<T>;
		const node<T>* cur;
		ListIteratorConst(const node<T>* n);
	public:
		bool operator==(const ListIteratorConst<T>& i) const;
		bool operator!=(const ListIteratorConst<T>& i) const;
		const T& operator*() const;
		ListIteratorConst<T>& operator++();
		ListIteratorConst<T>& operator--();
		const T* operator->() const;
	};
};

template<class T>
	List<T>::List():
	h(nullptr),
	t(nullptr),
	s(0)
{}

template<class	T>
bool List<T>::IsEmpty() const noexcept
{
	return (!s || h == nullptr);
}

template<class T>
size_t List<T>::getSize() const noexcept
{
	return s;
}

template<class T>
void List<T>::push_back(const T& d)
{
	if (IsEmpty())
	{
		h = new node<T>;
		h->val = d;
		h->next = nullptr;
		h->prev = nullptr;
		t = h;
		s = 1;
		return;
	}
	t->next = new node<T>;
	t->next->val = d;
	t->next->prev = t;
	t = t->next;
	t->next = nullptr;
	s++;
}

template<class T>
void List<T>::push_front(const T& d)
{
	if (IsEmpty())
	{
		h = new node<T>;
		h->val = d;
		h->next = nullptr;
		h->prev = nullptr;
		t = h;
		s = 1;
		return;
	}
	h->prev = new node<T>;
	h->prev->val = d;
	h->prev->next = t;
	h = h->prev;
	h->prev = nullptr;
	s++;
}

template<class T>
List<T>::List(const List<T>& l):
	h(nullptr),
	t(nullptr),
	s(0)
{
	node<T>*	nod = l.h;
	while (nod != nullptr)
	{
		push_back(nod->val);
		nod = nod->next;
	}
}

#endif