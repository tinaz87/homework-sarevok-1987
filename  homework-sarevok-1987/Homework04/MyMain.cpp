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

template < typename T>
class PolloA
{
	T lol;

public:
	T provenienza;

	PolloA(){}
	//virtual void arrosto();
};

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

void DeallocateWithStackCategory(){

	MemoryManager::freeObjectCategory<Stack_Category<>>();
}


PatateAllocTest* NewWithDefaultCategory(){

	return  MemoryManager::newObjectCategory<PatateAllocTest,SmallObject_Category<>>(1);

}

void DeleteWithDefaultCategory(PatateAllocTest* p){

	MemoryManager::deleteObjectCategory<SmallObject_Category<>>(p);

}


PatateAllocTest* NewWithStackCategory(){


	return MemoryManager::newObjectCategory<PatateAllocTest,Stack_Category<>>(1);
}

void DeleteWithStackCategory(){

	MemoryManager::deleteObjectCategory<Stack_Category<>>();
}

template <typename T>
void* mmal()
{
	typedef typename __declspec(align(16)) T newAlgn;
	newAlgn* tmp = new newAlgn(0);
	std::cout << "struct First sizeof: "<< sizeof(int) << "  alignof: " << __alignof(int) << std::endl;
	std::cout << "struct First sizeof: "<< sizeof(*tmp) << "  alignof: " << __alignof(tmp) << std::endl;
	return tmp;
}

