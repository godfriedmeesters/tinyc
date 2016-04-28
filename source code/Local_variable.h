// Local_variable.h: interface for the Local_variable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCAL_VARIABLE_H__DEAF2F48_A9DE_4268_98E6_5F94683621D7__INCLUDED_)
#define AFX_LOCAL_VARIABLE_H__DEAF2F48_A9DE_4268_98E6_5F94683621D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"

class Local_variable : public Variable  
{
private:
	int offset;                         //offset + or - stack frame pointer
public:
	Local_variable(string type,string name,bool arr,int offset);
	virtual ~Local_variable();
	
	int get_offset();
};

#endif // !defined(AFX_LOCAL_VARIABLE_H__DEAF2F48_A9DE_4268_98E6_5F94683621D7__INCLUDED_)
