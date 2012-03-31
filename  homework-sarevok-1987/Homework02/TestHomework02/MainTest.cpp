#include "BigInt.h"
#include "FunctionTest.h"

int main()
{
	test_ctor();
 	std::cout << std::endl;
 	test_sum_and_sub();
	std::cout << std::endl;
	test_prod_div();
	int test = 7;
	int res = 3;
	test^=res;
	std::cout <<  test << std::endl;

	BigInt test1(7);
	BigInt res1 = 3;
	test1^=res1;
	std::cout << test1 << std::endl;

	BigInt ka1(9999);
	BigInt ka2(9999);
	
	ka1*=ka2;
	std::cout << ka1 << std::endl;
	getchar();
}