//// Homework02.h
//Design a class (or a class library) to handle unlimited integer operations (integer larger )
//	Provide the class standard overloaded arithmetic operators.
//
//	follow the rules "do as ints do".
//
//	Write a test for BigInt operations:
//
//Example:
//
//{
//	z &= x; // bitwise and, or, xor...
//}
#pragma once

#include <string>
#include <iostream>
#include <math.h>

class BigInt
{

private:
	int *number;
	int length;
	bool sign;
public:
	BigInt():length(1)
	{
		number = new int[1];
		number[0] = 0;
		sign=true;
		length = 1;
	}

	BigInt(const int& i)
	{
		if(i==0)
		{
			number = new int[1];
			number[0] = 0;
			sign=true;
			length = 1;
		}
		else
		{
			int len=0;
			int tmp = std::abs(i);
			if (i<0)
				sign = false;
			else
				sign = true;
			while(tmp > 0)
			{
				tmp=tmp/10;
				len++;
			}
			number = new int[len];
			length=len;
			len=0;
			tmp = std::abs(i);
			while (tmp>0)
			{
				number[len]=tmp%10;
				tmp=tmp/10;
				++len;
			}
		}
	}

	explicit BigInt(std::string str)
	{
		size_t len=str.size();
		int j=0;
		if (str[0]=='-')
		{
			sign=false;
			number=new int[len-1];
			length = len-1;
		}
		else
		{
			sign=true;
			number=new int[len];
			length = len;
		}
		int start=len-1;
		int finish=0;
		if (!sign)
		{
			finish = 1;
		}
		for(int i=start;i>=finish;--i)
		{
			char c = str[i];
			number[j]=atoi(&c);
			++j;
		}
	}

	explicit BigInt(const char* str)
	{
		size_t len=strlen(str);
		int j=0;
		if (str[0]=='-')
		{
			sign=false;
			number=new int[len-1];
			length = len-1;
		}
		else
		{
			sign=true;
			number=new int[len];
			length = len;
		}
		int start=len-1;
		int finish=0;
		if (!sign)
		{
			finish = 1;
		}
		for(int i=start;i>=finish;--i)
		{
			char c = str[i];
			number[j]=atoi(&c);
			++j;
		}
	}

	BigInt(const BigInt& i_that)
	{
		length = i_that.length;
		number = new int[i_that.length];
		for(int i=0;i< i_that.length;++i)
		{
			number[i]=i_that.number[i];
		}
		sign=i_that.sign;
	}

	~BigInt()
	{
		delete []number;
	}

	BigInt& operator= (const BigInt &i_other)
	{
		length = i_other.length;
		
		int *tmp = new int[i_other.length];
		for(int i=0;i< i_other.length;++i)
			tmp[i]=i_other.number[i];
		delete [] number;
		number = new int[i_other.length];
		for(int i=0;i< i_other.length;++i)
			number[i]=tmp[i];
		sign=i_other.sign;
		delete [] tmp;
		return *this;
	}

	BigInt& operator= (const int &i_other)
	{
		BigInt tmp(i_other);
		*this = tmp;
		return *this;
	}

	BigInt& operator+=(const BigInt &i_other)
	{
		if((!sign)&&(i_other.sign))
		{
			(*this) = -(*this);
			BigInt tmp1= i_other;
			if((*this) > tmp1)
			{
				(*this).internal_operation(tmp1,false);
				(*this) = -(*this);
			}
			else
			{
				tmp1.internal_operation((*this),false);
				(*this)=tmp1;
			}
			return *this;
		}
		if((sign)&&(!i_other.sign))
		{
			BigInt tmp1= i_other;
			tmp1= -tmp1;
			if((*this) > tmp1)
			{
				(*this).internal_operation(tmp1,false);
			}
			else
			{
				tmp1.internal_operation((*this),false);
				(*this)=-tmp1;
			}
			return *this;
		}
		if((!sign)&&(!i_other.sign))
		{
			BigInt tmp1= i_other;
			tmp1= -tmp1;
			(*this) = -(*this);
			(*this).internal_operation(tmp1);
			(*this) = -(*this);
			return (*this);
		}
		return (*this).internal_operation(i_other);
	}

	BigInt& operator-=(const BigInt &i_other)
	{
		BigInt tmp = i_other;
		tmp = -tmp;
		(*this)+=tmp;
		return (*this);
	}

