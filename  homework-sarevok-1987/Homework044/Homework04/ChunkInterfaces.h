#pragma  once
#include <iostream>
#include <bitset>
#include <assert.h>


class ChunkInterfaces
{
public:
	typedef ChunkInterfaces ChunkType;
	typedef ChunkInterfaces* PtrChunkType;
	typedef const ChunkInterfaces* CPtrChunkType;
	typedef ChunkInterfaces& RefChunkType;
	typedef const ChunkInterfaces& CRefChunkType;

	virtual void* Allocate(std::size_t blockSize) = 0;
	virtual void Deallocate(void* p, std::size_t blockSize) = 0;
	virtual void Release() = 0;
	virtual ~ChunkInterfaces(){};
};
//ciao
class Simple_Chunk : public ChunkInterfaces
{
	unsigned int* m_data;
	typedef unsigned int Marker;
	Marker m_stackSize;
	Marker m_top;
	Marker m_previus;
	Marker m_sprevius;
public:

	typedef Simple_Chunk ChunkType;
	typedef Simple_Chunk* PtrChunkType; 
	typedef const Simple_Chunk* CPtrChunkType;
	typedef Simple_Chunk& RefChunkType;
	typedef const Simple_Chunk& CRefChunkType;

	bool Init(std::size_t i_stackSize)
	{
		assert(i_stackSize > 0);
		// Overflow check
		m_data = static_cast<unsigned int *>(std::calloc(i_stackSize,1));

		if ( nullptr == m_data)
			return false;
		Reset(i_stackSize);
		return true;
	}

	virtual void Reset( std::size_t i_stackSize)
	{
		assert(i_stackSize > 0);

		m_top = 0;
		m_previus = 0;
		m_sprevius = 0;
		m_stackSize = i_stackSize;
	}

	void* Allocate(std::size_t blockSize)
	{
		if(IsFilled())
			return nullptr;
		assert(m_top + blockSize <= m_stackSize);
		assert(m_previus <= m_top);
		unsigned int *pResult;
		//if(m_top % sizeof(int) != 0)
		//	pResult = m_data + ((m_top+1 / sizeof(int)));
		//else
		//	pResult = m_data + (m_top / sizeof(int));
		pResult = reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(m_data) + m_top);
		m_sprevius = m_previus;
		m_previus = m_top;
		m_top += blockSize;
		m_stackSize -= blockSize;
		return pResult;
	}

	void Deallocate(void* p, std::size_t blockSize)
	{
		assert(m_top-m_previus == blockSize);
		assert(m_top - blockSize > 0);
		assert(reinterpret_cast<unsigned int *>(p) >= m_data);
		assert(reinterpret_cast<unsigned int *>(p) < reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(m_data) + m_top));
		unsigned int blockDealSize = reinterpret_cast<unsigned char *>(reinterpret_cast<unsigned char *>(m_data) + m_top) - reinterpret_cast<unsigned char *>(p);
		m_top -= blockDealSize;
		m_previus = m_sprevius;
		m_stackSize += blockDealSize;
	}

	void Deallocate(std::size_t blockSize)
	{
		assert(m_top-m_previus == blockSize);
		assert(m_top - blockSize >=0);
		m_top = m_previus;
		m_previus = m_sprevius;
		m_stackSize += blockSize;
	}

	void Release()
	{
		return free(m_data);
	}

	int SizeLastBlock()
	{
		Marker g = m_top - m_previus;
		assert(m_top - m_previus >= 0);
		return m_top - m_previus;
	}

	bool IsFilled( void ) const
	{
		return m_stackSize == 0; 
	}



	~Simple_Chunk()
	{
		Release();
	}
};

class Default_Chunk : public ChunkInterfaces
{
private:
	unsigned char *m_data;
	unsigned char m_fstAvailableBlock;
	unsigned char m_numBlocksAvb;

	
public:
	typedef Default_Chunk ChunkType;
	typedef Default_Chunk* PtrChunkType;
	typedef const Default_Chunk* CPtrChunkType;
	typedef Default_Chunk& RefChunkType;
	typedef const Default_Chunk& CRefChunkType;

