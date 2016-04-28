// Error.h: interface for the Error_handler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Error_H__B292E2F8_F3D7_43CD_BE8F_5517D5B8DEB4__INCLUDED_)
#define AFX_Error_H__B292E2F8_F3D7_43CD_BE8F_5517D5B8DEB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <string>
#include <iostream>
using namespace std;

class Error_handler  
{
private:
	int line_counter;
public:
	Error_handler();
	virtual ~Error_handler();
	
	void inc_line_counter();                            //to next line
	void dec_line_counter();                            //to previous line
		
	void error(string err_msg);                         //err msg & exit
	void expected(string expect,string instead_of);     //err msg & exit

};

#endif // !defined(AFX_Error_H__B292E2F8_F3D7_43CD_BE8F_5517D5B8DEB4__INCLUDED_)
