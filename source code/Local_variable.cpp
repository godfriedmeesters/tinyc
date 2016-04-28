// Local_variable.cpp: implementation of the Local_variable class.
//
//////////////////////////////////////////////////////////////////////

#include "Local_variable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Local_variable::Local_variable(string type,string name,
							   bool arr,int offset):Variable(type,name,arr)
{
	this->offset = offset;	
}

Local_variable::~Local_variable()
{
}

int Local_variable::get_offset()   //return offset from curr stack frame pointer
{
	return offset;
}

