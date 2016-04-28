// Error.cpp: implementation of the Error_handler class.
//
//////////////////////////////////////////////////////////////////////

#include "Error.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Error_handler::Error_handler()
{
	line_counter = 1;
}

Error_handler::~Error_handler()
{

}

void Error_handler::inc_line_counter()
{
	line_counter++;                      //to next line
}

void Error_handler::dec_line_counter()
{
	line_counter--;                      //to previous line
}

void Error_handler::error(string err_msg)  //output err msg & exit
{
	cout << "\n\t " << "Error on line " << line_counter << ": " << err_msg << endl;
	exit(0);
}

void Error_handler::expected(string expect,string instead_of)
{
	error("expected '" + expect + "' instead of '" + instead_of + "'.");
}

