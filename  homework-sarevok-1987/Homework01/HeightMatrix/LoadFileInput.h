#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "MyTypeDef.h"

void fill_row(std::string i_line,int i_Column,int *o_Row, ipair& o_MinMax);
void control_start_line(std::string i_line,int &o_nRow,int &o_nColumn);
ArrayMatrix control_file(char *i_fileName,ipair& o_RowAndColumn,ipair& o_MinMax);