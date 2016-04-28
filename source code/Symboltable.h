// Symboltable.h: interface for the Symbtablemanager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLTABLE_H__032F6A98_C3B8_4092_B29A_D405C11F04DF__INCLUDED_)
#define AFX_SYMBOLTABLE_H__032F6A98_C3B8_4092_B29A_D405C11F04DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Local_variable.h"
#include "Function.h"
#include <vector>
using namespace std;

extern Error_handler error_handler;                 //shared error handler object

class Symtablemanager  
{
private:
	vector<Local_variable> loc_vartable;           //table of CURR function's loc vars
	vector<Function> functable;

public:
	Symtablemanager();
	virtual ~Symtablemanager();
	

	void add_function(string name);                 //add function to func table
	bool in_functable(string func_name);          //check if function declared
	Function &function(string func_name);           //return a Function object

	void add_loc_var(string type,string name,bool is_arr,int offset);
	bool in_loc_vartable(string loc_var_name);     //check if loc var/param declared
	Local_variable loc_var(string name);           //return a Loc_variable object
	
	void clear_loc_vars();                         //clear CURR function's loc vars
	
};

#endif // !defined(AFX_SYMBOLTABLE_H__032F6A98_C3B8_4092_B29A_D405C11F04DF__INCLUDED_)
