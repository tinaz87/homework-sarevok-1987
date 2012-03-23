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
		printf("Inserire il nome del file\n Es:\n c:\\lol\\asd> WaterCalc.exe input.txt");
		getchar();
		exit(1);
	}

	ArrayMatrix mt=NULL;
	ipair RxC;
	ipair MinMax;
	mt=control_file(argv[1],RxC,MinMax);
	MyMatrix WaterMatrix(mt,RxC);
	std::cout << WaterMatrix;
	for (int i = MinMax.first + 1; i <= MinMax.second; ++i)
	{
		WaterMatrix.scanLevel(i);
		std::cout << WaterMatrix;
		WaterMatrix.isBorderFall();
		std::cout << WaterMatrix;
		WaterMatrix.blockFallHoles();
		std::cout << WaterMatrix;
		WaterMatrix.readHoles();
		std::cout << WaterMatrix;
	}

	std::cout << "The volume is: " << WaterMatrix.getVolumeWater() << "L" << std::endl << std::endl;
	getchar();

	//MyMatrix mymt(RxC.first,RxC.second);
	//for (int i=0; i<RxC.first;++i)
	//{
	//	for(int j=0;j<RxC.second;++j)
	//	{
	//		mymt.my_insert(std::make_pair(i,j),mt[i][j]);
	//	}
	//}
	//std::cout << mymt;
	//mymt.flush_near();
	//std::cout << mymt;
	//mymt.label_pool();
	//std::cout << mymt;
	//mymt.create_llist();
	//std::cout << mymt;
	//mymt.fix_llist();
	//std::cout << mymt;
	//mymt.spread_link();
	//std::cout << mymt;
	//mymt.calc_pool();
	//std::cout << mymt;
	//mymt.fix_lack();
	//std::cout << mymt;
	//std::cout << "Matrix contain:" << mymt.calc_gain() << " lt water" << std::endl;
	//std::cout << mymt;
	//getchar();
	return 0;
}
//ipair RxC = std::make_pair(righe,colonne);
//print_matrix(mt,RxC);
//
//int min=0;
//int max=0;
//int i=0;
//int i1=0;
//int j=0;
//int j1=0;
//ipair cell_min;
//ipair cell_max;
//Max(mt,righe,colonne,max,cell_max);
//std::cout << "Min in matrix: "<< max << "in M["<< cell_max.first << ";" << cell_max.second << "]" << std::endl;
//Min(mt,righe,colonne,min,cell_min);
//std::cout << "Min in matrix: "<< min << "in M["<< cell_min.first << ";" << cell_min.second << "]" << std::endl;
//ideque open_cell;
//ideque closed_cell;
//open_cell.push_front(cell_min);
//int **visited_matrix=NULL;
//int **level_matrix=NULL;
//visited_matrix=create_matrix(righe,colonne);
//copy_matrix(visited_matrix,mt,RxC);
//ipair *val=NULL;
//for(int j=min+1;j<=max+1;j++)
//{
//	Min(visited_matrix,righe,colonne,min,cell_min);
//	open_cell.push_front(cell_min);
//	//level_matrix=create_matrix(righe,colonne);
//	while (!open_cell.empty())
//	{
//		ipair tmp = open_cell.front();
//		my_four_near(mt,RxC,tmp,&val);
//		open_cell.pop_front();
//		if(mt[tmp.first][tmp.second]<j)
//		{
//			for(int i=0;i<4;++i)
//			{
//				if((val[i].first!=-1)&&(val[i].second!=-1))
//				{
//					if((level_matrix[val[i].first][val[i].second]!=-1)&&(visited_matrix[val[i].first][val[i].second]!=-1))
//						open_cell.push_front(val[i]);
//				}
//			}
//		}
//		closed_cell.push_front(tmp);
//		level_matrix[tmp.first][tmp.second]=-1;
//		visited_matrix[tmp.first][tmp.second]=-1;
//
//		print_ideque(open_cell);
//		print_ideque(closed_cell);
//		print_matrix(level_matrix,RxC);
//	}
//	print_matrix(visited_matrix,RxC);
//int **mP=NULL;
//mP = new int*[righe];
//for (int z = 0; z < righe; ++z)
//	mP[z] = new int[colonne];
//for(int k=0;k<righe*colonne;++k)
//{
//	int *val= NULL;
//	my_four_near(mt,righe,colonne,i,j,&val);
//	std::cout << "M["<< i << ";" << j-1 << "]="<< val[0] << std::endl;
//	std::cout << "M["<< i << ";" << j+1 << "]="<< val[1] << std::endl;
//	std::cout << "M["<< i-1   << ";" << j << "]="<< val[2] << std::endl;
//	std::cout << "M["<< i+1   << ";" << j << "]="<< val[3] << std::endl;
//	int i1=0;
//	int j1=0;
//	min_my_four_near(val,i,j,min,i1,j1);
//	std::cout << "Min in my 4 near: "<< min << "in M["<< i1 << ";" << j1 << "]" << std::endl;
//	mP[i][j]=min - mt[i][j];
//	std::cout << "Guadagno " << mt[i][j] << "-" << min;
//	std::cout << "= " << mt[i][j] - min << std::endl; 
//	mt[i][j]=-1;
//	i=i1;
//	j=j1;
//}

	//for(int level=min;level<=max;++level)
	//{
	//	search_in_mylevel(mt,righe,colonne,level,visited_matrix);
	//	print_matrix(visited_matrix,righe,colonne);
	//	check_level(visited_matrix,righe,colonne,level,chk);
	//	/*if(chk==false)
	//		update_mylevel(mP,righe,colonne,level-1,level);
	//	else
	//		std::cout << "No water contained" ;
	//	print_matrix(visited_matrix,righe,colonne);
	//}

//std::pair<int,int> my_pair;
//my_pair = std::make_pair(0,2);
//std::deque<int> open_cell;
//std::deque<int> closed_cell;
//open_cell.push_front(2);
//std::cout << open_cell.front();
//std::cout << "[" << my_pair.first << ";" << my_pair.second << "]";