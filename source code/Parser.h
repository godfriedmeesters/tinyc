// Parser.h: interface for the Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__BE1613A1_B391_4F2D_83B4_6ABC11B7FD0E__INCLUDED_)
#define AFX_PARSER_H__BE1613A1_B391_4F2D_83B4_6ABC11B7FD0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scanner.h"
#include "Error.h"
#include "Symboltable.h"
#include "Local_variable.h"

#define ARR   true
#define VAR   false
#define MEM_TO_AX true
#define AX_TO_MEM false
#define NO_XOR false

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

extern Error_handler error_handler;               //shared error handler object

class Parser  
{
public:
	Parser(char* file);
	virtual ~Parser();
	void do_program();                           //do compilation

private:
	int type_size(string type);
	Scanner scanner;
	Symtablemanager symtablemanager;

	ofstream out;                               //main output stream
	string output_file;                         //main output file

	int loc_top_decls();                        //decls in function header
	void loc_var_decl(string type,string name,int offset);
	int loc_arr_decl(string type,string name,int offset);
	
	void glob_decls();                          //func decls                          

	void func_decl(string name);                
	int param_decls(string func_name);          
	void param_decl(vector<Variable> &params);  
	
	void compound_statement(string break_label);
	void pstatement(string break_label);        //pop token and do statement
	void statement(string break_label);          
	
	int l_count;                               //number of labels generated
	void print_label(string label);             
	string new_label();                       

	void do_for();
	void do_if(string break_label);
	void do_while();
	void do_break(string break_label);
	
	void do_func_or_assignment();
	void do_func(string func_name);
	void do_params(string func_name);
	void do_return();                             //get expression into AX
	void do_assignment(string l_var_name);

	void do_read();                               //read variable
	void do_print();						      //print variable
	void print_str(string s);                     //print quoted string 
	
	
	void relation();                           
	void factor();
	void term();
	void b_expression();
	void expression();
	
	void identifier(string ident);               //parse arr/var/func

	string type_to_reg(string type,bool xor = true);            //convert type to AX/AH
	void copy_ident_reg(string ident,bool mem_to_AX); //copy arr/var/func to AX/L
};

#endif // !defined(AFX_PARSER_H__BE1613A1_B391_4F2D_83B4_6ABC11B7FD0E__INCLUDED_)
