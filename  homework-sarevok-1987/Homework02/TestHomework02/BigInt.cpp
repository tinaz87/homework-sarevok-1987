#include "BigInt.h"

std::ostream& operator<< (std::ostream& os, const BigInt& bi)
{
	if (!bi.sign)
		os << "-";
	for(int i=bi.length-1;i>=0;--i)
	{
		os << bi.number[i];
	}
	return os;
}

BigInt operator+(const BigInt &i_bi1,const BigInt &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp += i_bi2;
	return tmp;
}

BigInt operator-(const BigInt &i_bi1,const BigInt &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp -= i_bi2;
	return tmp;
}

BigInt operator*(const BigInt &i_bi1,const BigInt &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp *= i_bi2;
	return tmp;
}

BigInt operator/(const BigInt &i_bi1,const BigInt &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp /= i_bi2;
	return tmp;
}

BigInt operator%(const BigInt &i_bi1,const BigInt &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp %= i_bi2;
	return tmp;
}

BigInt operator&(const BigInt &i_bi1,const BigInt &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp &= i_bi2;
	return tmp;
}

BigInt operator<<(const BigInt &i_bi1,const int &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp <<= i_bi2;
	return tmp;
}

BigInt operator>>(const BigInt &i_bi1,const int &i_bi2)
{
	BigInt tmp = i_bi1;
	tmp >>= i_bi2;
	return tmp;
}

bool operator==(const BigInt &i_bi1, const BigInt &i_bi2)
{
	if(i_bi1.length!=i_bi2.length)
		return false;
	if(i_bi1.sign!=i_bi2.sign)
		return false;
	for(int i=0;i<i_bi1.length;++i)
	{
		if(i_bi1.number[i]!=i_bi2.number[i])
			return false;
	}
	return true;
}

bool operator!=(const BigInt &i_bi1, const BigInt &i_bi2)
{
	return !(i_bi1==i_bi2);
}

bool operator<(const BigInt &i_bi1, const BigInt &i_bi2)
{
	if(i_bi1.length<i_bi2.length)
		return true;
	if(i_bi1.length>i_bi2.length)
		return false;
	if((!i_bi1.sign) && (i_bi2.sign))
		return true;
	if((i_bi1.sign) && (!i_bi2.sign))
		return false;
	for(int i=i_bi1.length-1;i>=0;--i)
	{
		if(i_bi1.sign && i_bi2.sign)
		{
			if(i_bi1.number[i]>i_bi2.number[i])
				return false;
			else if (i_bi1.number[i]<i_bi2.number[i])
				return true;
		}
		else
		{
			if(i_bi1.number[i]<i_bi2.number[i])
				return false;
			else if (i_bi1.number[i]>i_bi2.number[i])
				return true;
		}
	}
	return false;
}

bool operator>(const BigInt &i_bi1, const BigInt &i_bi2)
{
	return !(i_bi1<=i_bi2);
}

bool operator<=(const BigInt &i_bi1, const BigInt &i_bi2)
{
	return ((i_bi1< i_bi2)||(i_bi1==i_bi2));
}

bool operator>=(const BigInt &i_bi1, const BigInt &i_bi2)
{
	return !(i_bi1<i_bi2);
}

BigInt pow(const BigInt &i_base,const BigInt &i_exponet)
{
	if(i_exponet==0)
		return 1;
	BigInt tmp = i_base;
	BigInt result = i_base;
	for (int i=1; i < i_exponet;++i)
		result*=tmp;
	return result;
}

int blog (BigInt const &i_bi)
{
	int r=0;
	BigInt tmp = i_bi;
	tmp>>=1;
	r++;
	while(tmp>0)
	{
		tmp>>=1;
		r++;
	}
	return r;
}