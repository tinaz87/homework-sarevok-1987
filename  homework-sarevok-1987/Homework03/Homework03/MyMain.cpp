#include <iostream>
#include <list>
#include <vector>
#include "SList.h"
#include "SListArray.h"
#include "SFixedList.h"
template<typename T >
void print_list(T& mp);

int main()
{
	std::vector<int> pre(2,1);
	SList<std::vector<int>> tmp(2,pre);
	SList<int> prova(5,4);
	SList<int> merge(5,2);
	prova.push_back(3);
	print_list(prova);
	prova.push_front(4);
	print_list(prova);
	prova.insert(prova.begin(),9);
	print_list(prova);
	prova.insert(prova.end(),2,7);
	print_list(prova);
	prova.insert(pre.begin(),pre.end(),prova.end());
	print_list(prova);
	prova.erase(prova.end());
	print_list(prova);
	prova.erase(prova.begin(),prova.end());
	print_list(prova);
	prova.push_front(5);
	print_list(prova);
	prova.resize(8);
	print_list(prova);
	prova.resize(4);
	print_list(prova);
	prova.merge(merge);
	print_list(prova);
	prova.remove(0);
	print_list(prova);
	prova.swap(merge);
	print_list(prova);
	if(merge > prova)
	{
		std::cout << "ok compare" << std::endl ;
	}
	std::list<int> prova1(prova.begin(),prova.end());
	print_list(prova1);
	prova1.clear();
	print_list(prova1);
	SList<int> prova7(10,0);
	SList<int> prova8(5,1);
	prova7.swap(prova8);
	print_list(prova7);
	prova8.clear();

	SListArray<int> prova10(10,0);
	for(int i=0;i < 6; ++i)
		prova10.erase(++prova10.begin());

	SListArray<int> prova6(5);
	SListArray<int> prova5(5,5);
	prova5.push_back(6);
	print_list(prova5);
	prova5.push_front(7);
	print_list(prova5);
	prova5.pop_back();
	print_list(prova5);
	prova5.pop_front();
	print_list(prova5);
	prova5.insert(++prova5.begin(),4);
	print_list(prova5);
	prova5.insert(prova5.begin(), 4, 3);
	print_list(prova5);
	prova5.erase(++(++prova5.begin()));
	print_list(prova5);
	prova5.resize(5);
	print_list(prova5); 
	prova5.merge(prova6);
	print_list(prova5);
	prova5.clear();
	print_list(prova5);
	FixedSList<int,25> prv;
	FixedSList<int,25> prv1(5);
	FixedSList<int,25> prv2(5,2);

	prv1.push_back(9);
	print_list(prv1);
	prv1.push_front(3);
	print_list(prv1);
	prv1.pop_back();
	print_list(prv1);
	prv1.pop_front();
	print_list(prv1);
	prv1.insert(++prv1.begin(),2,8);
	print_list(prv1);
	prv1.erase(++(++prv1.begin()));
	print_list(prv1);
	prv1.clear();
	print_list(prv1);
	malloc(4);
	getchar();
}

template<typename T >
void print_list(T& mp)
{
	std::cout << "[";
	for (T::iterator it = mp.begin();it != mp.end();++it)
	{
		std::cout << *it << " ";
	}
	std::cout << "] "<< mp.size() <<std::endl;;
}