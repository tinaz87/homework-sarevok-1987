#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "MyTypeDef.h"
//Fill the row of matrix with row of file
void fillRow(std::string i_line,int i_Column,int *o_Row, ipair& o_MinMax);
//Take the number of row and column from first line
void controlStartLine(std::string i_line,int &o_nRow,int &o_nColumn);
//Control if file is good formatted and fill the matrix
ArrayMatrix controlFile(char *i_fileName,ipair& o_RowAndColumn,ipair& o_MinMax);