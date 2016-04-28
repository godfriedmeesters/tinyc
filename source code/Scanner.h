// Scanner.h: interface for the Scanner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANNER_H__E502E281_F104_4D61_BA4F_4D2824A6BF4D__INCLUDED_)
#define AFX_SCANNER_H__E502E281_F104_4D61_BA4F_4D2824A6BF4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <string>
#include <iostream>
#include <list>
using namespace std;

#include "Error.h"

extern Error_handler error_handler;               //shared error handler object

class Scanner  
{
private:
	void scan();                               //extract next token
	list<string> pushed_tokens;               //contains last pushed token
	string get_operator();                     //extract operator token
	string get_number();                       //extract number token
	string get_name();						   //extract alpa token
	int _char;                                 //holds curr input stream char 
	FILE* input;                               //main input file
	void pop_char();						   //read next input stream char
	void push_char();                          //push back input stream char
	bool is_operator(int c);                   //check if 'c' is (part of) operator

public:
	Scanner(char* input_source);
	~Scanner();
	
	string token;                              //curr input stream token
	
	string pop_token();                        //extract next input stream token
	void push_token(string ptoken = "");         //push back curr token

	string get_string();                       //return a "quoted" string
	                     
	void match(string m);                      //match 'm' to token 
	void pmatch(string m);                     //pop next token & match 'm'

	int check_number();                        //check if curr token is number
	
};

#endif // !defined(AFX_SCANNER_H__E502E281_F104_4D61_BA4F_4D2824A6BF4D__INCLUDED_)
