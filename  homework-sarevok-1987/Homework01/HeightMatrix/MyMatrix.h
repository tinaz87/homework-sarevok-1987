#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <deque>
#include "MyTypeDef.h"

class MyMatrix
{
	ArrayMatrix m_Matrix;//Matrix with heath value.
	ArrayMatrix m_levelInfMatrix;//Temp Matrix for level information such as leaks and puddles.
	int m_Row;
	int m_Column;
	ideque m_leaksIndex;//Index of matrix element that is leak
	int m_volume;

public:

	MyMatrix(const ArrayMatrix& i_mt, const ipair& i_RxC)
		: m_Matrix(i_mt)
		, m_Row(i_RxC.first)
		, m_Column(i_RxC.second)
		, m_volume(0)
	{
		m_levelInfMatrix= new int*[m_Row];
		for (int i = 0; i < m_Row; ++i)
			m_levelInfMatrix[i] = new int[m_Column];
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{
				m_levelInfMatrix[i][j] = 0;
			}
		}
	}

	void clearLevelInfMatrix()
	{
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{
				m_levelInfMatrix[i][j] = 0;
			}
		}
	}
	//If the element M[i_Row][i_Column] is leaks and its nears are puddle then they are leaks too
	void checkLeakNear(int i_Row,int i_Column)
	{
		if((i_Row > 0)&&(m_levelInfMatrix[i_Row-1][i_Column] == 0))
		{
			m_levelInfMatrix[i_Row-1][i_Column] = 2;
			m_leaksIndex.push_back(std::make_pair(i_Row-1,i_Column));
		}
		if((i_Column < m_Column - 1)&&(m_levelInfMatrix[i_Row][i_Column+1] == 0))
		{
			m_levelInfMatrix[i_Row][i_Column+1] = 2;
			m_leaksIndex.push_back(std::make_pair(i_Row,i_Column+1));
		}
		if((i_Row < m_Row - 1)&&(m_levelInfMatrix[i_Row+1][i_Column] == 0))
		{
			m_levelInfMatrix[i_Row+1][i_Column] = 2;
			m_leaksIndex.push_back(std::make_pair(i_Row+1,i_Column));
		}
		if((i_Column > 0)&&(m_levelInfMatrix[i_Row][i_Column-1] == 0))
		{
			m_levelInfMatrix[i_Row][i_Column-1] = 2;
			m_leaksIndex.push_back(std::make_pair(i_Row,i_Column-1));
		}
	}
	//Cheak if there are some puddle at this level
	void scanLevel(int level)
	{
		clearLevelInfMatrix();
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{

				if (m_Matrix[i][j] < level)
					m_levelInfMatrix[i][j] = 0;
				else
					m_levelInfMatrix[i][j] = 1;
			}
		}
		m_leaksIndex.clear();
		return;
	}
	//Check the edges of matrix and if there is a leak it'll marked in levelMatrixInfo
	void isEdgeLeak()
	{
		for (int i = 0; i < m_Row; ++i)
		{
			if (m_levelInfMatrix[i][0] == 0)
			{
				m_levelInfMatrix[i][0] = 2;
				m_leaksIndex.push_back(std::make_pair(i,0));
			}
			if (m_levelInfMatrix[i][m_Column-1] == 0)
			{
				m_levelInfMatrix[i][m_Column-1] = 2;
				m_leaksIndex.push_back(std::make_pair(i,m_Column-1));
			}
		}
		for (int j=0 ; j < m_Column; ++j)
		{
			if (m_levelInfMatrix[0][j] == 0)
			{
				m_levelInfMatrix[0][j] = 2;
				m_leaksIndex.push_back(std::make_pair(0,j));
			}
			if (m_levelInfMatrix[m_Row-1][j] == 0)
			{
				m_levelInfMatrix[m_Row-1][j] = 2;
				m_leaksIndex.push_back(std::make_pair(m_Row-1,j));
			}
		}
	}
	//Spread Leak to the nears
	void spreadLeak()
	{
		while(!m_leaksIndex.empty())
		{
			checkLeakNear(m_leaksIndex.front().first,m_leaksIndex.front().second);
			m_leaksIndex.pop_front();
		}
	}
	//Sum the puddles gain
	void countLevelPuddles()
	{
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{
				if (m_levelInfMatrix[i][j] == 0)
				{
					++m_volume;
				}
			}
		}
	}
	//Return sum the all puddles
	int getVolumeWater()
	{
		return m_volume;
	}

	friend std::ostream& operator<<(std::ostream& i_os, const MyMatrix& i_mt);
};

std::ostream& operator<<(std::ostream& i_os, const MyMatrix& i_mt)
{
	for(int rowCnt = 0; rowCnt < i_mt.m_Row; ++rowCnt)
	{
		for(int colCnt = 0; colCnt < i_mt.m_Column; ++colCnt)
		{
			i_os << i_mt.m_Matrix[rowCnt][colCnt] << "  ";
		}
		i_os << std::endl;
	}
	i_os << std::endl;
	for(int rowCnt = 0; rowCnt < i_mt.m_Row; ++rowCnt)
	{
		for(int colCnt = 0; colCnt < i_mt.m_Column; ++colCnt)
		{
			i_os << i_mt.m_tmpMatrix[rowCnt][colCnt] << "  ";
		}
		i_os << std::endl;
	}
	i_os << std::endl;
	return i_os;
}