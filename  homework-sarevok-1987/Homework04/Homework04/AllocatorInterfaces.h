#pragma once
#include <iostream>
#include <vector>
#include "ChunkInterfaces.h"
#include <assert.h>



template < typename Chunk  = ChunkInterfaces >
class AllocatorInterfaces
{
public:
	AllocatorInterfaces(){};
	typedef typename Chunk::ChunkType ChunkType;
	typedef typename Chunk::PtrChunkType PtrChunkType;
	typedef typename Chunk::CPtrChunkType CPtrChunkType;
	typedef typename Chunk::RefChunkType RefChunkType;
	typedef typename Chunk::CRefChunkType CRefChunkType;

	typedef AllocatorInterfaces<ChunkType> AllocatorType;
	typedef AllocatorInterfaces<ChunkType>* PtrAllocatorType;

	virtual void* Allocate(void) = 0;
	virtual void* Allocate(std::size_t blockSize) =0;
	virtual bool Deallocate( void * p, PtrChunkType hint ) = 0;
	virtual bool Deallocate( void ) = 0;
	virtual ~AllocatorInterfaces(){};
};


template < typename Chunk = Simple_Chunk >
class Stack_Allocator : public AllocatorInterfaces<Chunk>
{
public:

	typedef typename Chunk::ChunkType ChunkType;
	typedef typename Chunk::PtrChunkType PtrChunkType;
	typedef typename Chunk::CPtrChunkType CPtrChunkType;
	typedef typename Chunk::RefChunkType RefChunkType;
	typedef typename Chunk::CRefChunkType CRefChunkType;

	typedef typename Stack_Allocator<ChunkType> AllocatorType;
	typedef typename Stack_Allocator<ChunkType>* PtrAllocatorType;

	Stack_Allocator()
		: m_blockSize( 0 )
		, m_initChunk( nullptr )
		, m_stack ( )
	{
	}

	void Initialize(std::size_t i_StackSize)
	{
		m_stack.Init(i_StackSize);
		m_blockSize = i_StackSize;
	}

	void * Allocate( void )
	{
		return nullptr;
	}

	void * Allocate(std::size_t i_size)
	{
		return m_stack.Allocate(i_size);
	}

	bool Deallocate( void * p, PtrChunkType hint )
	{
		m_stack.Deallocate(p,SizeLastBlock());
		return true;
	}

	bool Deallocate( void )
	{
		m_stack.Deallocate(SizeLastBlock());
		return true;
	}

	CPtrChunkType CHasBlock( void * p ) const
	{
		return &m_stack;
	}

	PtrChunkType HasBlock( void * p )
	{
		return const_cast< PtrChunkType >(const_cast<AllocatorType*>(this)->CHasBlock( p ) );
	}

	~Stack_Allocator()
	{

	}

private:

	int SizeLastBlock()
	{
		return m_stack.SizeLastBlock();
	}

	PtrChunkType m_initChunk;
	ChunkType m_stack;
	unsigned int m_blockSize;
};



static unsigned char MaxObjectsPerChunk = 0xFF;
static unsigned char MinObjectsPerChunk = 8;



template <typename Chunk = Default_Chunk>
class Fixed_Allocator : public AllocatorInterfaces<Chunk>
{
public:
	typedef typename Chunk::ChunkType ChunkType;
	typedef typename Chunk::PtrChunkType PtrChunkType;
	typedef typename Chunk::CPtrChunkType CPtrChunkType;
	typedef typename Chunk::RefChunkType RefChunkType;
	typedef typename Chunk::CRefChunkType CRefChunkType;

	typedef typename Fixed_Allocator<ChunkType> AllocatorType;
	typedef typename Fixed_Allocator<ChunkType>* PtrAllocatorType;

	Fixed_Allocator()
		: m_blockSize( 0 )
		, m_numBlocks( 0 )
		, m_chunks( 0 )
		, m_allocChunk( nullptr )
		, m_deallocChunk( nullptr )
		, m_emptyChunk( nullptr )
	{
	}

