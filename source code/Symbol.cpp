// Symbol.cpp: implementation of the Symbol class.
//
//////////////////////////////////////////////////////////////////////

#include "Symbol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Symbol::Symbol(string name)
{
	if(isalpha(name.at(0)))
		this->name = name;
	else
		error_handler.error("invalid identifier " + name + ".");
}

Symbol::~Symbol()
{
}

bool Symbol::operator==(string name)  //if a symbol object is compared, use "name"
{
	return this->name == name;
}

string Symbol::get_name()             //return the symbol name
{
	return name;
}
