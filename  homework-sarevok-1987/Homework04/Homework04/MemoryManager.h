#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "ChunkInterfaces.h"
#include "AllocatorInterfaces.h"
#include "ObjectAllocatorInterfaces.h"
#include "CategoryInterfaces.h"
#include "Singleton.h"
#include "ThreadSafeInterfaces.h"
#include <assert.h>

#pragma region MemoryManagerOBJ
template < typename ThreadPolicy >
class MemoryManagerObj
{
	typedef std::map<void*,void*> AddressMap;
	typedef std::pair<void*,void*> MapElem;

	AddressMap m_AlignedAddress;

	ThreadPolicy mThreadSafety;
public:

	MemoryManagerObj(): m_AlignedAddress()
	{

	}

	template< typename Category>
	void* mallocObjCategory(std::size_t size)
	{
		mThreadSafety.Lock();
		Category i_Category;
		void *tmp = i_Category.Allocate(size);
		mThreadSafety.Unlock();
		return tmp;
	}

	void * mallocObject(std::size_t size)
	{
		return malloc(size);
	}

	void * mallocObjectAlgnment(std::size_t size,std::size_t align)
	{
		assert((align!=0) && ((align & (align - 1)) == 0));
		mThreadSafety.Lock();
		void *startAdd = malloc(size + align - 1);
		unsigned int rest = reinterpret_cast<unsigned int>(startAdd) % align;
		if(rest != 0)
		{
			assert((reinterpret_cast<unsigned char*>(startAdd) + (align - rest) + size) < (reinterpret_cast<unsigned char*>(startAdd) + (size + (align - 1))));
			m_AlignedAddress.insert(MapElem(reinterpret_cast<unsigned char*>(startAdd) + (align - rest),startAdd));
			mThreadSafety.Unlock();
			return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(startAdd) + (align - rest));
		}
		else
		{
			mThreadSafety.Unlock();
			return startAdd;
		}
		
	}
	template< typename Category>
	void * mallocObjectCategoryAlgnment(std::size_t size,std::size_t align)
	{
		assert((align!=0) && ((align & (align - 1)) == 0));
		mThreadSafety.Lock();
		Category i_Category;
		void *startAdd = i_Category.Allocate(size + align - 1);
		unsigned int rest = reinterpret_cast<unsigned int>(startAdd) % align;
		if(rest != 0)
		{
			m_AlignedAddress.insert(MapElem(reinterpret_cast<unsigned char*>(startAdd) + (align - rest),startAdd));
			assert((reinterpret_cast<unsigned char*>(startAdd) + (align - rest) + size) < (reinterpret_cast<unsigned char*>(startAdd) + (size + (align - 1))));
			mThreadSafety.Unlock();
			return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(startAdd) + (align - rest));
		}
		else
		{
			mThreadSafety.Unlock();
			return startAdd;
		}
		
	}

	template< typename Category>
	void freeObjectCategory(void *p)
	{
		Category i_Category;
		mThreadSafety.Lock();
		AddressMap::iterator findIt = m_AlignedAddress.find(p);
		if(findIt != m_AlignedAddress.end())
		{
			i_Category.Deallocate(m_AlignedAddress[p]);
			m_AlignedAddress.erase(findIt);
		}
		else
			i_Category.Deallocate(p);
		mThreadSafety.Unlock();
	}

	void freeObject(void *p)
	{
		mThreadSafety.Lock();
		AddressMap::iterator findIt = m_AlignedAddress.find(p);
		if(findIt != m_AlignedAddress.end())
		{
			free(findIt->second);
			m_AlignedAddress.erase(findIt);
		}
		else
			free(p);
		mThreadSafety.Unlock();
		return;
	}

	template< typename Category>
	void freeObjectCategory()
	{
		mThreadSafety.Lock();
		Category i_Category;
		i_Category.Deallocate();
		mThreadSafety.Unlock();
	}
};

#pragma endregion MemoryManagerOBJ


#pragma region MemoryManagerSingleton
template < typename ThreadPolicy >
class MemoryManagerSingleton : public MemoryManagerObj<ThreadPolicy>
{
	typedef SingletonHolder<MemoryManagerSingleton> allocator;

	MemoryManagerSingleton( const MemoryManagerSingleton & );

	MemoryManagerSingleton & operator = ( const MemoryManagerSingleton & );

public:

	static MemoryManagerSingleton& Instance( void )
	{
		return allocator::Instance();
	}

	MemoryManagerSingleton() : MemoryManagerObj()
	{
	
	}

	/// The destructor is not meant to be called directly.
	~MemoryManagerSingleton( void )
	{

	}
};

#pragma endregion MemoryManagerSingleton

#pragma region MemoryManager
template< typename ThreadPolicy =  WinThreadSafe>
class MemoryManager
{
	typedef MemoryManagerSingleton<ThreadPolicy> MM;

public:

	template< typename Category >
	static void * mallocObjCategory(std::size_t size)
	{
		return MM::Instance().mallocObjCategory<Category>(size);
	}

	static void * mallocObject(std::size_t size)
	{
		return MM::Instance().mallocObject(size);
	}

	static void * mallocObjAlignment(std::size_t i_size,std::size_t i_algn)
	{
		return MM::Instance().mallocObjectAlgnment(i_size,i_algn);
	}
	template < typename Category >
	static void * mallocObjCategoryAlignment(std::size_t i_size,std::size_t i_algn)
	{
		return MM::Instance().mallocObjectCategoryAlgnment<Category>(i_size,i_algn);
	}

	template<typename Type, typename Category>
	static Type* newObjectCategory(std::size_t size)
	{
		return static_cast<Type*>(MM::Instance().mallocObjCategory<Category>(size * sizeof(Type)));
	}

	template<typename Type>
	static Type* newObject(std::size_t size)
	{
		return static_cast<Type*>( operator new (size) ); //static_cast<Type*>(MM::Instance().mallocObject(size * sizeof(Type)));
	}

	template< typename Type, typename Category>
	static Type* newObjectArray(std::size_t size)
	{
		return static_cast<Type*>(MM::Instance().mallocObjCategory<Category>(size * sizeof(Type)));
	}

	template < typename Type >
	static Type* newObjectArray(std::size_t size)
	{
		return static_cast<Type*>(MM::Instance().mallocObject(size * sizeof(Type)));
	}

	template< typename Category>
	static void freeObjectCategory(void *p)
	{
		return MM::Instance().freeObjectCategory<Category>(p);
	}

	static void freeObject(void *p)
	{
		return MM::Instance().freeObject(p);
	}

	template< typename Category>
	static void deleteObjectCategory(void* p)
	{
		return MM::Instance().freeObjectCategory<Category>(p);
	}

	static void deleteObject(void* p)
	{
		return delete p; //MM::Instance().freeObject(p);
	}

	template< typename Category >
	static void deleteObjectArrayCategory(void* p)
	{
		return MM::Instance().freeObjectCategory<Category>(p);
	}

	static void deleteObjectArray(void* p)
	{
		return MM::Instance().freeObject(p);
	}

	template< typename Category>
	static void freeObjectCategory()
	{
		return MM::Instance().freeObjectCategory<Category>();
	}

	template< typename Category>
	static void deleteObjectCategory()
	{
		return MM::Instance().freeObjectCategory<Category>();
	}
};

#pragma endregion MemoryManager