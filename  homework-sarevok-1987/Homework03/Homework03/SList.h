#pragma once
#include <memory>
#include "SListNode.h"
//SList Class foward declaration for friend declaration in the Iterators 
template<typename T, typename A = std::allocator<SListNode<T>>>
class SList;
//Const Iterator 
template < typename T>
class ConstSListIterator
{
	friend SList<T>;
	const SListNode<T>* cur;

public:
	
	typedef typename const T value_type;
	typedef typename std::ptrdiff_t difference_type;
	typedef typename const T* pointer;
	typedef typename const T& reference;
	typedef typename std::forward_iterator_tag iterator_category;

	ConstSListIterator()
	{
		cur = NULL;
	}

	ConstSListIterator(SListNode<T>& i_node)
	{
		cur = new SListNode<T>(*(i_node.val),i_node.Next);
	}

	ConstSListIterator(const ConstSListIterator& i_copy)
	{
		cur = new SListNode<T>(*(i_copy.cur));
	}

	ConstSListIterator& operator ++()
	{
		cur = cur->Next;
		return (*this);
	}

	ConstSListIterator operator ++(int)
	{
		ConstSListIterator tmp = (*this)
			++(*this);
		return tmp;
	}

	reference operator *()
	{
		return cur->val;
	}

	pointer operator ->()
	{
		return &(cur->val);
	}

	bool operator==(const ConstSListIterator<T>& i_slist2) const
	{
		return (*this).cur->Next == i_slist2.cur->Next;
	}

	bool operator!=(const ConstSListIterator<T>& i_slist2) const
	{
		return !((*this)==i_slist2);
	}
};
//Iterator
template < typename T>
class SListIterator
{

	SListNode<T>* cur;
	friend SList<T>;
public:
	typedef typename T value_type;
	typedef typename std::ptrdiff_t difference_type;
	typedef typename T* pointer;
	typedef typename T& reference;
	typedef typename std::forward_iterator_tag iterator_category;

	SListIterator()
	{
		cur = NULL;
	}

	SListIterator(SListNode<T>& i_node)
	{
		cur = &i_node;
	}

	SListIterator(const SListIterator& i_copy)
	{
		cur = i_copy.cur;
	}

	SListIterator& operator ++()
	{
		cur = cur->Next;
		return (*this);
	}

	SListIterator operator ++(int)
	{
		SListIterator tmp = (*this);
		++(*this);
		return tmp;
	}

	reference operator *()
	{
		return cur->val;
	}

	pointer operator ->()
	{
		return &(cur->val);
	}
	bool operator==(const SListIterator<T>& i_slist2) const
	{
		return (*this).cur == i_slist2.cur;
	}
	bool operator!=(const SListIterator<T>& i_slist2) const
	{
		return !((*this)==i_slist2);
	}
};
// SList is a class that follow the STL list interface. It's a single list
template<typename T, typename A >
class SList
{
	// STL Allocator
	A alloc;

	typedef typename SListNode<T>* NodePtr;
	typedef typename SListNode<T> Node;
	//num of element in the list
	size_t MySize;
	//Pointer to start list
	NodePtr start_list;
	//Pointer to end list
	NodePtr end_list;

public:

	typedef typename T value_type;
	typedef typename A allocator_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename T* pointer;
	typedef const T* const_pointer;

