#include <vector>
#include "SListNode.h"

template < typename T, typename A = std::allocator<T>>
class SListArray;

template < typename T>
class ConstSListArrayIterator
{

	int cur;
	const std::vector<SListNodeInt<T>> *m_data;
	friend SListArray<T>;
public:
	typedef typename const T value_type;
	typedef typename std::ptrdiff_t difference_type;
	typedef typename const T* pointer;
	typedef typename const T& reference;
	typedef typename std::forward_iterator_tag iterator_category;

	ConstSListArrayIterator()
	{
		cur = -1;
	}

	explicit ConstSListArrayIterator(const int& i_node, std::vector<SListNodeInt<T>>& i_data)
	{
		cur = i_node;
		m_data = &i_data;
	}

	ConstSListArrayIterator(const ConstSListArrayIterator& i_copy)
	{
		cur = i_copy.cur;
		m_data = i_copy.m_data;
	}

	ConstSListArrayIterator& operator ++()
	{
		cur = (*m_data)[cur].Next;
		return (*this);
	}

	ConstSListArrayIterator operator ++(int)
	{
		SListArrayIterator tmp = (*this);
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
	bool operator==(const ConstSListArrayIterator<T>& i_slist2) const
	{
		return (*this).cur == i_slist2.cur;
	}
	bool operator!=(const ConstSListArrayIterator<T>& i_slist2) const
	{
		return !((*this)==i_slist2);
	}
};

template < typename T>
class SListArrayIterator
{

	int cur;
	std::vector<SListNodeInt<T>> *m_data;
	friend SListArray<T>;

public:

	typedef typename T value_type;
	typedef typename std::ptrdiff_t difference_type;
	typedef typename T* pointer;
	typedef typename T& reference;
	typedef typename std::forward_iterator_tag iterator_category;

	SListArrayIterator()
	{
		cur = -1;
	}

	explicit SListArrayIterator(const int& i_node, std::vector<SListNodeInt<T>>& i_data)
	{
		cur = i_node;
		m_data = &i_data;
	}

	SListArrayIterator(const SListArrayIterator& i_copy)
	{
		cur = i_copy.cur;
		m_data = i_copy.m_data;
	}

	SListArrayIterator& operator ++()
	{
		cur = (*m_data)[cur].Next;
		return (*this);
	}

	SListArrayIterator operator ++(int)
	{
		SListArrayIterator tmp = (*this);
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

	bool operator==(const SListArrayIterator<T>& i_slist2) const
	{
		return (*this).cur == i_slist2.cur;
	}
	bool operator!=(const SListArrayIterator<T>& i_slist2) const
	{
		return !((*this)==i_slist2);
	}
};

template < typename T, typename A >
class SListArray
{
public:
	A alloc;

	typedef typename SListNodeInt<T>* NodePtr;
	typedef typename SListNodeInt<T> Node;

	size_t MySize;

	int start_list;
	int end_list;

	std::vector<SListNodeInt<T>> m_data;
public:

	typedef typename T value_type;
	typedef typename A allocator_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename T* pointer;
	typedef const T* const_pointer;

	typedef typename SListArrayIterator<T> iterator;
	typedef typename ConstSListIterator<T> const_iterator;

	explicit SListArray( const A& a = A() ):alloc(a),MySize(0)
	{
		end_list = start_list = -1;
	}

	explicit SListArray( size_type count ):MySize(count)
	{
		for (unsigned int i = 0; i < count; ++i)
			m_data.push_back(*(new Node(static_cast<T>(0),m_data.size()+1)));
		start_list = 0;
		end_list = m_data.size()-1;
		m_data[end_list].Next=-1;
	}

	explicit SListArray( size_type count, const T& i_val, const A& a = A()): alloc(a), MySize(count)
	{
		for (unsigned int i = 0; i < count; ++i)
			m_data.push_back(*(new Node(i_val,m_data.size()+1)));
		start_list = 0;
		end_list = m_data.size()-1;
		m_data[end_list].Next=-1;
	}

	//template <typename InputIterator>
	//SListArray( InputIterator first, InputIterator last, const A& a = A() ) : alloc(a),MySize(0)
	//{
	//	while (first!=last)
	//	{
	//		m_data.push_back(*(new Node(*first,m_data.size()+1)));
	//		++first;
	//	}
	//}

	~SListArray()
	{
		//m_data.std::~vector();
	}

	size_type size() const
	{
		return MySize;
	}

	iterator begin()
	{
		return iterator(start_list,m_data);
	}

	const_iterator cbegin() const
	{
		const_iterator(start_list,m_data);
	}

	iterator end()
	{
		return iterator(-1,m_data);
	}

	const_iterator cend() const
	{
		return const_iterator(-1,m_data);
	}

	bool empty() const
	{
		return MySize==0;
	}

