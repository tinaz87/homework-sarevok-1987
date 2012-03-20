#pragma once
#include "AllocatorInterfaces.h"

template < typename Allocator = AllocatorInterfaces<> >
class ObjectAllocatorInterface
{
public:
	typedef typename Allocator::ChunkType ChunkType;
	typedef typename Allocator::PtrChunkType PtrChunkType;
	typedef typename Allocator::CPtrChunkType CPtrChunkType;
	typedef typename Allocator::RefChunkType RefChunkType;
	typedef typename Allocator::CRefChunkType CRefChunkType;

	typedef typename Allocator::AllocatorType AllocatorType;
	typedef typename Allocator::PtrAllocatorType PtrAllocatorType;

	typedef typename ObjectAllocatorInterface<AllocatorType> ObjectAllocatorType;
	typedef typename ObjectAllocatorInterface<AllocatorType>* PtrObjectAllocatorType;

	virtual void * Allocate( std::size_t size) = 0;
	virtual void Deallocate( void * p ) = 0;
	virtual void Deallocate( void * p, std::size_t i_numBytes ) = 0;
	virtual void Deallocate() =0;
};



template <typename Allocator = Stack_Allocator<>>
class Stack_Object_Allocator : public ObjectAllocatorInterface<Allocator>
{
public:
	typedef typename Allocator::ChunkType ChunkType;
	typedef typename Allocator::PtrChunkType PtrChunkType;
	typedef typename Allocator::CPtrChunkType CPtrChunkType;
	typedef typename Allocator::RefChunkType RefChunkType;
	typedef typename Allocator::CRefChunkType CRefChunkType;

	typedef typename Allocator::AllocatorType AllocatorType;
	typedef typename Allocator::PtrAllocatorType PtrAllocatorType;

	typedef typename Stack_Object_Allocator<AllocatorType> ObjectAllocatorType;
	typedef typename Stack_Object_Allocator<AllocatorType>* PtrObjectAllocatorType;

	Stack_Object_Allocator( std::size_t i_StackSize = 48000000)
		: m_pool( nullptr )
	{
		m_pool = new AllocatorType();//allocCount;
		m_pool->Initialize(i_StackSize);
	}

	void * Allocate( std::size_t i_numBytes)
	{
		Allocator& allocator = m_pool[0];
		void * place = allocator.Allocate(i_numBytes);
		assert(place != nullptr);

		return place;
	}

	void Deallocate( void * p )
	{
		Allocator& allocator = m_pool[0];
		allocator.Deallocate(p , allocator.HasBlock(p));
	}

	void Deallocate( void * p, std::size_t i_numBytes )
	{

	}

	void Deallocate( void )
	{
		Allocator& allocator = m_pool[0];
		allocator.Deallocate();
	}
	
	~Stack_Object_Allocator()
	{

	}

private:

	AllocatorType* m_pool;
};

template<typename Allocator = Fixed_Allocator<>>
class Small_Object_Allocator : public ObjectAllocatorInterface<Allocator>
{
public:
	typedef typename Allocator::ChunkType ChunkType;
	typedef typename Allocator::PtrChunkType PtrChunkType;
	typedef typename Allocator::CPtrChunkType CPtrChunkType;
	typedef typename Allocator::RefChunkType RefChunkType;
	typedef typename Allocator::CRefChunkType CRefChunkType;

	typedef typename Allocator::AllocatorType AllocatorType;
	typedef typename Allocator::PtrAllocatorType PtrAllocatorType;

	typedef typename Small_Object_Allocator<AllocatorType> ObjectAllocatorType;
	typedef typename Small_Object_Allocator<AllocatorType>* PtrObjectAllocatorType;

	Small_Object_Allocator( std::size_t i_pageSize = 4096, std::size_t i_maxObjectSize = 256, std::size_t i_objectAlignSize = 4)
		: m_pool( nullptr )
		, m_maxObjectSize( i_maxObjectSize )
		, m_objectAlignSize( i_objectAlignSize )
	{
		assert( 0 != i_objectAlignSize );
		const std::size_t allocCount = GetOffset( i_maxObjectSize, i_objectAlignSize );
		m_pool = new AllocatorType[allocCount];
		for ( std::size_t i = 0; i < allocCount; ++i )
			m_pool[i].Initialize( ( i+1 ) * i_objectAlignSize, i_pageSize);
	}

	void * Allocate( std::size_t i_numBytes)
	{
		//if ( i_numBytes > GetMaxObjectSize() )
		//	return DefaultAllocator( i_numBytes);

		assert( nullptr != m_pool );
		if ( 0 == i_numBytes ) i_numBytes = 1;
		const ::std::size_t index = GetOffset( i_numBytes, GetAlignment() ) - 1;
		const ::std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
		(void) allocCount;
		assert( index < allocCount );

		Allocator& allocator = m_pool[ index ];
		assert( allocator.BlockSize() >= i_numBytes );
		assert( allocator.BlockSize() < i_numBytes + GetAlignment() );
		void * place = allocator.Allocate();

		if ( ( nullptr == place ) )//&& TrimExcessMemory() )
			place = allocator.Allocate();

		return place;
	}

	void Deallocate( void * p )
	{
		if ( nullptr == p )
			return;
		assert( nullptr != m_pool );
		Allocator * pAllocator = nullptr;
		const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
		Allocator::PtrChunkType chunk = nullptr;

		for ( std::size_t ii = 0; ii < allocCount; ++ii )
		{
			chunk = m_pool[ ii ].HasBlock( p );
			if ( nullptr != chunk )
			{
				pAllocator = &m_pool[ ii ];
				break;
			}
		}
		if ( nullptr == pAllocator )
		{
			//DefaultDeallocator( p );
			return;
		}

		assert( nullptr != chunk );
		const bool found = pAllocator->Deallocate( p, chunk );
		(void) found;
		assert( found );
	}

	void Deallocate( void * p, std::size_t i_numBytes )
	{
		if ( nullptr == p )
			return;
		//if ( numBytes > GetMaxObjectSize() )
		//{
		//	DefaultDeallocator( p );
		//	return;
		//}
		assert( nullptr != m_pool );
		if ( 0 == i_numBytes ) i_numBytes = 1;
		const std::size_t index = GetOffset( i_numBytes, GetAlignment() ) - 1;
		const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
		(void) allocCount;
		assert( index < allocCount );
		Allocator& allocator = m_pool[ index ];
		assert( allocator.BlockSize() >= i_numBytes );
		assert( allocator.BlockSize() < i_numBytes + GetAlignment() );
		const bool found = allocator.Deallocate( p, nullptr );
		(void) found;
		assert( found );
	}

	void Deallocate( )
	{
		return;
	}

	~Small_Object_Allocator( void )
	{
		delete [] m_pool;
	}

private:

	std::size_t GetOffset( std::size_t i_numBytes, std::size_t i_alignment )
	{
		const std::size_t alignExtra = i_alignment-1;
		return ( i_numBytes + alignExtra ) / i_alignment;
	}

	std::size_t GetAlignment() const
	{
		return m_objectAlignSize;
	}

	std::size_t GetMaxObjectSize() const
	{
		return m_maxObjectSize;
	}

	const std::size_t m_maxObjectSize;
	/// Size of alignment boundaries.
	const std::size_t m_objectAlignSize;
	AllocatorType* m_pool;
};
