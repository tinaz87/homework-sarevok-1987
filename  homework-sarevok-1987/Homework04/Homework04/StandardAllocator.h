#include "MemoryManager.h"

template<typename T >
class StandardAllocator
{
public:
	typedef typename T value_type;
	typedef typename T* pointer;
	typedef typename const T* const_pointer;
	typedef typename T& reference;
	typedef typename const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	StandardAllocator() throw()
	{

	}

	StandardAllocator ( const StandardAllocator& i_copy) throw() : BigObjAllocator(i_copy.BigObjAllocator)
	{

	}

	template <class U>
	StandardAllocator ( const StandardAllocator<U>& i_copy) throw() : BigObjAllocator(i_copy.BigObjAllocator)
	{

	}
	
	pointer address ( reference x ) const
	{
		return &x
	}
	const_pointer address ( const_reference x ) const
	{
		return const &x;
	}

	pointer allocate (size_type n,  const_pointer hint=0)
	{
		assert(n >= 1);
		if(sizeof(T) * n > 255)
			return BigObjAllocator.allocate(n);
		return MemoryManager::newObjectCategory<value_type,SmallObject_Category<>>(n);
	}

	void deallocate (pointer p, size_type n)
	{
		if(sizeof(T)*n > 255)
			return BigObjAllocator.deallocate(p,n);
		MemoryManager::deleteObjectCategory<SmallObject_Category<> >(p);
	}

	void construct ( pointer p, const_reference val )
	{
		new ((void*)p) T (val);
	}

	void destroy (pointer p)
	{
		((pointer)p)->~value_type();
	}

	~StandardAllocator() throw()
	{

	}

	template<class _Other>
	struct rebind {

		typedef StandardAllocator<_Other> other;

	};


	//size_type max_size() const
	//{	// return maximum possible length of sequence
	//	return (this->_Alval.max_size());
	//}

	size_type max_size() const{

		return BigObjAllocator.max_size();

	}
//private:

	std::allocator<T> BigObjAllocator;
};