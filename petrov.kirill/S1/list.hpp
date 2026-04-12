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
#endif