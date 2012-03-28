#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <intsafe.h>
#include "Timer.h"
#include "Singleton.h"
#include "MemoryManager.h"
#include "SmartPointers.h"
#include "StandardAllocator.h"

class PatateAllocTest
{
public:

	int tempo_cottura;
	bool cotto;
	char tipo;
	float pesoGr;

	PatateAllocTest():tempo_cottura(25),cotto(false),pesoGr(250)
	{

	}
	void arrosto()
	{
		return;
	}

	int getTempoCottura(){
		
		return tempo_cottura;
	}
};


PatateAllocTest* AllocateWithNew(){

	return MemoryManager::newObject<PatateAllocTest>(sizeof(PatateAllocTest));
}

void DeallocateWithNew(PatateAllocTest* p){

	MemoryManager::deleteObject(p);

}


PatateAllocTest* AllocateWithDefultCategory(){


	return  ( static_cast<PatateAllocTest *>(MemoryManager::mallocObjCategory<SmallObject_Category<>>(sizeof(PatateAllocTest))) );

}

void DeallocateWithDefaultCategory(PatateAllocTest* p){

	MemoryManager::freeObjectCategory<SmallObject_Category<>>(p);
}


PatateAllocTest* AllocateWithStackCategory(){


	return  ( static_cast<PatateAllocTest *>(MemoryManager::mallocObjCategory<Stack_Category<>>(sizeof(PatateAllocTest))) );

}

void DeallocateWithStackCategoryPtr(void *p){

	MemoryManager::freeObjectCategory<Stack_Category<>>(p);


}
void DeallocateWithStackCategory(){

	MemoryManager::freeObjectCategory<Stack_Category<>>();

}

PatateAllocTest* NewWithDefaultCategory(){

	return  MemoryManager::newObjectCategory<PatateAllocTest,SmallObject_Category<>>(1);

}

void DeleteWithDefaultCategory(PatateAllocTest* p){

	MemoryManager::deleteObjectCategory<SmallObject_Category<>>(p);

}


PatateAllocTest* NewWithStackCategory()
{
	return MemoryManager::newObjectCategory<PatateAllocTest,Stack_Category<>>(1);
}

void DeleteWithStackCategory(void *p)
{
	MemoryManager::deleteObjectCategory<Stack_Category<>>(p);
}

PatateAllocTest* DefaultMallocWithAlignmet()
{
	size_t align=0;
	while(!((align!=0) && ((align & (align - 1)) == 0)))
		align = rand() % 1024;
	size_t size = rand() % 255;
	PatateAllocTest* p=static_cast<PatateAllocTest*>(MemoryManager::mallocObjAlignment(size * sizeof(PatateAllocTest),align));
	if (reinterpret_cast<int>(p) % align != 0)
		std::cout << "no aligned" << std::endl;
	return p;
}

void DefaultFreeObject(void *p)
{
	return MemoryManager::freeObject(p);
}

