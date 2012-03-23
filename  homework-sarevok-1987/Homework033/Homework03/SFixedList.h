#include "SListNode.h"

template < typename T, unsigned int N, typename A = std::allocator<T>>
class FixedSList;

template < typename T, unsigned int N>
class ConstFixedSListIterator
{

	int cur;
	SListNodeInt<T> *m_data;
	friend FixedSList<T,N>;

public:

	typedef typename const T value_type;
	typedef typename std::ptrdiff_t difference_type;
	typedef typename const T* pointer;
	typedef typename const T& reference;
	typedef typename std::forward_iterator_tag iterator_category;

	ConstFixedSListIterator()
	{
		cur = -1;
	}

	explicit ConstFixedSListIterator(const int& i_node, SListNodeInt<T> * i_data)
	{
		cur = i_node;
		m_data = &i_data;
	}

	ConstFixedSListIterator(const ConstFixedSListIterator& i_copy)
	{
		cur = i_copy.cur;
		m_data = i_copy.m_data;
	}

	ConstFixedSListIterator& operator ++()
	{
		cur = (*m_data)[cur].Next;
		return (*this);
	}

	ConstFixedSListIterator operator ++(int)
	{
		FixedSListIterator tmp = (*this);
		++(*this);
		return tmp;
	}

	reference operator *()
	{
		return (*m_data)[cur].val;
	}

	pointer operator ->()
	{
		return &((*m_data)[cur].val);
	}
	bool operator==(const ConstFixedSListIterator<T,N>& i_slist2) const
	{
		return (*this).cur == i_slist2.cur;
	}
	bool operator!=(const ConstFixedSListIterator<T,N>& i_slist2) const
	{
		return !((*this)==i_slist2);
	}
};

template < typename T, unsigned int N>
class FixedSListIterator
{
	int cur;
	SListNodeInt<T> *m_data;
	friend FixedSList<T,N>;

public:
	
	typedef typename T value_type;
	typedef typename std::ptrdiff_t difference_type;
	typedef typename T* pointer;
	typedef typename T& reference;
	typedef typename std::forward_iterator_tag iterator_category;

	FixedSListIterator()
	{
		cur = -1;
	}

	explicit FixedSListIterator(const int& i_node, SListNodeInt<T>& i_data)
	{
		cur = i_node;
		m_data = &i_data;
	}

	FixedSListIterator(const FixedSListIterator& i_copy)
	{
		cur = i_copy.cur;
		m_data = i_copy.m_data;
	}

	FixedSListIterator& operator ++()
	{
		cur = m_data[cur].Next;
		return (*this);
	}

	FixedSListIterator operator ++(int)
	{
		FixedSListIterator tmp = (*this);
		++(*this);
		return tmp;
	}

	reference operator *()
	{
		return m_data[cur].val;
	}

	pointer operator ->()
	{
		return &(m_data[cur].val);
	}

	bool operator==(const FixedSListIterator<T,N>& i_slist2) const
	{
		return (*this).cur == i_slist2.cur;
	}
	bool operator!=(const FixedSListIterator<T,N>& i_slist2) const
	{
		return !((*this)==i_slist2);
	}
};

template < typename T, unsigned int N, typename A >
class FixedSList
{
	
	int start_list;
	int end_list;
	int free_list;
	SListNodeInt<T> m_data[N];
	int capacity;

	typedef typename SListNodeInt<T>* NodePtr;
	typedef typename SListNodeInt<T> Node;

	A alloc;

	size_t MySize;

	void create_free_list()
	{
		for( int i=0; i < capacity; ++i)
		{
			m_data[i].val = static_cast<T>(0);
			m_data[i].Next = i+1;
		}
		free_list=0;
	}

public:

	typedef typename T value_type;
	typedef typename A allocator_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename T* pointer;
	typedef const T* const_pointer;

	typedef typename FixedSListIterator<T,N> iterator;
	typedef typename ConstFixedSListIterator<T,N> const_iterator;

	explicit FixedSList( const A& a = A() ):alloc(a),MySize(0)
	{
		create_free_list();
		end_list = start_list = -1;
	}

	explicit FixedSList( size_type count ):MySize(count),capacity(N)
	{
		create_free_list();
		for (unsigned int i = 0; i < count; ++i)
			m_data[i].val = static_cast<T>(0);
		start_list = 0;
		end_list = count-1;
		free_list = count;
		m_data[end_list].Next=-1;
	}

	explicit FixedSList( size_type count, const T& i_val, const A& a = A()): alloc(a), MySize(count),capacity(N)
	{
		create_free_list();
		T tmp = i_val;
		for (unsigned int i = 0; i < count; ++i)
			m_data[i].val = i_val;
		start_list = 0;
		end_list = count-1;
		free_list = count;
		m_data[end_list].Next=-1;
	}

	//template <typename InputIterator>
	//FixedSList( InputIterator first, InputIterator last, const A& a = A() ) : alloc(a),MySize(0)
	//{

	//}

	~FixedSList()
	{
	
	}

	size_type size() const
	{
		return MySize;
	}

	iterator begin()
	{
		return iterator(start_list,*m_data);
	}

