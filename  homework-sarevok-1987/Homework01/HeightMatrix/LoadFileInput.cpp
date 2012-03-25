#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <utility>
#include <intsafe.h>
#include "LoadFileInput.h"
#include <basetsd.h>

void fillRow( std::string i_line,int i_Column,int *o_Row, ipair& o_MinMax )
{
	size_t found=0;
	int nColumn=0;
	std::string line_tmp=i_line + " ";//to take last number
	while ((found=line_tmp.find(' '))!=std::string::npos)//splitter of column is ' '
	{
		std::string str=line_tmp.substr(0,found);
		if(nColumn < i_Column)
		{
			o_Row[nColumn] = atoi(str.c_str());
			if(o_Row[nColumn] < o_MinMax.first)
				o_MinMax.first = o_Row[nColumn];
			if(o_Row[nColumn] > o_MinMax.second)
				o_MinMax.second = o_Row[nColumn];
		}
		else
		{
			std::cout << "File input.txt error: too many column" << std::endl;
			getchar();
			exit(1);
		}
		++nColumn;
		line_tmp = line_tmp.substr(found+1);
	}
}

void controlStartLine( std::string i_line,int &o_nRow,int &o_nColumn )
{
	size_t found=i_line.find('x');
	if (found!=std::string::npos)
	{
		std::string str = i_line.substr(0,found);
		o_nRow = atoi(str.c_str());
		str = i_line.substr(found+1);
		o_nColumn = atoi(str.c_str());
	}
	else
	{
		std::cout << "File input.txt error: missing char 'x' in row columns declaration" << std::endl;
		getchar();
		exit(1);
	}
	return;
}

ArrayMatrix controlFile( char *i_fileName,ipair& o_RowAndColumn,ipair& o_MinMax )
{
	std::ifstream i_file(i_fileName);
	int n_row=0;
	int n_row_mt =0;
	int n_column_mt=0;
	std::string line;
	int **mt;
	o_MinMax.first = 2000000000;
	o_MinMax.second = -20000000;
	if (i_file.is_open())
	{
		while ( i_file.good() )
		{
			getline (i_file,line);
			if(n_row==0)
			{
				controlStartLine(line,n_row_mt,n_column_mt);
				mt = new int*[n_row_mt];
				for (int i = 0; i < n_row_mt; ++i)
					mt[i] = new int[n_column_mt];
			}
			else
			{
				if(n_row-1 < n_row_mt)
				{
					fillRow(line,n_column_mt,mt[n_row-1],o_MinMax);
				}
				else
				{
					std::cout << "File input.txt error: too many row" << std::endl;
					getchar();
					exit(1);
				}
			}
			++n_row;
		}
		if(n_row==0)
		{
			std::cout << "File input.txt error: empty" << std::endl;
			getchar();
			exit(1);
		}
		if(n_row == 1)
		{
			std::cout << "File input.txt error: missing matrix" << std::endl;
			getchar();
			exit(1);
		}
		i_file.close();
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}
	o_RowAndColumn = std::make_pair(n_row_mt,n_column_mt);
	return mt;
}