	typedef typename SListIterator<T> iterator;
	typedef typename ConstSListIterator<T> const_iterator;
	// default constructor. Constructs empty container.
	explicit SList( const A& a = A() ):alloc(a),MySize(0)
	{

	}
	//constructs the container with count value-initialized (default constructed, for classes) instances of T. No copies are made.
	explicit SList( size_type count ):MySize(count)
	{
		NodePtr firstNode = alloc.allocate(1);
		firstNode = new(firstNode) Node(); // construct the object to adress 
		start_list = end_list = firstNode;
		for (unsigned int i = 0; i < count; ++i)
		{
			NodePtr nextNode= alloc.allocate(1);
			nextNode = new(nextNode) Node();
			firstNode->Next= nextNode;
			end_list = nextNode;
			firstNode = nextNode;
		}
	}
	//  constructs the container with count copies of elements with value i_val.
	explicit SList( size_type count, const T& i_val, const A& a = A()): alloc(a), MySize(count)
	{
		NodePtr firstNode = alloc.allocate(1);
		firstNode = new(firstNode) Node(i_val);
		start_list = end_list = firstNode;
		for (unsigned int i = 0; i < count-1; ++i)
		{
			NodePtr nextNode= alloc.allocate(1);
			nextNode = new(nextNode) Node(i_val);
			firstNode->Next= nextNode;
			end_list = nextNode;
			firstNode = nextNode;
		}
	}
	//constructs the container with the contents of the range [first, last).
	//template <typename InputIterator>
	//SList( InputIterator first, InputIterator last, const A& a = A() ) : alloc(a),MySize(0)
	//{
	//	NodePtr tmp = alloc.allocate(1);
	//	//tmp = new(tmp) Node(*first);
	//	alloc.construct(tmp,Node(*first));
	//	start_list = end_list = tmp;
	//	++first;
	//	++MySize;
	//	while (first!=last)
	//	{
	//		NodePtr tmp1= alloc.allocate(1);
	//		tmp1 = new(tmp1) Node(*first);
	//		tmp->Next= tmp1;
	//		end_list = tmp1;
	//		tmp = tmp1;
	//		++first;
	//		++MySize;
	//	}
	//}
	//Destruct the list
	~SList()
	{
		if(start_list != NULL)
		{
			SListNode<T>* tmp_erase = start_list;
			SListNode<T>* tmp_nx_er = tmp_erase->Next;
			while (tmp_erase != NULL)
			{
				tmp_erase->~Node();
				alloc.deallocate(tmp_erase,1);
				tmp_erase = tmp_nx_er;
				if(tmp_nx_er!=NULL)
					tmp_nx_er = tmp_erase->Next;
			}
		}
	}
	//Returns the number of elements in the container
	size_type size() const
	{
		return MySize;
	}
	//Returns an iterator to the first element of the container.
	iterator begin()
	{
		return iterator(*start_list);
	}

	const_iterator cbegin() const
	{
		return const_iterator(*start_list);
	}
	//Returns an iterator to the last element of the container.
	iterator end() const
	{
		return iterator(*(end_list->Next));
	}

	const_iterator cend() const
	{
		return const_iterator(*end_list);
	}
	//Checks if the container has no elements, i.e. whether begin() == end().
	bool empty() const
	{
		return begin()==end();
	}
	//Appends the given element i_val to the end of the container.
	//No iterators or references are invalidated.
	void push_back( const T& i_val )
	{
		NodePtr newNode = alloc.allocate(1);
		newNode = new(newNode) Node(i_val);
		(*end_list).Next = newNode;
		end_list = newNode;
		if (start_list == NULL)
			start_list = end_list;
		++MySize;
	}
	//Appends the given element i_val to the end of the container.
	//No iterators or references are invalidated.
	void push_front( const T& i_val )
	{
		NodePtr newNode = alloc.allocate(1);
		newNode = new(newNode) Node(i_val);
		newNode->Next = start_list;
		start_list = newNode;
		++MySize;
		if(MySize == 1)
			end_list = start_list;
	}
	//Removes the last element of the container.
	//References and iterators to the erased element are invalidated.
	void pop_back()
	{
		NodePtr new_last = start_list;
		while (end_list != new_last->Next)
			new_last = new_last->Next;
		new_last->Next = NULL;
		end_list->~SListNode<T>();
		alloc.deallocate(end_list,1);
		end_list = new_last;
		--MySize;
	}
	//Removes the first element of the container.
	//References and iterators to the erased element are invalidated.
	void pop_front()
	{
		NodePtr tmp_erase = start_list;
		start_list = start_list->Next;
		tmp_erase->~SListNode<T>();
		alloc.deallocate(tmp_erase,1);
		--MySize;
	}
	//Returns reference to the first element in the container.
	reference front()
	{
		return start_list->val;
	}

	const_reference front() const
	{
		return const *(start_list->val);
	}
	//Returns reference to the last element in the container.
	reference back()
	{
		return end_list->val;
	}

