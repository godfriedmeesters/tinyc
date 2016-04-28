// Variable.cpp: implementation of the Variable class.
//
//////////////////////////////////////////////////////////////////////

#include "Variable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Variable::Variable(string type,string name,bool arr = false):Symbol(name)
{
	if(type != "BYTE" && type != "WORD")
		error_handler.error("expected 'BYTE' or 'WORD'");
	this->type = type;
	this->arr = arr;
}

Variable::~Variable()
{
}

string Variable::get_type()				             //get type string
{
	return type;                                   
}

int Variable::get_type_size()                        //get size in bytes
{
	return type == "WORD" ? 2 : 1;
}

bool Variable::is_arr()                              //arr or var?
{
	return arr;
}

