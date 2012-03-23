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
	typedef CategoryInterfaces<>* CategoryArrayType;
	typedef std::vector<CategoryArrayType> CategoryVector;

	CategoryVector m_ArrayCategory;
public:

	MemoryManagerObj(): m_ArrayCategory()
	{

	}

	template< typename Category>
	void* mallocObjCategory(std::size_t size)
	{
		Category i_Category;
		bool noContains = true;
		for(CategoryVector::iterator i = m_ArrayCategory.begin(); i != m_ArrayCategory.end(); ++i)
		{
			if(*i == dynamic_cast<CategoryInterfaces<>*>(&i_Category))
				noContains = false;
		}
		if(noContains)
			m_ArrayCategory.push_back(dynamic_cast<CategoryInterfaces<>*>(&i_Category));
		return i_Category.Allocate(size);
	}

	void * mallocObject(std::size_t size)
	{
		//SmallObject_Category<> dcategory;
		return malloc(size);//dcategory.Allocate(size);
	}

	template< typename Category>
	void freeObjectCategory(void *p)
	{
		Category i_Category;
		return i_Category.Deallocate(p);
	}

	void freeObject(void *p)
	{
		free(p);
		//for(CategoryVector::iterator i = m_ArrayCategory.begin(); i != m_ArrayCategory.end(); ++i)
			//(*i)->Deallocate(p);
		return;
	}

	template< typename Category>
	void freeObjectCategory()
	{
		Category i_Category;
		return i_Category.Deallocate();
	}


/*
	void freeObject()
	{
		for(CategoryVector::iterator i = m_ArrayCategory.begin(); i != m_ArrayCategory.end(); ++i)
			(*i)->Deallocate();
		return;
	}*/
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

	/*static void freeObject()
	{
		return MM::Instance().freeObject();
	}*/

	template< typename Category>
	static void deleteObjectCategory()
	{
		return MM::Instance().freeObjectCategory<Category>();
	}
	/*
	static void deleteObject()
	{
		return MM::Instance().freeObject();
	}*/
};

#pragma endregion MemoryManager