	void push_back( const T& i_val )
	{
		m_data.push_back(*(new Node(i_val,-1)));
		m_data[end_list].Next = m_data.size()-1;
		end_list = m_data.size()-1;
		++MySize;
	}

	void push_front( const T& i_val )
	{
		m_data.push_back(*(new Node(i_val,start_list)));
		start_list = m_data.size()-1;
		++MySize;
	}

	void pop_back()
	{
		int i=0;
		while ( m_data[i].Next != end_list )
			i++;
		m_data[i].Next = -1;
		end_list = i;
		--MySize;
		defrag_vector();
	}

	void pop_front()
	{
		int tmp = m_data[start_list].Next;
		m_data[start_list].Next = -1;
		start_list = tmp;
		--MySize;
		defrag_vector();
	}

	void defrag_vector()
	{
		if((m_data.size() - MySize) >= m_data.size()/2)
		{
			std::vector<SListNodeInt<T>> tmp_vec;
			for (iterator it = begin();it != end();++it)
				tmp_vec.push_back(*(new Node(*it,tmp_vec.size()+1)));
			tmp_vec[tmp_vec.size()-1].Next = -1;
			std::swap(tmp_vec,m_data);
			start_list = 0;
			end_list = m_data.size()-1;
			MySize = m_data.size();
		}
	}

	reference front()
	{
		return m_data[start_list].val;
	}

	const_reference front() const
	{
		return &m_data[start_list].val;
	}

	reference back()
	{
		return m_data[end_list].val;
	}

	const_reference back() const
	{
		return &m_data[end_list].val;
	}

	iterator insert( iterator i_pos, const T& i_val )
	{
		if(i_pos.cur == start_list)
		{
			push_front(i_val);
			return iterator(m_data[start_list].Next, m_data);
		}
		else if(i_pos.cur == end_list)
		{
			push_back(i_val);
			return iterator(m_data[end_list].Next, m_data);
		}
		else
		{
			int i=0;
			while ( m_data[i].Next != i_pos.cur )
				i++;
			m_data.push_back(*(new Node(i_val, i_pos.cur)));
			m_data[i].Next = m_data.size() - 1;
			++MySize;
			return iterator(m_data.size() - 1, m_data);
		}
	}

	iterator insert( iterator i_pos, size_type i_count, const T& i_val )
	{
		iterator it;
		for(unsigned int i = 0; i <  i_count; ++i)
		{
			it = insert(i_pos, i_val);
		}
		return it;
	}

	template< typename InputIterator >
	iterator insert( InputIterator first, InputIterator last, iterator i_pos)
	{
		iterator it;
		while(first != last)
		{
			it = insert(i_pos, *first);
			++first;
		}
		return it;
	}

	void clear()
	{
		m_data.clear();
		start_list = end_list = -1;
		MySize = 0;
	}

	iterator erase( iterator i_pos )
	{
		if(i_pos.cur == start_list)
		{
			pop_front();
			return iterator(i_pos.cur, m_data);
		}
		else if(i_pos.cur == end_list)
		{
			pop_back();
			return iterator(i_pos.cur, m_data);
		}
		else
		{
			int i=0;
			while ( m_data[i].Next != i_pos.cur )
				i++;
			m_data[i].Next = m_data[i_pos.cur].Next;
			m_data[i_pos.cur].Next = -1;
			--MySize;
			defrag_vector();
			return iterator(i_pos.cur, m_data);
		}
	}

	iterator erase( iterator first, iterator last )
	{
		while(first != last)
		{
			erase(first);
			++first;
		}
		return iterator(i_pos.cur, m_data);
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

	void merge( SListArray& i_other )
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

	void swap( SListArray& i_other )
	{
		std::swap(m_data,i_other.m_data);
		size_type swp_size = MySize;
		MySize = i_other.MySize;
		i_other.MySize = swp_size;
	}
};

template< typename T, typename Alloc >
bool operator==( SListArray<T,Alloc> &lhs, SListArray<T,Alloc> &rhs)
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
bool operator!=( SListArray<T,Alloc> &lhs, SListArray<T,Alloc> &rhs)
{
	return !(lhs==rhs);
}

template< typename T, typename Alloc >
bool operator<( SListArray<T,Alloc> &lhs, SListArray<T,Alloc> &rhs)
{
	return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}
template< typename T, typename Alloc >
bool operator<=( SListArray<T,Alloc> &lhs, SListArray<T,Alloc> &rhs)
{
	return (lhs == rhs) || (lhs < rhs);
}

template< typename T, typename Alloc >
bool operator>( SListArray<T,Alloc> &lhs, SListArray<T,Alloc> &rhs)
{
	return !(lhs<=rhs);
}
template< typename T, typename Alloc >
bool operator>=( SListArray<T,Alloc> &lhs, SListArray<T,Alloc> &rhs)
{
	return lhs == rhs || lhs > rhs;
}