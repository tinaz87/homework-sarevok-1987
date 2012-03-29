#include <WinBase.h>

class ThreadSafeInterface
{
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	virtual ~ThreadSafeInterface() = 0;
};

class WinThreadSafe :public ThreadSafeInterface
{
	CRITICAL_SECTION MemoryManagerCS;

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