	const_iterator cbegin() const
	{

	}

	iterator end()
	{
		return iterator(-1,*m_data);
	}

	const_iterator cend() const
	{

	}

	bool empty() const
	{
		return MySize==0;
	}

	void push_back( const T& i_val )
	{
		end_list = m_data[end_list].Next = free_list;
		free_list = m_data[free_list].Next;
		m_data[end_list].val = i_val;
		m_data[end_list].Next = -1;
		++MySize;
	}

	void push_front( const T& i_val )
	{
		int tmp = free_list;
		free_list = m_data[free_list].Next;
		m_data[tmp].Next = start_list;
		start_list = tmp;
		m_data[start_list].val = i_val;
		++MySize;
	}

	void pop_back()
	{
		int i=0;
		while ( m_data[i].Next != end_list )
			i++;
		m_data[end_list].Next = free_list;
		free_list = end_list;
		m_data[i].Next = -1;
		end_list = i;
		--MySize;
	}

	void pop_front()
	{
		int tmp = start_list;
		start_list = m_data[start_list].Next;
		m_data[tmp].Next = free_list;
		free_list = tmp;
		--MySize;
	}

	reference front()
	{
		return m_data[start_list].val;
	}

	const_reference front() const
	{
		return const m_data[start_list].val;
	}

	reference back()
	{
		return m_data[end_list].val;
	}

	const_reference back() const
	{
		return const m_data[end_list].val;
	}

	iterator insert( iterator i_pos, const T& i_val )
	{
		if(i_pos.cur == start_list)
		{
			push_front(i_val);
			return iterator(m_data[start_list].Next, *m_data);
		}
		else if(i_pos.cur == end_list)
		{
			push_back(i_val);
			return iterator(m_data[end_list].Next, *m_data);
		}
		else
		{
			int i=0;
			while ( m_data[i].Next != i_pos.cur )
				i++;
			int tmp = free_list;
			free_list = m_data[free_list].Next;
			m_data[tmp].Next = i_pos.cur;
			m_data[i].Next = tmp;
			m_data[tmp].val=i_val;
			++MySize;
			return iterator(m_data[tmp].Next, *m_data);
		}
	}

	iterator insert( iterator i_pos, size_type i_count, const T& i_val )
	{
		for(unsigned int i=0; i < i_count-1; ++i)
			insert(i_pos,i_val);
		return insert(i_pos,i_val);
	}

	template< typename InputIterator >
	iterator insert( InputIterator first, InputIterator last, iterator i_pos)
	{
		iterator it
		while(first!= last)
		{
			it=insert(i_pos,*first);
			++first;
		}
		return it;
	}

	void clear()
	{
		m_data[end_list].Next = free_list;
		free_list = start_list;
		MySize=0;
		start_list=end_list=-1;
	}

	iterator erase( iterator i_pos )
	{
		if(i_pos.cur == start_list)
		{
			pop_front();
			return iterator(m_data[start_list].Next, *m_data);
		}
		else if(i_pos.cur == end_list)
		{
			pop_back();
			return iterator(m_data[end_list].Next, *m_data);
		}
		else
		{
			int i=0;
			while ( m_data[i].Next != i_pos.cur )
				i++;
			m_data[free_list].Next = i_pos.cur;
			m_data[i].Next = m_data[i_pos.cur].Next;
			free_list = i_pos.cur;
			--MySize;
			return iterator(m_data[i].Next, *m_data);
		}
	}

	iterator erase( iterator first, iterator last )
	{
		iterator it;
		while(first!=last)
		{
			it = erase(first);
			++first;
		}
		return it;
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

	void merge( FixedSList& i_other )
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

	void swap( FixedSList& i_other )
	{
		std::swap(m_data,i_other.m_data);
		size_type swp_size = MySize;
		MySize = i_other.MySize;
		i_other.MySize = swp_size;
	}
};


template< typename T, unsigned int N >
bool operator==( FixedSList<T,N> &lhs, FixedSList<T,N> &rhs)
{
	if(lhs.size() != rhs.size())
		return false;
	SList<T,N>::iterator it = lhs.begin();
	SList<T,N>::iterator it1 = rhs.begin();

	while(it != lhs.end() && it != rhs.end())
	{
		if(*it!=*it1)
			return false;
	}
	return true;
}

template< typename T, unsigned int N >
bool operator!=( FixedSList<T,N> &lhs, FixedSList<T,N> &rhs)
{
	return !(lhs==rhs);
}

template< typename T, unsigned int N >
bool operator<( FixedSList<T,N> &lhs, FixedSList<T,N> &rhs)
{
	return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}
template< typename T, unsigned int N >
bool operator<=( FixedSList<T,N> &lhs, FixedSList<T,N> &rhs)
{
	return (lhs == rhs) || (lhs < rhs);
}

template< typename T, unsigned int N >
bool operator>( FixedSList<T,N> &lhs, FixedSList<T,N> &rhs)
{
	return !(lhs<rhs);
}
template< typename T, unsigned int N >
bool operator>=( FixedSList<T,N> &lhs, FixedSList<T,N> &rhs)
{
	return lhs == rhs || lhs > rhs;
}