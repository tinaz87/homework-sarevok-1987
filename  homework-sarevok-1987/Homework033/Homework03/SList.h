#pragma once
#include <memory>
#include "SListNode.h"

template<typename T, typename A = std::allocator<SListNode<T>>>
class SList;

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

template<typename T, typename A >
class SList
{
	A alloc;
	
	typedef typename SListNode<T>* NodePtr;
	typedef typename SListNode<T> Node;

	size_t MySize;

	NodePtr start_list;
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

	explicit SList( const A& a = A() ):alloc(a),MySize(0)
	{

	}

	explicit SList( size_type count ):MySize(count)
	{
		NodePtr tmp = alloc.allocate(1);
		tmp = new(tmp) Node();
		start_list = end_list = tmp;
		for (unsigned int i = 0; i < count; ++i)
		{
			NodePtr tmp1= alloc.allocate(1);
			tmp1 = new(tmp1) Node();
			tmp->Next= tmp1;
			end_list = tmp1;
			tmp = tmp1;
		}
	}

	explicit SList( size_type count, const T& i_val, const A& a = A()): alloc(a), MySize(count)
	{
		NodePtr tmp = alloc.allocate(1);
		tmp = new(tmp) Node(i_val);
		start_list = end_list = tmp;
		for (unsigned int i = 0; i < count-1; ++i)
		{
			NodePtr tmp1= alloc.allocate(1);
			tmp1 = new(tmp1) Node(i_val);
			tmp->Next= tmp1;
			end_list = tmp1;
			tmp = tmp1;
		}
	}

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

	size_type size() const
	{
		return MySize;
	}

	iterator begin()
	{
		return iterator(*start_list);
	}

	const_iterator cbegin() const
	{
		return const_iterator(*start_list);
	}

	iterator end() const
	{
		return iterator(*(end_list->Next));
	}

	const_iterator cend() const
	{
		return const_iterator(*end_list);
	}

	bool empty() const
	{
		return begin()==end();
	}

	void push_back( const T& i_val )
	{
		NodePtr tmp = alloc.allocate(1);
		tmp = new(tmp) Node(i_val);
		(*end_list).Next = tmp;
		end_list = tmp;
		if (start_list == NULL)
			start_list = end_list;
		++MySize;
	}

	void push_front( const T& i_val )
	{
		NodePtr tmp = alloc.allocate(1);
		tmp = new(tmp) Node(i_val);
		tmp->Next = start_list;
		start_list = tmp;
		++MySize;
		if(MySize == 1)
			end_list = start_list;
	}

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

	void pop_front()
	{
		NodePtr tmp_erase = start_list;
		start_list = start_list->Next;
		tmp_erase->~SListNode<T>();
		alloc.deallocate(tmp_erase,1);
		--MySize;
	}

	reference front()
	{
		return start_list->val;
	}

	const_reference front() const
	{
		return const *(start_list->val);
	}

	reference back()
	{
		return end_list->val;
	}

	const_reference back() const
	{
		return end_list->val;
	}

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
			NodePtr tmp = start_list;
			while (i_pos.cur != tmp->Next)
			{
				tmp = tmp->Next;
			}

			NodePtr tmp1 = alloc.allocate(1);
			tmp1 = new(tmp1) Node(i_val);
			tmp1->Next = tmp->Next;
			tmp->Next = tmp1;
			++MySize;

			return iterator(*(tmp1->Next));
		}
	}

	iterator insert( iterator i_pos, size_type i_count, const T& i_val )
	{
		for (unsigned int i=0;i < i_count ; ++i)
			insert(i_pos,i_val);
		return i_pos;
	}

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
			//if(i_pos.cur == end_list)
			//	end_list = tmp_strart;
			return iterator (*(tmp_strart->Next));
		}
	}

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

	void merge( SList& i_other )
	{
		for (iterator it = i_other.begin();it != i_other.end();++it)
			push_back(*it);
	}

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
	}
	return true;
}

template< typename T, typename Alloc >
bool operator!=( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	return !(lhs==rhs);
}

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
	return !(lhs<rhs);
}
template< typename T, typename Alloc >
bool operator>=( SList<T,Alloc> &lhs, SList<T,Alloc> &rhs)
{
	return lhs == rhs || lhs > rhs;
}