int main()
{
	/*int* p;
	p = new int;

	std::vector<PolloA<Patate>> p1;
	p1.push_back(*(new PolloA<Patate>()));*/

	//std::vector<int> pollo(3,1);
	//Default_Chunk chunk;
	//char * p;
	//if(chunk.Init(sizeof(char),10))
	//{
	//	p= static_cast<char *>(chunk.Allocate(3));
	//}
	//p[0]='f';
	//p[1]='o';
	//p[2]='o';
	//std::cout << sizeof(ChunkInterfaces);

	//Default_Allocator<>* fixAllocator;
	//fixAllocator.Initialize(sizeof(char),10);
	//char * p;
	//p= static_cast<char *>(fixAllocator.Allocate());
	//p[0]='f';
	//p[1]='o';
	//p[2]='o';

	//fixAllocator.Deallocate(p,&chunk);

	//Default_Object_Allocator<> objAlloc(4096,256,4);
	//char *p = static_cast<char *>(objAlloc.Allocate(3));
	//p[0]='f';
	//p[1]='o';
	//p[2]='o';

	//char * p = static_cast<char *>(MemoryManager::mallocObject<SmallObject_Category<>>(sizeof(char)*4));
	//p[0]='f';
	//p[1]='o';
	//p[2]='o';
	//p[3]='o';
	//p[4]='o';
	//int* i = static_cast<int *>(MemoryManager::mallocObject(sizeof(int)*4));
	//i[0]=346546;
	//MemoryManager::freeObject(p);
	//p = static_cast<char *>(MemoryManager::mallocObject(sizeof(char)*4));
	//std::cout << *p;
	//
	//char* pc = static_cast<char *>(MemoryManager::newObject(2));
	//MemoryManager::deleteObject(pc);

	//SingletonHolder<std::vector<int>>::Instance().begin();
	//SmartPtr<int> smtptr1(new int(2));
	//SmartPtr<int> smtptr2(smtptr1);
	//SmartPtr<int> smtptr3(new int(3));
	//smtptr1 = smtptr3;

	//std::cout << std::endl << *smtptr1;
	//std::cout << std::endl << *smtptr2;
	//std::cout << std::endl << *smtptr3;


	//// TODO Implementare System Allocator (new e delete)
	//int *p90=static_cast<int *>(MemoryManager::mallocObject(11 * sizeof(int)));
	//MemoryManager::freeObject(p90);

	//Object_Stack_Allocator<> prova(4096);
	//unsigned char *p67=static_cast<unsigned char*>(prova.Allocate(33));

	//Simple_Chunk provaChunk;
	//provaChunk.Init(4096);
	//unsigned char *p67=static_cast<unsigned char*>(provaChunk.Allocate(33 * sizeof(char)));
	//for(int i = 0; i < 33 ; ++i)
	//	p67[i]='a'+i;
	//unsigned char *p68=static_cast<unsigned char*>(provaChunk.Allocate(32));
	//
	//	p68[i]='a'+i;
	//provaChunk.Deallocate(p68,32);
/*

	
	char * p45 = static_cast<char *>(MemoryManager::mallocObject<SmallObject_Category<>>(sizeof(char)*4));

	int * p46 = static_cast<int *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(int)*4));

	char * p47 = static_cast<char *>(MemoryManager::mallocObject<SmallObject_Category<>>(sizeof(char)*4));

	int * p48 = static_cast<int *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(int)*4));

	char * p49 = static_cast<char *>(MemoryManager::mallocObject<SmallObject_Category<>>(sizeof(char)*4));

	int * p50 = static_cast<int *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(int)*4));


	char * p51 = static_cast<char *>(MemoryManager::mallocObject<SmallObject_Category<>>(sizeof(char)*4));

	UINT * p52 = static_cast<UINT *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(UINT)*4));

	char * p53 = static_cast<char *>(MemoryManager::mallocObject<SmallObject_Category<>>(sizeof(char)*4));

	UINT64 * p54 = static_cast<UINT64 *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(UINT64)*8));

	char * p55 = static_cast<char *>(MemoryManager::mallocObject<SmallObject_Category<>>(sizeof(char)*4));

	UINT16 * p56 = static_cast<UINT16 *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(UINT16)*4));

	char * p57 = static_cast<char *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(char)*4));

	PatateAllocTest * potato = static_cast<PatateAllocTest *>(MemoryManager::mallocObject<Stack_Category<>>(sizeof(PatateAllocTest)*4));
	




	for(int i = 0; i < 4 ; ++i)
	{
		p46[i]=i*2;
	}

	MemoryManager::freeObject<Stack_Category<>>();
	MemoryManager::freeObject<SmallObject_Category<>>(p45);*/

//#define PERFORMANCE_TEST
#ifdef PERFORMANCE_TEST

	Timer t;
	const size_t N = 1000;
	const size_t M = 100000;
	PatateAllocTest* vec[N];
	
	LONGLONG clock2=0;
	LONGLONG clock3=0;
	LONGLONG clock1=0;


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

		clock2 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With DefaultCategory -> "<<clock2<<std::endl;


		for(size_t i=0; i<N; ++i)
			vec[i] = 0;
		t.Start();

		for(size_t j = 0; j<N ; ++j)
		{
			vec[j] = AllocateWithStackCategory();
		}
		for(size_t j = 0; j<N ; ++j)
		{
			DeallocateWithStackCategory();
			vec[j] = 0;
		}
		clock3 = t.TimeElapsedMicroSec();

		std::cout<<"TimeMicroSec Elapsed With StackCategory -> "<<clock3<<std::endl;

		for(size_t i=0; i<N; ++i)
			vec[i] = 0;

		t.Start();
		//for(size_t j = 0; j<M ; ++j)
		//{
		//	const int r = rand()%N;
		//	if(vec[r])
		//	{
		//		DeallocateWithNew(vec[r]);
		//		vec[r] = 0;
		//	}
		//	else
		//	{
		//		vec[r] = AllocateWithNew();
		//	}
		//}
		for(size_t j = 0; j<N; ++j)
		{		
			vec[j] = AllocateWithNew();
		}

		for(size_t j = 0; j<N; ++j)
		{		
			DeallocateWithNew(vec[j]);
			vec[j] = 0;
		}
		clock1 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With Standard -> "<<clock1<<std::endl;

		/*
		for(size_t i=0; i<N; ++i)
			vec[i] = 0;

		t.Start();
		*/
		//for(size_t j = 0; j<M ; ++j)
		//{
		//	const int r = rand()%N;
		//	if(vec[r])
		//	{
		//		DeallocateWithDefaultCategory(vec[r]);
		//		vec[r] = 0;
		//	}
		//	else
		//	{
		//		vec[r] = AllocateWithDefultCategory();
		//	}
		//}
		
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

		clock1 = t.TimeElapsedMicroSec();
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

		clock2 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With DefaultCategory -> "<<clock2<<std::endl;



		for(size_t i=0; i<N; ++i)
			vec[i] = 0;

		t.Start();

		for(size_t j = 0; j<M ; ++j)
		{
			const int r = rand()%N;
			if(vec[r])
			{
				DeallocateWithStackCategory();
				vec[r] = 0;
			}
			else
			{
				vec[r] = AllocateWithStackCategory();
			}
		}

		clock3 = t.TimeElapsedMicroSec();
		std::cout<<"TimeMicroSec Elapsed With StackCategory -> "<<clock3<<std::endl;
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
			DeleteWithStackCategory();
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
	getchar();
	return 0;
}