#include "BigInt.h"

void test_ctor()
{
	BigInt bi1;
	BigInt bi2(20);
	BigInt bi3("502");
	BigInt bi4 = 5;
	BigInt bi5 = bi2;
	BigInt bi6((-20));
	BigInt bi7("-200");
	BigInt bi8 = -40;
	std::cout << bi1 << " default ctor" << std::endl;
	std::cout << bi2 << " int ctor" << std::endl;
	std::cout << bi3 << " string ctor" << std::endl;
	std::cout << bi4 << " copy with int ctor" << std::endl;
	std::cout << bi5 << " copy with BigInt ctor" << std::endl;
	std::cout << bi6 << " ctor with negative int" << std::endl;
	std::cout << bi7 << " ctor with negative string" << std::endl;
	std::cout << bi8 << " copy ctor with negative int" << std::endl;
}

void test_sum_and_sub()
{
	BigInt bi1(10);
	BigInt bi2(20);
	BigInt bi3("100");
	BigInt bi4(-10);
	BigInt bi5(20);
	BigInt bi6(-100);
	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b3="<< bi3 << std::endl;
	bi3 += bi1;
	std::cout << "b3+=b1 = " << bi3 << " operator+=" << std::endl; 
	
	std::cout << "b2="<< bi2 << std::endl;
	std::cout << "b3="<< bi3 << std::endl;
	bi3 -= bi2;
	std::cout << "b3-=b2 = " << bi3 << " operator-=" << std::endl;

	std::cout << "b2="<< -bi2 << std::endl;
	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b2+b1 = " << -bi2 + bi1 << " operator+" << std::endl;

	std::cout << "b2="<< bi2 << std::endl;
	std::cout << "b1="<< -bi1 << std::endl;
	std::cout << "b1+b2 = " << -bi1 + bi2 << " operator+" << std::endl;

	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b2="<< -bi2 << std::endl;
	std::cout << "b1+b2 = " << bi1 + -bi2 << " operator+" << std::endl;

	std::cout << "b1="<< -bi1 << std::endl;
	std::cout << "b2="<< -bi2 << std::endl;
	std::cout << "b1+b2 = " << (-bi1) + (-bi2) << " operator+" << std::endl;

	std::cout << "b1="<< -bi1 << std::endl;
	std::cout << "b2="<< -bi2 << std::endl;
	std::cout << "b1+b2 = " << (-bi1) - (-bi2) << " operator-" << std::endl;

	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b2="<< bi2 << std::endl;
	std::cout << "b1-b2 = " << bi1 - bi2 << " operator-" << std::endl;

	std::cout << "b4="<< bi4 << std::endl;
	std::cout << "b5="<< bi5 << std::endl;
	std::cout << "b4+b5 = " << bi4 + bi5 << " operator+ with negative" << std::endl;

	std::cout << "b3="<< -bi3 << " operator- unary operator" << std::endl;
	std::cout << "b6="<< bi6 << std::endl;
	std::cout << "b3-b6 = " << -bi3 - bi6 << " operator+ with negative" << std::endl;

	std::cout << "b3="<< -bi3 << std::endl;
	std::cout << "b6="<< bi6 << std::endl;
	std::cout << "b3-b6 = " << (-bi3) - bi6 << " operator+ with negative" << std::endl;

	std::cout << "b6="<< bi6 << std::endl;
	std::cout << "b6-5 = " << bi6 - 5 << " operator+ with negative" << std::endl;
}

void test_prod_div()
{
	BigInt bi1(10);
	BigInt bi2(20);
	BigInt bi3("100");
	BigInt bi4(-10);
	BigInt bi5(20);
	BigInt bi6(-100);

	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b3="<< bi3 << std::endl;
	bi3 *= bi1;
	std::cout << "b3*=b1 = " << bi3 << " operator*=" << std::endl;

	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b3="<< bi3 << std::endl;
	std::cout << "b3*b1 = " << bi3*bi1 << " operator*=" << std::endl;

	std::cout << "b1="<< -bi1 << std::endl;
	std::cout << "b3="<< bi3 << std::endl;
	std::cout << "b3*b1 = " << bi3*-bi1 << " operator*=" << std::endl;

	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b3="<< -bi3 << std::endl;
	std::cout << "b3*b1 = " << -bi3*-bi1 << " operator*=" << std::endl;

	std::cout << "b5="<< bi5 << std::endl;
	std::cout << "b1="<< bi1 << std::endl;
	std::cout << "b1*b5 = " << bi1*bi5 << " operator*=" << std::endl;

	std::cout << "b5++="<< bi5++ << std::endl;
	std::cout << "++b5="<< ++bi5 << std::endl;
}