	Default_Chunk()
		:m_data(nullptr)
		,m_fstAvailableBlock(0)
		,m_numBlocksAvb(0)
	{
	}

	bool Init(std::size_t i_blockSize, unsigned char i_blocks)
	{
		assert(i_blockSize > 0);
		assert(i_blocks > 0);
		// Overflow check
		assert( (i_blockSize * i_blocks) / i_blockSize == i_blocks);
		m_data = static_cast<unsigned char *>(std::calloc(i_blocks,i_blockSize));

		if ( nullptr == m_data)
			return false;
		Reset(i_blockSize,i_blocks);
		return true;
	}

	void Release()
	{
		std::free(m_data);
	}

	virtual void Reset( std::size_t i_blockSize, unsigned char i_blocks )
	{
		assert(i_blockSize > 0);
		assert(i_blocks > 0);
		// Overflow check
		assert((i_blockSize * i_blocks) / i_blockSize == i_blocks);

		m_fstAvailableBlock = 0;
		m_numBlocksAvb = i_blocks;

		unsigned char i = 0;
		for ( unsigned char * p = m_data; i != i_blocks; p += i_blockSize )
			*p = ++i;
	}

	bool IsFilled( void ) const
	{
		return m_numBlocksAvb == 0; 
	}

	void* Allocate(std::size_t i_blockSize)
	{
		if ( IsFilled() )
			return nullptr;

		assert((m_fstAvailableBlock * i_blockSize) / i_blockSize ==	m_fstAvailableBlock);
		unsigned char * pResult = m_data + (m_fstAvailableBlock * i_blockSize);
		m_fstAvailableBlock = *pResult;
		--m_numBlocksAvb;

		return pResult;
	}

	void Deallocate(void* p, std::size_t i_blockSize)
	{
		assert(p >= m_data);

		unsigned char* toRelease = static_cast<unsigned char*>(p);

		assert((toRelease - m_data) % i_blockSize == 0);

		unsigned char index = static_cast< unsigned char >(( toRelease - m_data ) / i_blockSize);

		if ( 0 < m_numBlocksAvb )
			assert( m_fstAvailableBlock != index );
		*toRelease = m_fstAvailableBlock;
		m_fstAvailableBlock = index;
		// Truncation check
		assert(m_fstAvailableBlock == (toRelease - m_data) / i_blockSize);

		++m_numBlocksAvb;
	}

	bool IsBlockAvailable( void * p, unsigned char i_numBlocks, std::size_t i_blockSize ) const
	{
		if ( IsFilled() )
			return false;

		unsigned char *place = static_cast< unsigned char * >(p);
		// Alignment check
		assert( ( place - m_data ) % i_blockSize == 0 );
		unsigned char blockIndex = static_cast< unsigned char >(( place - m_data ) / i_blockSize );

		unsigned char index = m_fstAvailableBlock;
		assert( i_numBlocks > index );
		if ( index == blockIndex )
			return true;

		/* If the bit at index was set in foundBlocks, then the stealth index was
		found on the linked-list.
		*/
		std::bitset< UCHAR_MAX > foundBlocks;
		unsigned char * nextBlock = nullptr;
		for ( unsigned char cc = 0; ; )
		{
			nextBlock = m_data + ( index * i_blockSize );
			foundBlocks.set( index, true );
			++cc;
			if ( cc >= m_numBlocksAvb )
				// Successfully counted off number of nodes in linked-list.
				break;
			index = *nextBlock;
			if ( index == blockIndex )
				return true;
			assert( i_numBlocks > index );
			assert( !foundBlocks.test( index ) );
		}

		return false;
	}

	bool HasBlock( void * p, ::std::size_t chunkLength ) const //check pointer is in range
	{
		unsigned char * pc = static_cast< unsigned char * >( p );
		return ( m_data <= pc ) && ( pc < m_data + chunkLength );
	}

	bool HasAvailable( unsigned char numBlocks ) const
	{
		return ( m_numBlocksAvb == numBlocks );
	}

	~Default_Chunk()
	{
		//Release();
	}
};