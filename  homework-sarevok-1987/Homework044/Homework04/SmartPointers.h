#pragma once

#include "MemoryManager.h"

template <class P>
class RefCounted
{
public:
	RefCounted(): m_pCount(static_cast<uintptr_t*>(MemoryManager::mallocObject(sizeof(uintptr_t))))
	{
		assert(m_pCount!=0);
		*m_pCount = 1;
	}

	RefCounted(const RefCounted& rhs): m_pCount(rhs.m_pCount)
	{}

	P Clone(const P& val)
	{
		++*m_pCount;
		return val;
	}

	bool Release(const P&)
	{
		if (!--*m_pCount)
		{
			MemoryManager::freeObject(m_pCount);
			m_pCount = NULL;
			return true;
		}
		return false;
	}

	void Swap(RefCounted& rhs)
	{
		std::swap(m_pCount, rhs.m_pCount);
	}

private:
	// Data
	uintptr_t* m_pCount;
};

template<typename T>
class SmartPtr
{
public:
	typedef typename T* StoredType; //the type of the pointee_ object
	typedef typename T* PointerType; //type returned by operator->
	typedef typename T& ReferenceType; //type returned by operator*

	typedef typename RefCounted<PointerType> RC;

	SmartPtr(PointerType i_var)
		:m_Pointee(i_var)
		,m_refCount()
	{
		
	}

	SmartPtr(SmartPtr& i_copy)
	{
		bool rel = m_refCount.Release(m_Pointee);
		m_Pointee = i_copy.m_refCount.Clone(i_copy.m_Pointee);
		assert(m_Pointee == i_copy.m_Pointee);
		m_refCount = i_copy.m_refCount;
		//assert(m_refCount == i_copy.m_refCount);
	}

	SmartPtr& operator=(SmartPtr& i_copy)
	{
		bool rel = m_refCount.Release(m_Pointee);
		m_Pointee = i_copy.m_refCount.Clone(i_copy.m_Pointee);
		assert(m_Pointee == i_copy.m_Pointee);
		m_refCount = i_copy.m_refCount;
		//assert(m_refCount == i_copy.m_refCount);
		return *this;
	}

	PointerType operator->() const
	{
		return m_Pointee;
	}

	ReferenceType operator*() const
	{
		return *m_Pointee;
	}

	template <class T>
	inline typename PointerType GetImpl(const SmartPtr<T>& sp)
	{
		return sp.m_Pointee;
	}

	template <class T>
	inline typename StoredType& GetImplRef(SmartPtr<T>& sp)
	{
		return sp.m_Pointee;
	}

	template <class T> void Reset(SmartPtr<T>& sp, T* source);
	template <class T> void Release(SmartPtr<T>& sp, T*& destination);
private:

	RefCounted<PointerType> m_refCount;
	StoredType m_Pointee;
};