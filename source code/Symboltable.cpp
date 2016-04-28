// Symboltable.cpp: implementation of the Symtablemanager class.
//
//////////////////////////////////////////////////////////////////////

#include "Symboltable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Symtablemanager::Symtablemanager()
{
}

Symtablemanager::~Symtablemanager()
{	
}

void Symtablemanager::add_function(string name)
{
	if(!in_functable(name))                        //if function not declared
			functable.push_back(Function(name));		//add function to functable
	else
		error_handler.error("duplicate function name " + name + ".");
}

bool Symtablemanager::in_functable(string func_name)  //check if function declared
{
	for(int i = 0;i < functable.size();i++)
		if(functable[i] == func_name)	
			return true;
	return false;
}

Function &Symtablemanager::function(string func_name)  //return declared Function object
{
	for(int i = 0;i < functable.size();i++)
	{
		if(functable[i] == func_name)
			return functable[i];
	}
	error_handler.error(func_name + " is not a valid function name.");

	return functable[0];                  //*only* to suppress compiler warning
}

void Symtablemanager::add_loc_var(string type,string name,bool is_arr,int offset)
{
	if(in_loc_vartable(name))          //if var declared
		error_handler.error("duplicate local variable " + name + ".");
	else                               //add var to vartable
		loc_vartable.push_back(Local_variable(type,name,is_arr,offset));
}

bool Symtablemanager::in_loc_vartable(string loc_var_name) //check if var declared
{
	for(int i = 0;i < loc_vartable.size();i++)
		if(loc_vartable[i] == loc_var_name)	 
			return true;                               //found
	return false;                                      //not found
}

Local_variable Symtablemanager::loc_var(string name) //return declared Loc_var object
{
	for(int i = 0;i < loc_vartable.size();i++)
		if(loc_vartable[i] == name)	
			return loc_vartable[i]; 

	error_handler.error("variable " + name + " not declared.");

	return loc_vartable[0];        //*only* to suppress compiler warning
}

void Symtablemanager::clear_loc_vars()
{
	loc_vartable.clear();                 //clear CURR function's loc vars
}