int main()
{
		Timer t;
	const size_t N = 1000;
	const size_t M = 100000;
	PatateAllocTest* vec[N];
//#define PERFORMANCE_TEST
//#define STD_ALLOC_TEST
#define ALGN_MALLCO_TEST
#ifdef PERFORMANCE_TEST

	int spa = sizeof(PatateAllocTest);

	std::cout<<"\n***** SEQUENTIAL ALLOCATION DEALLOCATION *****"<<std::endl;

	for (int k=1;k<=20;++k)
	{
		std::cout<<"\n - - TEST N. "<<k<<" - - "<<std::endl;

		for(size_t i=0; i<N; ++i)
			vec[i] = 0;

		t.Start();

		for(size_t j = 0; j<N ; ++j)
		{
			vec[j] = AllocateWithDefultCategory();
		}
		for(size_t j = 0; j<N ; ++j)
		{
			DeallocateWithDefaultCategory(vec[j]);
			vec[j] = 0;
		}

		LONGLONG clock2 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With DefaultCategory -> "<<clock2<<std::endl;


		for(size_t i=0; i<N; ++i)
			vec[i] = 0;
		t.Start();

		for(size_t j = 0; j<N ; ++j)
		{
			vec[j] = AllocateWithStackCategory();
		}
		for(size_t j = N-1; j>=0 ; --j)
		{
			DeallocateWithStackCategoryPtr(vec[j]);
			vec[j] = 0;

			if(j==1)
				int p=0;
		}
		LONGLONG clock3 = t.TimeElapsedMicroSec();

		std::cout<<"TimeMicroSec Elapsed With StackCategory -> "<<clock3<<std::endl;

		for(size_t i=0; i<N; ++i)
			vec[i] = 0;

		t.Start();
		for(size_t j = 0; j<N; ++j)
		{		
			vec[j] = AllocateWithNew();
		}

		for(size_t j = 0; j<N; ++j)
		{		
			DeallocateWithNew(vec[j]);
			vec[j] = 0;
		}
		LONGLONG clock1 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With Standard -> "<<clock1<<std::endl;		
	}


	std::cout<<"\n***** RANDOM ALLOCATION DEALLOCATION *****"<<std::endl;


	for (int p=1;p<=20;++p)
	{

		std::cout<<"\n - - TEST N. "<<p<<" - - "<<std::endl;

		for(size_t i=0; i<N; ++i)
			vec[i] = 0;

		t.Start();
	
		for(size_t j = 0; j<M ; ++j)
		{
			const int r = rand()%N;
			if(vec[r] != 0)
			{
				DeallocateWithNew(vec[r]);
				vec[r] = 0;
			}
			else
			{
				vec[r] = AllocateWithNew();
			}
		}

		LONGLONG clock1 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With Standard -> "<<clock1<<std::endl;



		for(size_t i=0; i<N; ++i)
			vec[i] = 0;

		t.Start();

		for(size_t j = 0; j<M ; ++j)
		{
			const int r = rand()%N;
			if(vec[r])
			{
				DeallocateWithDefaultCategory(vec[r]);
				vec[r] = 0;
			}
			else
			{
				vec[r] = AllocateWithDefultCategory();
			}
		}

		LONGLONG clock2 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With DefaultCategory -> "<<clock2<<std::endl;
	}

	std::cout<<"\n***** RANDOM ALLOCATION DEALLOCATION WITH NEW *****"<<std::endl;
	
	for(size_t i=0; i<N; ++i)
			vec[i] = 0;

	t.Start();
	for(size_t j = 0; j<M ; ++j)
	{
		const int r = rand()%N;
		if(vec[r])
		{
			DeleteWithDefaultCategory(vec[r]);
			vec[r] = 0;
		}
		else
		{
			vec[r] = AllocateWithDefultCategory();
		}
	}
	LONGLONG clock4 = t.TimeElapsedMicroSec();
	std::cout<<"TimeMicroSec Elapsed With DefultCategory with new -> " << clock4 << std::endl;

	for(size_t i=0; i<N; ++i)
			vec[i] = 0;

	t.Start();
	for(size_t j = 0; j<M ; ++j)
	{
		const int r = rand()%N;
		if(vec[r])
		{
			DeleteWithStackCategory(vec[r]);
			vec[r] = 0;
		}
		else
		{
			vec[r] = NewWithStackCategory();
		}
	}
	LONGLONG clock5 = t.TimeElapsedMicroSec();
	std::cout<<"TimeMicroSec Elapsed With StackCategory with new -> " << clock5 << std::endl;

	for(size_t i=0; i<N; ++i)
		vec[i] = 0;

	t.Start();

	for(size_t j = 0; j<M ; ++j)
	{
		const int r = rand()%N;
		if(vec[r] != 0)
		{
			DeallocateWithNew(vec[r]);
			vec[r] = 0;
		}
		else
		{
			vec[r] = AllocateWithNew();
		}
	}

	LONGLONG clock6 = t.TimeElapsedMicroSec();
	std::cout<<"TimeMicroSec Elapsed With Standard -> "<<clock6<<std::endl;
#endif // PERFORMANCE_TEST
#ifdef STD_ALLOC_TEST

	std::vector<int,StandardAllocator<int>> ciao(10,1);
	std::deque<int,StandardAllocator<int>> deck(20,2);
	std::list<int,StandardAllocator<int>> klist(20,3);

	ciao[2] = 4;
	std::cout << ciao[1]<< std::endl;
	ciao.resize(200);


	//int * pollo = static_cast<int *>(MemoryManager::mallocObject<SmallObject_Category<Stack_Object_Allocator<>>>(1000));
	//int * pollo1 = static_cast<int *>(MemoryManager::mallocObject<SmallObject_Category<>>(24));
	//int * pollo2 = static_cast<int *>(MemoryManager::mallocObject<SmallObject_Category<Stack_Object_Allocator<>>>(1000));
	//B<A> p;

	StandardAllocator<int> p;
	int *poi=p.allocate(3);
	p.construct(poi,5);
	std::cout << *poi<< std::endl;
//  #pragma pack (push)//packata
//  #pragma pack (1)
	struct tmp1
	{
		char cc;
		//UINT16 ii;
	};
//#pragma pack (pop)
	tmp1* puppo = static_cast<tmp1*>(calloc(1,sizeof(tmp1)));
	puppo->cc=25;
	//puppo->ii=12;
	std::cout << "struct First sizeof: "<< sizeof(*puppo) << "  alignof: " << __alignof(tmp1) << std::endl;

	__declspec(align(4)) struct tmp
	{
		int ii;
		char cc;
	};
	tmp* lilla = static_cast<tmp*>(calloc(1,sizeof(tmp)));
	lilla->cc=25;
	lilla->ii=12;
	std::cout << "struct First sizeof: "<< sizeof(*lilla) << "  alignof: " << __alignof(tmp) << std::endl;

	int* polo = static_cast<int*>(mmal<int>());
	//assert((int)polo % 16 == 0);
	*polo=2;
	std::cout << *polo << std::endl;
	//std::cout << "struct First sizeof: "<< sizeof(int) << "  alignof: " << __alignof(int) << std::endl;
	//std::cout << "struct First sizeof: "<< sizeof(polo) << "  alignof: " << __alignof(polo) << std::endl;
#endif // STD_ALLOC_TEST
#ifdef ALGN_MALLCO_TEST
	std::cout<<"\n - - TEST ALGN - - "<<std::endl;

	for(size_t i=0; i<N; ++i)
		vec[i] = 0;

	t.Start();

	for(size_t j = 0; j<M ; ++j)
	{
		const int r = rand()%N;
		if(vec[r] != 0)
		{
			DefaultFreeObject(vec[r]);
			vec[r] = 0;
		}
		else
		{
			vec[r] = DefaultMallocWithAlignmet();
		}
	}

	LONGLONG clockalgn = t.TimeElapsedMicroSec();
	std::cout<<"TimeMicroSec Elapsed With Standard -> "<<clockalgn<<std::endl;
#endif // ALGNMENT MALLOC TEST
	getchar();
	return 0;
}