	const_reference back() const
	{
		return end_list->val;
	}
	//inserts i_val before the element pointed to by i_pos
	iterator insert( iterator i_pos, const T& i_val )
	{
		if(i_pos.cur == start_list)
		{
			push_front(i_val);
			return iterator(*(start_list->Next));
		}
		if(i_pos.cur == end_list->Next)
		{
			push_back(i_val);
			return iterator(*(end_list->Next));
		}
		else
		{
			NodePtr nextNode = start_list;
			while (i_pos.cur != nextNode->Next)
			{
				nextNode = nextNode->Next;
			}

			NodePtr newNode = alloc.allocate(1);
			newNode = new(newNode) Node(i_val);
			newNode->Next = nextNode->Next;
			nextNode->Next = newNode;
			++MySize;

			return iterator(*(newNode->Next));
		}
	}
	//inserts count copies of the i_val before the element pointed to by i_pos
	iterator insert( iterator i_pos, size_type i_count, const T& i_val )
	{
		for (unsigned int i=0;i < i_count ; ++i)
			insert(i_pos,i_val);
		return i_pos;
	}
	//inserts elements from range [first, last) before the element pointed to by i_pos
	template< typename InputIterator >
	iterator insert( InputIterator first, InputIterator last, iterator i_pos)
	{
		while(first != last)
		{
			insert(i_pos,*first);
			++first;
		}
		return i_pos;
	}
	//Removes all elements from the container. The past-the-end iterators are not invalidated.
	void clear()
	{
		SListNode<T>* tmp_erase = start_list;
		SListNode<T>* tmp_nx_er = tmp_erase->Next;
		while (tmp_erase != NULL)
		{
			tmp_erase->~Node();
			alloc.deallocate(tmp_erase,1);
			tmp_erase = tmp_nx_er;
			if(tmp_nx_er!=NULL)
				tmp_nx_er = tmp_erase->Next;
		}
		start_list = alloc.allocate(1);
		end_list = alloc.allocate(1);
		start_list = NULL;
		end_list = new(end_list) Node();
		MySize = 0;
	}
	//Removes the element at i_pos.
	iterator erase( iterator i_pos )
	{
		if(i_pos.cur == start_list)
		{
			pop_front();
			return iterator(*(start_list->Next));
		}
		else if (i_pos.cur == end_list->Next)
		{
			pop_back();
			return iterator(*(end_list));
		}
		else
		{
			NodePtr tmp_strart = start_list;
			while (i_pos.cur != tmp_strart->Next)
			{
				tmp_strart = tmp_strart->Next;
			}
			NodePtr tmp_erase = tmp_strart->Next;
			tmp_strart->Next = tmp_erase->Next;
			tmp_erase->~SListNode<T>();
			alloc.deallocate(tmp_erase,1);
			--MySize;
			return iterator (*(tmp_strart->Next));
		}
	}
	//Removes the elements in the range [first; last).
	iterator erase( iterator first, iterator last )
	{
		if (first.cur == start_list && last.cur == end_list->Next)
		{
			clear();
			return iterator(*end_list);
		}
		iterator it;
		while(first!= last)
		{
			it = first;
			++first;
			erase(it);
		}
		return last;
	}
	//Resizes the container to contain i_count elements.
	//If the current size is less than i_count, additional elements are appended and initialized with i_val.
	//If the current size is greater than i_count, the container is reduced to its first count elements.
	void resize( size_type i_count, T i_val = T() )
	{
		if(MySize < i_count)
		{
			while(MySize != i_count)
				push_back(i_val);
		}
		else if (MySize > i_count)
		{
			while(MySize != i_count)
				pop_back();
		}
	}
	//Merges two lists into one.
	void merge( SList& i_other )
	{
		for (iterator it = i_other.begin();it != i_other.end();++it)
			push_back(*it);
	}
	//Removes all elements satisfying specific criteria.
	void remove( const T& i_val )
	{
		iterator it = begin();
		while(it != end())
		{
			if(*it == i_val)
				erase(it++);
			else
				++it;
		}
	}
	//Exchanges the contents of the container with those of other. All iterators and references remain valid.
	void swap( SList& i_other )
	{
		NodePtr swp = start_list;
		start_list = i_other.start_list;
		i_other.start_list = swp;
		swp=end_list;
		end_list = i_other.end_list;
		i_other.end_list = swp;
		size_type swp_size = MySize;
		MySize = i_other.MySize;
		i_other.MySize = swp_size;
	}
};
//
// Checks if the contents of lhs and rhs are equal, that is, lhs.size() == rhs.size() and each element in lhs has equivalent element in rhs at the same position.
template< typename T, typename Alloc >
bool operator==( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	if(lhs.size() != rhs.size())
		return false;
	SList<T,Alloc>::iterator it = lhs.begin();
	SList<T,Alloc>::iterator it1 = rhs.begin();

	while(it != lhs.end() && it != rhs.end())
	{
		if(*it!=*it1)
			return false;
		++it;
		++it1;
	}
	return true;
}

template< typename T, typename Alloc >
bool operator!=( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	return !(lhs==rhs);
}
//Compares the contents of lhs and rhs lexicographically. The comparison is performed by a function equivalent to
template< typename T, typename Alloc >
bool operator<( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template< typename T, typename Alloc >
bool operator<=( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	return (lhs == rhs) || (lhs < rhs);
}

template< typename T, typename Alloc >
bool operator>( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	return !(lhs<=rhs);
}

template< typename T, typename Alloc >
bool operator>=( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	return lhs == rhs || lhs > rhs;
}