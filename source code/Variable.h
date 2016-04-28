// Variable.h: interface for the Variable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLE_H__8861A3A7_93A1_45C1_B477_E23A070593F3__INCLUDED_)
#define AFX_VARIABLE_H__8861A3A7_93A1_45C1_B477_E23A070593F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Symbol.h"

class Variable : public Symbol  
{
private:
	string type;                                 //BYTE or WORD
	bool arr;                                    //arr or var
public:
	Variable(string type,string name,bool arr);  
	virtual ~Variable();
	
	int get_type_size();                         //get type size in bytes
	string get_type();                           //get type string
	bool is_arr();                               //determine if array or var
};

#endif // !defined(AFX_VARIABLE_H__8861A3A7_93A1_45C1_B477_E23A070593F3__INCLUDED_)
