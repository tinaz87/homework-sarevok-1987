#include <WinBase.h>

class ThreadSafeInterface
{
public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;

};

 class NoThreadSafe : public ThreadSafeInterface
 {
 
 public:
	NoThreadSafe()
	{
		
	}
 
	void Lock( void )
	{
		return;
	}
 
	void Unlock( void )
	{
		return;
	}
 
	~NoThreadSafe( void )
	{
		std::cout << "no safety";
	}
 };

#ifdef _WINDOWS_
class WinThreadSafe :public ThreadSafeInterface
{
	CRITICAL_SECTION MemoryManagerCS;
public:
	WinThreadSafe()
	{
		InitializeCriticalSection(&MemoryManagerCS);
	}

	void Lock()
	{
		EnterCriticalSection(&MemoryManagerCS);
	}

	void Unlock()
	{
		LeaveCriticalSection(&MemoryManagerCS);
	}

	~WinThreadSafe()
	{
		DeleteCriticalSection(&MemoryManagerCS);
	}
};
#endif // _WINDOWS_

#ifdef _POSIX_
class PosixThreadSafe :public ThreadSafeInterface
{
	pthread_mutex_t MemoryManagerCS;
public:
	WinThreadSafe()
	{
		pthread_mutex_init(MemoryManagerCS, PTHREAD_MUTEX_INITIALIZER);
	}

	void Lock()
	{
		pthread_mutex_lock(&MemoryManagerCS);
	}

	void Unlock()
	{
		pthread_mutex_unlock(&MemoryManagerCS);
	}

	~WinThreadSafe()
	{
		pthread_mutex_destroy(&MemoryManagerCS);
	}
};
#endif // _POSIX_