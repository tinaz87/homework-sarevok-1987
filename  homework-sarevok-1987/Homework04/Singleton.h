#pragma once

#include <iostream>

template<typename T>
class SingletonHolder
{
public:

	///  Type of the singleton object
	typedef T ObjectType;

	///  Returns a reference to singleton object
	static T& Instance()
	{
		if (!pInstance_)
		{
			MakeInstance();
		}
		return *pInstance_;
	}
private:
	// Helpers
	static void MakeInstance()
	{
		if (!pInstance_)
		{
			if (destroyed_)
			{
				destroyed_ = false;
			}
			pInstance_ = new T();
		}
	}
	static void DestroySingleton()
	{
		assert(!destroyed_);
		pInstance_->~T();
		pInstance_ = 0;
		destroyed_ = true;
	}

	// Protection
	SingletonHolder();

	// Data
	static T* pInstance_;
	static bool destroyed_;
};

template<typename T>
typename T*	SingletonHolder<T>::pInstance_ = NULL;

template<typename T>
bool SingletonHolder<T>::destroyed_ = false;