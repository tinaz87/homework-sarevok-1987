// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <utility>
#include <vector>
#include "LoadFileInput.h"
#include "MyMatrix.h"
#include "MyTypeDef.h"

int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Insert name of file\n Es:\n C:\\lol\\asd> WaterCalc.exe input.txt");
		getchar();
		exit(1);
	}

	ArrayMatrix mt=NULL;
	ipair RxC;
	ipair MinMax;
	mt=controlFile(argv[1],RxC,MinMax);
	MyMatrix WaterMatrix(mt,RxC);
	std::cout << WaterMatrix;
	for (int i = MinMax.first + 1; i <= MinMax.second; ++i)
	{
		WaterMatrix.scanLevel(i);
		WaterMatrix.isEdgeLack();
		WaterMatrix.spreadLack();
		WaterMatrix.countLevelPuddles();
	}
	std::cout << "The volume is: " << WaterMatrix.getVolumeWater() << "L" << std::endl;
	getchar();
	return 0;
}