	BigInt& operator*=(const BigInt &i_other)
	{
		BigInt tmp1 = i_other;
		bool op_sign=true;
		if((tmp1.sign)&&(!sign))
		{
			(*this) = -(*this);
			op_sign=false;
		}
		if((!tmp1.sign)&&(sign))
		{
			tmp1 = -tmp1;
			op_sign = false;
		}
		if((!tmp1.sign)&&(!sign))
		{
			tmp1 = -tmp1;
			(*this) = -(*this);
		}
		BigInt tmp = (*this);
		for(int i=1 ;i < tmp1; ++i)
		{
			*this += tmp;
		}
		sign = op_sign;
		return *this;
	}

	BigInt& operator/=(const BigInt &i_other)
	{
		BigInt tmp1 = i_other;
		bool op_sign=true;
		if((tmp1.sign)&&(!sign))
		{
			(*this) = -(*this);
			op_sign=false;
		}
		if((!tmp1.sign)&&(sign))
		{
			tmp1 = -tmp1;
			op_sign = false;
		}
		if((!tmp1.sign)&&(!sign))
		{
			tmp1 = -tmp1;
			(*this) = -(*this);
		}
		BigInt tmp = (*this);
		BigInt res = 0;
		if(tmp>tmp1)
		{
			while(tmp>=tmp1)
			{
				tmp-=tmp1;
				res++;
			}
		}
		else
			res = 0;
		sign = op_sign;
		(*this) = res;
		return *this;
	}

	BigInt operator%=(const BigInt &i_other)
	{
		BigInt tmp1 = i_other;
		bool op_sign=true;
		if((tmp1.sign)&&(!sign))
		{
			(*this) = -(*this);
			op_sign=false;
		}
		if((!tmp1.sign)&&(sign))
		{
			tmp1 = -tmp1;
			op_sign = false;
		}
		if((!tmp1.sign)&&(!sign))
		{
			tmp1 = -tmp1;
			(*this) = -(*this);
		}
		BigInt tmp = (*this);
		BigInt res = 0;
		if(tmp>tmp1)
		{
			while(tmp>=tmp1)
			{
				tmp-=tmp1;
				res++;
			}
		}
		else
			res = 0;
		sign = op_sign;
		(*this) = tmp;
		return *this;
	}

	BigInt& operator&=(const BigInt &i_other)
	{
		BigInt base=i_other;
		base++;
		(*this)%=base;
		return (*this);
	}

	BigInt& operator|=(const BigInt &i_other)
	{
		int size1=0;
		int size2=0;
		int *data1= bit_data((*this),size1);
		int *data2= bit_data(i_other,size2);
		int min_size = std::min(size1,size2);
		for (int i=0;i<min_size;++i)
		{
			if(min_size==size1)
				data2[i]&=data1[i];
			else
				data1[i]&=data2[i];
		}
		if (min_size==size1)
			(*this) = data_bit(data2,size2);
		else
			(*this) = data_bit(data1,size1);
		delete [] data1;
		delete [] data2;
		return *this;
	}

	BigInt& operator^=(const BigInt &i_other)
	{
		int size1=0;
		int size2=0;
		int *data1= bit_data((*this),size1);
		int *data2= bit_data(i_other,size2);
		int min_size = std::min(size1,size2);
		for (int i=0;i<min_size;++i)
		{
			if(min_size==size1)
				data2[i]^=data1[i];
			else
				data1[i]^=data2[i];
		}
		if (min_size==size1)
			(*this) = data_bit(data2,size2);
		else
			(*this) = data_bit(data1,size1);
		delete [] data1;
		delete [] data2;
		return *this;
	}

	BigInt operator ~() const
	{
		int size1=0;

		int *data1= bit_data((*this),size1);
		for (int i=0;i<size1;++i)
		{
			data1[i]= ~data1[i];
		}
		BigInt tmp = data_bit(data1,size1);
		delete [] data1;
		return tmp;
	}

	int *bit_data(const BigInt& i_bi,int &o_size) const 
	{
		BigInt c_i_bi=i_bi;
		int exp = blog(c_i_bi);
		BigInt tmp = 0;
		o_size=exp;
		int *data= new int[exp+1];
		while(exp>=0)
		{
			tmp=pow(BigInt(2),exp);
			if(c_i_bi>=tmp)
			{
				data[exp]=1;
				c_i_bi-=tmp;
			}
			else
				data[exp]=0;
			exp--;
		}
		return data;
	}

	BigInt data_bit(const int* i_data,int &i_size) const 
	{
		BigInt tmp=0;
		int exp=0;
		for(int i=0;i<i_size;i++)
		{
			exp = (int)pow(2.0f,i);
			tmp+=(i_data[i] * exp);
		}
		return tmp;
	}

