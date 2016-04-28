// Symbol.h: interface for the Symbol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOL_H__E0A7EEE9_288A_41DD_9301_67714359662A__INCLUDED_)
#define AFX_SYMBOL_H__E0A7EEE9_288A_41DD_9301_67714359662A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string> 
using namespace std;

#include "Error.h"

extern Error_handler error_handler;               //shared error handler object

class Symbol  
{
protected:
	string name;
public:
	Symbol(string name);
	virtual ~Symbol();

	string get_name();
	bool operator==(string name);
};

#endif // !defined(AFX_SYMBOL_H__E0A7EEE9_288A_41DD_9301_67714359662A__INCLUDED_)
