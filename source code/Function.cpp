// Function.cpp: implementation of the Function class.
//
//////////////////////////////////////////////////////////////////////

#include "Function.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Function::Function(string name):Symbol(name)
{i = 9;
}

Function::~Function()
{
}

void Function::add_param(Variable v)
{	
	if(in_paramtable(v.get_name()))           //if param already declared...
		error_handler.error("duplicate parameter name " + v.get_name() + ".");
	else
	paramtable.push_back(v);                  //add the param to the function
}

int Function::get_param_count()               //get the # of params
{
	return paramtable.size(); 
}

string Function::get_func_header()            //get func/params info
{
	string param_list = name + "(";  
	
	for(int i = 0;i < paramtable.size();i++)
	{
		if(i > 0)                            //if # params processed > 0
			param_list += ",";               //add ","
		param_list += paramtable[i].get_type() + " " + paramtable[i].get_name();
		
	}
	param_list += ")";

	return param_list;                
}

bool Function::in_paramtable(string param_name) //check if param declared
{
	for(int i = 0;i < paramtable.size();i++)
		if(paramtable[i] == param_name)	      
			return true;                       
	return false;
}