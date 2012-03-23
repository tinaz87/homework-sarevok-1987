#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <deque>
#include "MyTypeDef.h"

class MyMatrix
{
	ArrayMatrix m_Matrix;
	ArrayMatrix m_tmpMatrix;
	int m_Row;
	int m_Column;
	ideque m_holesIndex;
	int m_volume;

public:

	MyMatrix(const ArrayMatrix& i_mt, const ipair& i_RxC)
		: m_Matrix(i_mt)
		, m_Row(i_RxC.first)
		, m_Column(i_RxC.second)
		, m_volume(0)
	{
		m_tmpMatrix= new int*[m_Row];
		for (int i = 0; i < m_Row; ++i)
			m_tmpMatrix[i] = new int[m_Column];
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{
				m_tmpMatrix[i][j] = 0;
			}
		}
	}

	void clearTmpMatrix()
	{
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{
				m_tmpMatrix[i][j] = 0;
			}
		}
	}

	void checkNear(int i_Row,int i_Column)
	{
		if((i_Row > 0)&&(m_tmpMatrix[i_Row-1][i_Column] == 0))
		{
			m_tmpMatrix[i_Row-1][i_Column] = 2;
			m_holesIndex.push_back(std::make_pair(i_Row-1,i_Column));
		}
		if((i_Column < m_Column - 1)&&(m_tmpMatrix[i_Row][i_Column+1] == 0))
		{
			m_tmpMatrix[i_Row][i_Column+1] = 2;
			m_holesIndex.push_back(std::make_pair(i_Row,i_Column+1));
		}
		if((i_Row < m_Row - 1)&&(m_tmpMatrix[i_Row+1][i_Column] == 0))
		{
			m_tmpMatrix[i_Row+1][i_Column] = 2;
			m_holesIndex.push_back(std::make_pair(i_Row+1,i_Column));
		}
		if((i_Column > 0)&&(m_tmpMatrix[i_Row][i_Column-1] == 0))
		{
			m_tmpMatrix[i_Row][i_Column-1] = 2;
			m_holesIndex.push_back(std::make_pair(i_Row,i_Column-1));
		}
	}

	void scanLevel(int level)
	{
		clearTmpMatrix();
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{

				if (m_Matrix[i][j] < level)
					m_tmpMatrix[i][j] = 0;
				else
					m_tmpMatrix[i][j] = 1;
			}
		}
		m_holesIndex.clear();
		return;
	}

	void isBorderFall()
	{
		for (int i = 0; i < m_Row; ++i)
		{
			if (m_tmpMatrix[i][0] == 0)
			{
				m_tmpMatrix[i][0] = 2;
				m_holesIndex.push_back(std::make_pair(i,0));
			}
			if (m_tmpMatrix[i][m_Column-1] == 0)
			{
				m_tmpMatrix[i][m_Column-1] = 2;
				m_holesIndex.push_back(std::make_pair(i,m_Column-1));
			}
		}
		for (int j=0 ; j < m_Column; ++j)
		{
			if (m_tmpMatrix[0][j] == 0)
			{
				m_tmpMatrix[0][j] = 2;
				m_holesIndex.push_back(std::make_pair(0,j));
			}
			if (m_tmpMatrix[m_Row-1][j] == 0)
			{
				m_tmpMatrix[m_Row-1][j] = 2;
				m_holesIndex.push_back(std::make_pair(m_Row-1,j));
			}
		}
	}

	void blockFallHoles()
	{
		while(!m_holesIndex.empty())
		{
			checkNear(m_holesIndex.front().first,m_holesIndex.front().second);
			m_holesIndex.pop_front();
		}
	}

	void readHoles()
	{
		for (int i = 0; i < m_Row; ++i)
		{
			for (int j=0 ; j < m_Column; ++j)
			{
				if (m_tmpMatrix[i][j] == 0)
				{
					++m_volume;
				}
			}
		}
	}

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