	void Initialize(std::size_t i_blockSize, std::size_t i_pageSize)
	{
		assert( i_blockSize > 0 );
		assert( i_pageSize >= i_blockSize );
		m_blockSize = i_blockSize;
		std::size_t numBlocks = i_pageSize / i_blockSize;
		if ( numBlocks > MaxObjectsPerChunk ) numBlocks = MaxObjectsPerChunk;
		else if ( numBlocks < MinObjectsPerChunk ) numBlocks = MinObjectsPerChunk;

		m_numBlocks = static_cast<unsigned char>(numBlocks);
		assert(m_numBlocks == numBlocks);
	}

	void * Allocate( void )
	{
		// prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
		assert( ( nullptr == m_emptyChunk ) || ( m_emptyChunk->HasAvailable( m_numBlocks ) ) );
		assert( CountEmptyChunks() < 2 );
		//create new chunk
		if ( ( nullptr == m_allocChunk ) || m_allocChunk->IsFilled() )
		{
			if ( nullptr != m_emptyChunk)
			{
				m_allocChunk= m_emptyChunk;
				m_emptyChunk = nullptr;
			}
			else
			{
				for ( iterator_chunk i( m_chunks.begin() ); ; ++i )
				{
					if ( m_chunks.end() == i )
					{
						if ( !MakeNewChunk() )
							return nullptr;
						break;
					}
					if ( !i->IsFilled() )
					{
						m_allocChunk = &*i;
						break;
					}
				}
			}
		}
		else if ( m_allocChunk == m_emptyChunk)
			m_emptyChunk = nullptr;

		assert( m_allocChunk != nullptr );
		assert( !m_allocChunk->IsFilled() );
		void * place = m_allocChunk->Allocate( m_blockSize );

		// prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
		assert( ( nullptr == m_emptyChunk ) || ( m_emptyChunk->HasAvailable( m_numBlocks ) ) );
		assert( CountEmptyChunks() < 2 );

		return place;
	}

	void* Allocate(std::size_t blockSize)
	{
		return nullptr;
	}

	bool Deallocate( void * p, PtrChunkType i_hint )
	{
		assert( !m_chunks.empty() );
		assert( &m_chunks.front() <= m_deallocChunk );
		assert( &m_chunks.back() >= m_deallocChunk );
		assert( &m_chunks.front() <= m_allocChunk );
		assert( &m_chunks.back() >= m_allocChunk );
		assert( CountEmptyChunks() < 2 );

		PtrChunkType foundChunk = nullptr;
		const ::std::size_t chunkLength = m_numBlocks * m_blockSize;
		if ( ( nullptr != i_hint ) && ( i_hint->HasBlock( p, chunkLength ) ) )
			foundChunk = i_hint;
		else if ( m_deallocChunk->HasBlock( p, chunkLength ) )
			foundChunk = m_deallocChunk;
		else if ( m_allocChunk->HasBlock( p, chunkLength ) )
			foundChunk = m_allocChunk;
		else
			foundChunk = VicinityFind( p );
		if ( nullptr == foundChunk )
			return false;

		assert( foundChunk->HasBlock( p, chunkLength ) );
		m_deallocChunk = foundChunk;
		DoDeallocate(p);
		assert( CountEmptyChunks() < 2 );

		return true;
	}

	bool Deallocate()
	{
		return false;
	}

	std::size_t BlockSize() const
	{
		return m_blockSize;
	}

	CPtrChunkType CHasBlock( void * p ) const
	{
		const std::size_t chunkLength = m_numBlocks * m_blockSize;
		for ( const_iterator_chunk it( m_chunks.cbegin() ); it != m_chunks.cend(); ++it )
		{
			CRefChunkType chunk = *it;
			if ( chunk.HasBlock( p, chunkLength ) )
				return &chunk;
		}
		return nullptr;
	}

	PtrChunkType HasBlock( void * p )
	{
		return const_cast< PtrChunkType >(const_cast<AllocatorType*>(this)->CHasBlock( p ) );
	}

	~Fixed_Allocator()
	{
		for ( iterator_chunk i( m_chunks.begin() ); i != m_chunks.end(); ++i )
			i->Release();
	}

	//typedef Chunk ChunkOut;

private:

	std::size_t CountEmptyChunks( void ) const
	{
		return ( nullptr == m_emptyChunk ) ? 0 : 1;
	}

