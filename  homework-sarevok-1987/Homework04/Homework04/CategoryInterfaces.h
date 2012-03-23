#pragma once
#include "Singleton.h"
#include "ObjectAllocatorInterfaces.h"

template< typename ObjectAllocator = ObjectAllocatorInterface<> >
class CategoryInterfaces
{
public:
	typedef typename ObjectAllocator::AllocatorType AllocatorType;
	typedef typename ObjectAllocator::ObjectAllocatorType ObjectAllocatorType;
	typedef typename ObjectAllocator::PtrObjectAllocatorType PtrObjectAllocatorType;
	typedef typename CategoryInterfaces<ObjectAllocatorType> CategoryType;

	virtual void* Allocate(std::size_t size) = 0;
	virtual void Deallocate(void* p) = 0;
	virtual void Deallocate() = 0;
	virtual ObjectAllocatorType& getAllocatorInstance() = 0;
	virtual ~CategoryInterfaces(){};
};

template <typename ObjAllocator>
class SingletonAllocator : public ObjAllocator
{
	typedef SingletonHolder<SingletonAllocator> allocator;

	SingletonAllocator( const SingletonAllocator & );

	SingletonAllocator & operator = ( const SingletonAllocator & );

	public:

	static SingletonAllocator& Instance( void )
	{
		return allocator::Instance();
	}

	SingletonAllocator() :	ObjAllocator()
	{
	
	}

	/// The destructor is not meant to be called directly.
	~SingletonAllocator( void )
	{

	}
};

//remeber the object allocator must be singleton

//class StackAllocatorSingleton : public Object_Stack_Allocator<>
//{
//	typedef SingletonHolder<StackAllocatorSingleton> allocator;
//
//	StackAllocatorSingleton( const StackAllocatorSingleton & );
//
//	StackAllocatorSingleton & operator = ( const StackAllocatorSingleton & );
//
//public:
//
//	static StackAllocatorSingleton& Instance( void )
//	{
//		return allocator::Instance();
//	}
//
//	StackAllocatorSingleton() :	Object_Stack_Allocator<>(48000000)
//	{
//	
//	}
//
//	/// The destructor is not meant to be called directly.
//	~StackAllocatorSingleton( void )
//	{
//
//	}
//};


template <	typename ObjectAllocator = Stack_Object_Allocator<> >
class Stack_Category : public CategoryInterfaces<ObjectAllocator>
{
public:

	typedef typename ObjectAllocator::AllocatorType AllocatorType;
	typedef typename ObjectAllocator::ObjectAllocatorType ObjectAllocatorType;
	typedef typename ObjectAllocator::PtrObjectAllocatorType PtrObjectAllocatorType;
	typedef typename Stack_Category<ObjectAllocatorType> CategoryType;

	Stack_Category()
	{
		allocator::Instance();
	}

	void* Allocate(std::size_t size)
	{
		return allocator::Instance().Allocate(size);
	}

	void Deallocate(void* p)
	{
		return allocator::Instance().Deallocate(p); 
	}

	void Deallocate()
	{
		return allocator::Instance().Deallocate();
	}

	ObjectAllocatorType& getAllocatorInstance()
	{
		return allocator::Instance();
	}

	virtual ~Stack_Category()
	{
		
	}

public:

	typedef SingletonAllocator<ObjectAllocatorType> allocator;
};

//class AllocatorSingleton : public Default_Object_Allocator<>
//{
//	typedef SingletonHolder<AllocatorSingleton> allocator;
//
//	AllocatorSingleton( const AllocatorSingleton & );
//
//	AllocatorSingleton & operator = ( const AllocatorSingleton & );
//
//public:
//
//	static AllocatorSingleton& Instance( void )
//	{
//		return allocator::Instance();
//	}
//
//	AllocatorSingleton() :	Default_Object_Allocator<>(4096,256,4)
//	{
//	
//	}
//
//	/// The destructor is not meant to be called directly.
//	~AllocatorSingleton( void )
//	{
//
//	}
//};



template< typename ObjectAllocator = Small_Object_Allocator<> >
class SmallObject_Category : public CategoryInterfaces<ObjectAllocator>
{
public:

	typedef typename ObjectAllocator::AllocatorType AllocatorType;
	typedef typename ObjectAllocator::ObjectAllocatorType ObjectAllocatorType;
	typedef typename ObjectAllocator::PtrObjectAllocatorType PtrObjectAllocatorType;
	typedef typename SmallObject_Category<ObjectAllocatorType> CategoryType;

	SmallObject_Category()
	{
		allocator::Instance();
	}

	void* Allocate(std::size_t size)
	{
		return allocator::Instance().Allocate(size);
	}

	void Deallocate(void* p)
	{
		return allocator::Instance().Deallocate(p);
	}

	void Deallocate()
	{
		return;
	}

	ObjectAllocatorType& getAllocatorInstance()
	{
		return allocator::Instance();
	}

	virtual ~SmallObject_Category()
	{
		
	}

public:
	typedef SingletonAllocator<ObjectAllocatorType> allocator;
	//typedef AllocatorSingleton allocator;
};



