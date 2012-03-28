#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "ChunkInterfaces.h"
#include "AllocatorInterfaces.h"
#include "ObjectAllocatorInterfaces.h"
#include "CategoryInterfaces.h"
#include "Singleton.h"
#include <assert.h>

#pragma region MemoryManagerOBJ

class MemoryManagerObj
{
	typedef std::map<void*,void*> AddressMap;
	typedef std::pair<void*,void*> MapElem;

	AddressMap m_AlignedAddress;

public:

	MemoryManagerObj(): m_AlignedAddress()
	{

	}

	template< typename Category>
	void* mallocObjCategory(std::size_t size)
	{
		Category i_Category;
		return i_Category.Allocate(size);
	}

	void * mallocObject(std::size_t size)
	{
		return malloc(size);
	}

	void * mallocObjectAlgnment(std::size_t size,std::size_t align)
	{
		assert((align!=0) && (align & (align - 1) == 0));
		void *startAdd = malloc(size + align - 1);
		unsigned int rest = reinterpret_cast<unsigned int>(startAdd) % align;
		assert((reinterpret_cast<unsigned char*>(startAdd) + (align - rest) + size) < (reinterpret_cast<unsigned char*>(startAdd) + size + (align - 1)));
		if(rest == 0)
			m_AlignedAddress.insert(MapElem(reinterpret_cast<unsigned char*>(startAdd) + (align - rest),startAdd));
		return reinterpret_cast<unsigned char*>(startAdd) + (align - rest);
	}

	template< typename Category>
	void freeObjectCategory(void *p)
	{
		Category i_Category;
		return i_Category.Deallocate(p);
	}

	void freeObject(void *p)
	{
		AddressMap::iterator findIt = m_AlignedAddress.find(p);
		if(findIt != m_AlignedAddress.end())
		{
			free(m_AlignedAddress[p]);
		}
		free(p);
		return;
	}

	template< typename Category>
	void freeObjectCategory()
	{
		Category i_Category;
		return i_Category.Deallocate();
	}
};

#pragma endregion MemoryManagerOBJ


#pragma region MemoryManagerSingleton

class MemoryManagerSingleton : public MemoryManagerObj
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

class MemoryManager
{
	typedef MemoryManagerSingleton MM;

public:

	template< typename Category>
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
		MM::Instance().mallocObjectAlgnment(i_size,i_algn);
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