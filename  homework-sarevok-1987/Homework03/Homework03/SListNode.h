#pragma once

template < typename T>
class SListNode
{
public:
	SListNode* Next;
	T val;

	explicit SListNode()
	{
		this->val = static_cast<T>(0);
		this->Next = NULL;
	}

	explicit SListNode(const SListNode<T>& i_copy)
	{
		this->val = i_copy.val;
		this->Next = i_copy.Next;
	}

	explicit SListNode(const T& i_val)
	{
		this->val = i_val;
		this->Next = NULL;
	}

	explicit SListNode(const T& i_val, SListNode* i_next)
	{
		this->val = i_val;
		this->Next = i_next;
	}

	explicit SListNode(const T& i_val, int i_next)
	{
		this->val = i_val;
		this->Next = i_next;
	}

	~SListNode()
	{

	}

	SListNode& operator=(const SListNode& i_copy)
	{
		this->val = i_copy.val;
		this->Next = i_copy.Next;
		return *this;
	}
};

template < typename T>
class SListNodeInt
{
public:
	int Next;
	T val;

	explicit SListNodeInt()
	{
		this->val = static_cast<T>(0);
		this->Next = -1;
	}

	explicit SListNodeInt(const SListNode<T>& i_copy)
	{
		this->val = i_copy.val;
		this->Next = i_copy.Next;
	}

	explicit SListNodeInt(const T& i_val)
	{
		this->val = i_val;
		this->Next = -1;
	}

	explicit SListNodeInt(const T& i_val, int i_next)
	{
		this->val = i_val;
		this->Next = i_next;
	}

	~SListNodeInt()
	{

	}

	SListNodeInt& operator=(const SListNodeInt& i_copy)
	{
		this->val = i_copy.val;
		this->Next = i_copy.Next;
		return *this;
	}
};