	BigInt& operator<<=(const int &i_other)
	{
		int base=0;
		base =(int) std::pow(2.0f,i_other);
		BigInt tmp(base);
		(*this)*=tmp;
		return (*this);
	}

	BigInt& operator>>=(const int &i_other)
	{
		int base=0;
		base =(int) std::pow(2.0f,i_other);
		BigInt tmp(base);
		(*this)/=tmp;
		return (*this);
	}

	BigInt operator-()
	{
		
		BigInt tmp= *this;
		tmp.sign=!tmp.sign;
		return tmp;
	}

	BigInt operator++()
	{
		(*this) += 1;
		return (*this);
	}

	BigInt operator++(int)
	{
		BigInt tmp = (*this);
		++(*this);
		return tmp;
	}

	BigInt operator--()
	{
		(*this) -= 1;
		return (*this);
	}

	BigInt operator--(int)
	{
		BigInt tmp = (*this);
		--(*this);
		return tmp;
	}

	BigInt& internal_operation(const BigInt &i_other,bool i_operation = true)
	{
		int tmp_len=std::max(length,i_other.length)+1;// tolgo g� il segno per gestire il caso 999+1 = 1000
		int *tmp_num= new int[tmp_len];//array di appoggio
		int *tmp_num1= new int[tmp_len];//array di appoggio
		for(int pos=0;pos< tmp_len ; ++pos)
		{
			tmp_num[pos]=0;
			tmp_num1[pos]=0;
		}
		for(int pos=0;pos< length ; ++pos)
			tmp_num[pos]=number[pos];
		for(int pos=0;pos< i_other.length ; ++pos)
			tmp_num1[pos]=i_other.number[pos];
		for(int pos=0;pos< tmp_len ; ++pos)
		{
			if(i_operation)
			{
				tmp_num[pos] += tmp_num1[pos];
				if(tmp_num[pos]>=10)
				{
					if((pos+1)<tmp_len)
						tmp_num[pos+1]+=1;
					tmp_num[pos]%=10;
				}
			}
			else
			{
				tmp_num[pos] -= tmp_num1[pos];
				if(tmp_num[pos]<0)
				{
					if((pos+1)<tmp_len)
						tmp_num[pos+1]-=1;
					tmp_num[pos]= ((tmp_num[pos]+tmp_num1[pos])+10)-tmp_num1[pos];
				}
			}
		}
		while(tmp_num[tmp_len-1]<=0)// primi 0 tolti
			--tmp_len;
		if(tmp_len<=0)
		{
			delete [] number;
			number = new int[1];
			number[0]=0;
			length=1;
			sign=true;
		}
		else
		{
			delete [] number;
			number = new int[tmp_len];
			for(int pos=0;pos< tmp_len ; ++pos)
				number[pos]=tmp_num[pos];
			length=tmp_len;
		}
		delete [] tmp_num;
		delete [] tmp_num1;
		return *this;
	}

	friend int blog (BigInt const &i_bi);
	friend BigInt pow(const BigInt &i_base,const BigInt &i_exponet);
	friend std::ostream& operator<< (std::ostream& os, const BigInt& i);
	friend BigInt operator+(const BigInt &i_bi1,const BigInt &i_bi2);
	friend BigInt operator-(const BigInt &i_bi1,const BigInt &i_bi2);
	friend BigInt operator*(const BigInt &i_bi1,const BigInt &i_bi2);
	friend BigInt operator/(const BigInt &i_bi1,const BigInt &i_bi2);
	friend BigInt operator<<(const BigInt &i_bi1,const BigInt &i_bi2);
	friend BigInt operator>>(const BigInt &i_bi1,const BigInt &i_bi2);
	friend BigInt operator%(const BigInt &i_bi1,const BigInt &i_bi2);
	friend BigInt operator&(const BigInt &i_bi1,const BigInt &i_bi2);
	friend bool operator==(const BigInt &i_bi1, const BigInt &i_bi2);
	friend bool operator!=(const BigInt &i_bi1, const BigInt &i_bi2);
	friend bool operator<(const BigInt &i_bi1, const BigInt &i_bi2);
	friend bool operator>(const BigInt &i_bi1, const BigInt &i_bi2);
	friend bool operator<=(const BigInt &i_bi1, const BigInt &i_bi2);
	friend bool operator>=(const BigInt &i_bi1, const BigInt &i_bi2);
};