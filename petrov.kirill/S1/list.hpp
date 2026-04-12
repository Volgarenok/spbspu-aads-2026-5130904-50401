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
	};
	template<class T>
	class ListIterator
	{
		friend class List<T>;
		node<T>* cur;
		ListIterator(node<T>* n);
	public:
		bool operator==(const ListIterator i) const;
		bool operator!=(const ListIterator i) const;
		
		T& operator*() const;
		ListIterator& operator++();
	};
};
#endif