// Function.h: interface for the Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTION_H__24FD42A3_661C_4F80_8BBD_925CA5391733__INCLUDED_)
#define AFX_FUNCTION_H__24FD42A3_661C_4F80_8BBD_925CA5391733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Symbol.h"
#include "Variable.h"
#include <vector>
using namespace std;



class Function:public Symbol  
{
private:

	vector<Variable> paramtable;
	bool in_paramtable(string param_name); //check if a parameter is declared
public:
	int i;
	Function(string name);         
	virtual ~Function();

	void add_param(Variable v);      //add a parameter

	int get_param_count();           //get # of parameters
	string get_func_header();        //get func/params info
};

#endif // !defined(AFX_FUNCTION_H__24FD42A3_661C_4F80_8BBD_925CA5391733__INCLUDED_)