	bool MakeNewChunk( void )
	{
		bool allocated = false;

		std::size_t size = m_chunks.size();
		// Calling m_chunks.reserve *before* creating and initializing the new
		// Chunk means that nothing is leaked by this function in case an
		// exception is thrown from reserve.
		if ( m_chunks.capacity() == size )
		{
			if ( 0 == size ) size = 4;
			m_chunks.resize( size * 2 );
		}
		PtrChunkType newChunk = new ChunkType();
		allocated = newChunk->Init( m_blockSize, m_numBlocks );
		if ( allocated )
			m_chunks.push_back( *newChunk );

		if ( !allocated ) return false;

		m_allocChunk = &m_chunks.back();
		m_deallocChunk = &m_chunks.front();
		return true;
	}

	PtrChunkType VicinityFind( void * p ) const
	{
		if ( m_chunks.empty() )
			return nullptr;
		assert(m_deallocChunk);

		const ::std::size_t chunkLength = m_numBlocks * m_numBlocks;
		PtrChunkType lo = m_deallocChunk;
		PtrChunkType hi = m_deallocChunk + 1;
		CPtrChunkType loBound = &m_chunks.front();
		CPtrChunkType hiBound = &m_chunks.back() + 1;

		// Special case: deallocChunk_ is the last in the array
		if ( hi == hiBound )
			hi = nullptr;

		for (;;)
		{
			if (lo)
			{
				if ( lo->HasBlock( p, chunkLength ) )
					return lo;
				if ( lo == loBound )
				{
					lo = nullptr;
					if ( nullptr == hi )
						break;
				}
				else --lo;
			}

			if (hi)
			{
				if ( hi->HasBlock( p, chunkLength ) )
					return hi;
				if ( ++hi == hiBound )
				{
					hi = nullptr;
					if ( nullptr == lo )
						break;
				}
			}
		}

		return nullptr;
	}

	void DoDeallocate(void* p)
	{
		// Show that deallocChunk_ really owns the block at address p.
		assert( m_deallocChunk->HasBlock( p, m_numBlocks * m_blockSize ) );
		// Either of the next two assertions may fail if somebody tries to
		// delete the same block twice.
		assert( m_emptyChunk != m_deallocChunk );
		assert( !m_deallocChunk->HasAvailable( m_numBlocks ) );
		// prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
		assert( ( nullptr == m_emptyChunk ) || ( m_emptyChunk->HasAvailable( m_numBlocks ) ) );

		// call into the chunk, will adjust the inner list but won't release memory
		m_deallocChunk->Deallocate(p, m_blockSize);

		if ( m_deallocChunk->HasAvailable( m_numBlocks ) )
		{
			assert( m_emptyChunk != m_deallocChunk );
			// deallocChunk_ is empty, but a Chunk is only released if there are 2
			// empty chunks.  Since emptyChunk_ may only point to a previously
			// cleared Chunk, if it points to something else besides deallocChunk_,
			// then FixedAllocator currently has 2 empty Chunks.
			if ( nullptr != m_emptyChunk )
			{
				// If last Chunk is empty, just change what deallocChunk_
				// points to, and release the last.  Otherwise, swap an empty
				// Chunk with the last, and then release it.
				PtrChunkType lastChunk = &m_chunks.back();
				if ( lastChunk == m_deallocChunk )
					m_deallocChunk = m_emptyChunk;
				else if ( lastChunk != m_emptyChunk )
					std::swap( *m_emptyChunk, *lastChunk );
				assert( lastChunk->HasAvailable( m_numBlocks ) );
				lastChunk->Release();
				m_chunks.pop_back();
				if ( ( m_allocChunk == lastChunk ) || m_allocChunk->IsFilled() )
					m_allocChunk = m_deallocChunk;
			}
			m_emptyChunk = m_deallocChunk;
		}

		// prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
		assert( ( nullptr == m_emptyChunk ) || ( m_emptyChunk->HasAvailable( m_numBlocks ) ) );
	}

	Fixed_Allocator(const Fixed_Allocator&);
		/// Not implemented.
	Fixed_Allocator& operator=(const Fixed_Allocator&);

	std::vector<ChunkType> m_chunks;
	typedef typename std::vector<ChunkType>::iterator iterator_chunk;
	typedef typename std::vector<ChunkType>::const_iterator const_iterator_chunk;
	unsigned char m_numBlocks;
	std::size_t m_blockSize;
	PtrChunkType m_emptyChunk;
	PtrChunkType m_allocChunk;
	PtrChunkType m_deallocChunk;
};


