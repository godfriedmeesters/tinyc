// Parser.cpp: implementation of the Parser class.
//
//////////////////////////////////////////////////////////////////////

#include "Parser.h"

#define debug cout << scanner.token << endl; exit(0);
	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Parser::Parser(char* file):scanner(file)
{
l_count = 0;                                  //0 labels

output_file = file;
output_file.replace(output_file.find(".tc"),3,".asm");      //replace .sc by .asm            
out.open(output_file.data());
}

Parser::~Parser()
{
}

////////////////////////////////DECL/////////////////////////////////////////

void Parser::glob_decls()
{
	while(scanner.pop_token() != "EOF")
		func_decl(scanner.token);				   //do declaration
}

void Parser::func_decl(string name)
{
		int loc_vars_size,params_size;            //size in bytes for ENTER & RET   
		
		symtablemanager.add_function(name);          //update function table

		out << endl << name << " PROC" << endl;           //start of function

		params_size = param_decls(name);           //process parameters
	
		loc_vars_size = loc_top_decls();           //process top declarations
	
		//push bp; bp = sp; sub sp, loc_vars_size
		out << "SUB SP," << loc_vars_size << endl; 
		
		pstatement("");									   //exec body code 

		out << "LEAVE" << endl;                   //sp = bp; pop bp
		out << "RET " << params_size << endl;           //return to caller & del params
		
		out << name << " ENDP" << endl;            //end of funtion
		
		symtablemanager.clear_loc_vars();            //clear loc var table
}

int Parser::param_decls(string func_name)       //process the params of a function
{
	vector<Variable> params;                    //temp vector of params

	scanner.pmatch("(");                   //check opening brace
	
	if(scanner.pop_token() == ")")         //if empty param list
		return 0;                          //return size 0
	else
		scanner.push_token();              //push back token
	
	param_decl(params);                    //process single parameter
	
	while(scanner.pop_token() == ",")           //if another par...
	{
		param_decl(params);              //process parameter
	}
	
	scanner.match(")");							//check closing brace

	int param_offset = 0;             //start from bp + 0


	for(int i = 0;i < params.size();i++)
	{
		param_offset = (params.size() - i - 1) * 2 + 4;  //add 4 to skip bp and ip
		symtablemanager.function(func_name).add_param(params[i]); //for func info
		symtablemanager.add_loc_var(params[i].get_type(),
			params[i].get_name(),params[i].is_arr(),param_offset); //add stack var
	}

	return params.size() * 2;                       //for RET para_size_in_bytes
}

void Parser::param_decl(vector<Variable> &params)
{
	string par_name,par_type;
	par_type = scanner.pop_token();
	par_name = scanner.pop_token();
	
	if(scanner.pop_token() == "[")                   //if arr[]
	{
		scanner.pmatch("]");
		params.push_back(Variable(par_type,par_name,ARR));  //add arr 
	}
	else
	{
		scanner.push_token();                          //push back non - "["
		params.push_back(Variable(par_type,par_name,VAR)); //add var
	}
}

int Parser::loc_top_decls()         //local var declarations in function header
{
	string type, name;                       //var type and name
	int offset = 0;                          //negative offset from BP

	out << "PUSH BP" << endl;                //save BP
	out << "MOV BP,SP" << endl;              //set stack frame pointer
	
	while((type = scanner.pop_token()) == "WORD" || type == "BYTE" )   
	{
		name = scanner.pop_token();          //get var name 
		
		offset -= type_size(type);          //sub offset by type size

		if(scanner.pop_token() == "[")      //opening array brace
			offset -= (loc_arr_decl(type,name,offset) - type_size(type));//define array
		else
		{
			scanner.push_token();                //push back non - "["
			loc_var_decl(type,name,offset);    //define loc var
			
			while(scanner.pop_token() == ",")    //if other variable of same type...
			{
				offset -= type_size(type);          //sub offset by type size
				name = scanner.pop_token();   //get var name
				loc_var_decl(type,name,offset);  //define loc var
			}
		}
		scanner.match(";");							//check ';'
	}
	
	scanner.push_token();                    //push back unrecognized token 
	
	return -offset;                         //return vars size for ENTER instruction
}

int Parser::loc_arr_decl(string type,string name,int first_elem_offset)
{				
	symtablemanager.add_loc_var(type,name,ARR,first_elem_offset);
	
	if(!isdigit(scanner.pop_token().at(0)))
		error_handler.error("array size has to be a number.");
	int arr_size = atoi(scanner.token.data());  //get int size
	scanner.pmatch("]");							//get closing bracket
	
    ////////////////////////////////////////
	if(scanner.pop_token() == "=")
	{
		int elem_offset = first_elem_offset;        //set to first arr elem offset
	
		scanner.pmatch("{");                        //check opening brace
		
		scanner.pop_token();                        //read first elem value

		scanner.check_number();                     //check if token is number
	
		out << "MOV " << type << " PTR [BP-" << -elem_offset << "]," 
			<< scanner.token << endl;
		
		while(scanner.pop_token() == ",")
		{
			scanner.pop_token();                  //read elem value
			scanner.check_number();               //check if token is number
			
			elem_offset -= type_size(type);       //move to next elem

			out << "MOV " << type << " PTR [BP-" << -elem_offset << "]," 
				<< scanner.token << endl;
		}

		scanner.match("}");                       //check closing brace
		
		scanner.pop_token();                      //read ";"    
	}
	
	return arr_size * type_size(type);         //return arr size for ENTER instruction
}

void Parser::loc_var_decl(string type,string name,int offset)
{
	//update local var table
	symtablemanager.add_loc_var(type,name,VAR,offset); 
	
	if(scanner.pop_token() == "=")	      //assignment?
		if(isdigit(scanner.pop_token().at(0)))  //only assign numbers
			out << "MOV " << type << " PTR [BP-" << -offset  << "]," 
			    << scanner.token <<  endl; 
		else
			error_handler.error("only numbers can be assigned."); //output err msg & exit
	else
		scanner.push_token();             //push back non - "="
}


///////////////////////////////////STAT//////////////////////////////////////

void Parser::do_program()
{
	/////////////////////////////////////////////////
	out << ".386" << endl;
	out <<	"stack_s SEGMENT STACK" << endl;
	out <<	"	DB 512 DUP('STACK!!!')" << endl;
	out <<	"stack_s ENDS" << endl << endl;                            
	
	/////////////////////////////////////////////////
	out << "code_s SEGMENT" << endl;
	out << "ASSUME CS:code_s" << endl;  
	out << "EXTRN READ_NUMBER:PROC" << endl;
	out << "EXTRN WRITE_NUMBER:PROC" << endl;
	/////////////////////////////////////////


	glob_decls();
	scanner.pmatch("EOF");                        //program ends with EOF
	
	if(!symtablemanager.in_functable("MAIN"))
		error_handler.error("missing 'MAIN' function"); 
	
	out << "start:" << endl;                      //program entrance point
	out << "CALL main" << endl;                   //call main()
	
	out << "MOV AX,4C00h" << endl;
	out << "INT 21h" << endl;
	out << "code_s ENDS" << endl;
	out << "END start" << endl;

	out.close();

	string command = "borland\\tasm32 " + output_file + " > nul";
	
	system(command.data());

	output_file.replace(output_file.find(".asm"),4,""); //replace "asm" with ""

	
	command = "borland\\tlink " + output_file + " lib\\library > nul";
	system(command.data());

	//delete temp files
	command = "del " + output_file + ".asm";
	system(command.data());

	command = "del " + output_file + ".obj";
	system(command.data());

	command = "del " + output_file + ".map";
	system(command.data());

}

void Parser::statement(string break_label)
{
	if(scanner.token == "{")                   //begining of compound statement
	{
		compound_statement(break_label);                     	
	}
	else
	{
		if(scanner.token == "WHILE")          //beginning of while construct
			do_while();                       
		else

		if(scanner.token == "IF")             //beginning of if construct
			do_if(break_label);
		else
			
		if(scanner.token == "FOR")            //beginning of for construct
			do_for();
		else

		if(scanner.token == "BREAK")          //break from current loop
          	do_break(break_label);
		else

		if(scanner.token == "PRINT")          //write variable to screen
          	do_print();
		else

		if(scanner.token == "READ")          //read variable from keyboard
          	do_read();
		else
		if(scanner.token == "RETURN")        //extract expression into AX
			do_return();
	    else
		if(scanner.token == ";")			//null statement
			;
		else
		if(isalpha(scanner.token.at(0)))    //identifier
			do_func_or_assignment();                     
		else
			error_handler.error("illegal statement " + scanner.token + ".");
	}
}

void Parser::compound_statement(string break_label)
{	
	scanner.pop_token();                      //read next token (skip "{")
	
	//while not eo statement
	while(scanner.token != "}" && scanner.token != "EOF")
	{
		statement(break_label);
		scanner.pop_token();                  //read next token
	}
	
	scanner.match("}");                      //end of compound statement
											 //so END(EOF) not allowed
}

void Parser::pstatement(string break_label)
{
	scanner.pop_token();						//read next token
	statement(break_label);						//execute statement
}
 
void Parser::do_while()
{
    string header;              //while label
	string footer;           //endwhile label
	 
	header = new_label();     //initialize header label 
	print_label(header);     //print header label
	footer = new_label();        //initialize footer label
	scanner.pmatch("(");       //while"("
	b_expression();          //set ZF
	scanner.pmatch(")");        //while(b_expression)
	out << "JE " << footer << endl;   //if cond = false, jmp to end of loop
	
	pstatement(footer);              //execute body
	
	out << "JMP " << header << endl;     //jmp to header
	print_label(footer);             //print footer label

}

void Parser::do_break(string break_label)
{
	if(break_label == "") //there is *no* loop to break from
		error_handler.error("no loop to break from."); //so output err msg & exit program
	else
		out << "JMP " << break_label << endl; //JMP to end of current loop
	scanner.pmatch(";");	
}

void Parser::do_if(string break_label)
{
/*if cond block1 else block2 endif
cmp ax,bx
je label0
block1
jmp label1
label0:
block2
label1:
*/

	string endif_label;                    //else/endif label

	endif_label = new_label();         //create endif label
	scanner.pmatch("(");            //if"("
	b_expression();				 //set ZF
	scanner.pmatch(")");            //if(b_expression")"
	out << "JE " << endif_label << endl;       //JE "endif:"/"else:" - label

	pstatement(break_label);  //execute "if" body

	if(scanner.pop_token() == "ELSE")            //update "endif" label 
	{
		string else_label;         //else block label       
		else_label = endif_label;   //"else" block label = prev endif label
		endif_label = new_label();;  //create endif_label
		out << "JMP " << endif_label << endl;  //skip "else" block
		print_label(else_label);    //print "else" block label
		pstatement(break_label);        //execute "else" body
	}
	else
		scanner.push_token();       //token is not "else", so pass to caller

	print_label(endif_label);         //print endif label - end of "if" construct
}

void Parser::do_for()
{

/*FOR loop_counter = expr TO limit

MOV AX,expr
MOV loop_counter,AX

MOV AX,limit
PUSH AX

label0:
POP AX
CMP loop_counter,AX
JG label1
PUSH AX

block

INC loop_counter
JMP label0

label1:
*/

	string header;        //label at "condition"
	string footer;        //label at end of loop
	string loop_counter;   //variable name

	scanner.pmatch("(");    //for"("

	scanner.pop_token();       //read loop counter

	loop_counter = scanner.token;		  //loop counter = token

	scanner.pmatch("=");

	expression();               //extract init expression into AX
	
	copy_ident_reg(loop_counter,AX_TO_MEM); //copy AX to loop_counter

	scanner.pmatch("TO");
	
	expression();	     		  //extract limit into AX

	scanner.pmatch(")");           //for(loop_counter = start to limit")"

	out << "PUSH AX" << endl;     //put limit on stack

	header = new_label();         //create header label
	////////HEADER////
	print_label(header);          //print header label

	out << "POP BX" << endl;      //POP limit into BX

	copy_ident_reg(loop_counter,MEM_TO_AX);

	out << "CMP AX,BX" << endl; //compare loop counter and AX

	footer = new_label();          //create footer label
	out << "JG " << footer << endl;  //JMP to footer if loop_counter > limit (AX)

	out << "PUSH BX" << endl;     //save limit on stack 

	pstatement(footer);                 //execute body

	copy_ident_reg(loop_counter,MEM_TO_AX); //load loop_counter in AX
	
	out << "INC AX" << endl;              //increment AX
	
	copy_ident_reg(loop_counter,AX_TO_MEM);  //load AX in loop_counter
	
	out << "JMP " << header   << endl;  //JMP to header label - do test

	print_label(footer);		     //print footer label
}

void Parser::print_str(string s)     //print string
{
	for(int i = 0;i < s.size();i++)
	{
		out << "MOV DL,'" << s.at(i) << "'" << endl;
		out << "MOV AH,2" << endl;
		out << "INT 21H" << endl;
	}
}

void Parser::do_read()			        //read to variable
{
	out << "call read_number" << endl;     //read number in AX 
	scanner.pmatch("(");
	copy_ident_reg(scanner.pop_token(),false); //copy AX to ident
	scanner.pmatch(")");
	scanner.pmatch(";");
}

void Parser::do_print()                   //print number or quoted string
{
	scanner.pmatch("(");                    //check opening brace                     

	do
	{
	scanner.pop_token();
	
		if(scanner.token == "\"")              //first qoute of string
		{
			print_str(scanner.get_string());   //get string in quotes	
		}
		else
			if(scanner.token == "ENDL")   // print CR and LF
			{
				out << "MOV DL,0DH" << endl;
				out << "MOV AH,2" << endl;
				out << "INT 21H" << endl;
				
				out << "MOV DL,0AH" << endl;
				out << "MOV AH,2" << endl;
				out << "INT 21H" << endl;
			}
			else                                        
			{
				scanner.push_token();
				expression();                           //extract number
				out << "PUSH AX" << endl;               //put number on stack
				out << "call write_number" << endl;     //read number in AX 
			}
	
	scanner.pop_token();                      //read ","
	
	}while(scanner.token == ",");

	scanner.match(")");                               //check closing brace

	scanner.pmatch(";");                              //end of statement
}

void Parser::do_func_or_assignment()
{
		string identifier = scanner.token;                 //save valid identifier

		if(scanner.pop_token() == "(")                    //opening brace of function                 
			do_func(identifier);                     
		else
		{
			scanner.push_token();                          //push back "="
			do_assignment(identifier);
		}
}

void Parser::do_assignment(string l_var_name)    //assign expression to l_var_name
{
	list<string> l_value_tokens;                //list of l-value tokens

	while(scanner.pop_token() != "=")	   //store all l-value tokens (but skip name)
		l_value_tokens.push_back(scanner.token);
		
	expression();  						//extract expression after "=" into AX/L

	scanner.pmatch(";");						//assignment is ended by ";"	

	while(!l_value_tokens.empty())       //push back l-value tokens to scanner
	{
		scanner.push_token(l_value_tokens.front());   
		l_value_tokens.pop_front();      //remove token
	}
	
	//now, copy ident consisting of pushed tokens to AX/L
	copy_ident_reg(l_var_name,AX_TO_MEM);           
}

void Parser::do_func(string func_name)
{
	if(!symtablemanager.in_functable(func_name))
		error_handler.error("function " + func_name + " is not declared.");
	
	//push params
	do_params(func_name);

	//tranfer control
	out << "CALL " << func_name << endl;           //JMP to proc label
}

void Parser::do_params(string func_name)       //push all params on the stack
{
	int params_required = symtablemanager.function(func_name).get_param_count();
	int params_processed = 0;

	if(scanner.pop_token() != ")")             //if param list is not empty                 
	{
		scanner.push_token();                 //hand param name over to expression
		expression();                         //extract expression into AX
		out << "PUSH AX" << endl;             //push AX on stack
		params_processed++;                   
	
		while((scanner.pop_token()) == ",")   //if another param
		{
			expression();                     //extract expression into AX
			out << "PUSH AX" << endl;         //push AX on stack
			params_processed++;
		}

	}
	
	scanner.match(")");                       //check closing brace

	if(params_processed != params_required)
		error_handler.error("wrong number of arguments for " + 
			symtablemanager.function(func_name).get_func_header());
}

void Parser::do_return()
{
	expression();         //get expression into AX
	scanner.pmatch(";");  //check ";"
}

////////////////////////////////////MISC//////////////////////////////////////////

string Parser::type_to_reg(string type,bool xor)
{
	if(type == "BYTE")               //if byte
	{
		if(xor)
			out << "XOR AH,AH" << endl;   //clear AX upper 8 bits
		return "AL";                   //return byte register
	}
	else
		return "AX";                //return word register
}

string Parser::new_label()
{
	string label;                       //holds label to be created
	label = "Label";                    //first label part = 'Label'
	char number[5];                    //ascii l_count
	itoa(l_count++,number,10);         //convert l_count b10 to ascii
	label += number;                  //append index
	return label;                     //return label to caller
}

void Parser::print_label(string label)
{
	out << label  << ":" << endl;      //simply print label + colon
}

int Parser::type_size(string type)
{
	return type == "WORD" ? 2 : 1;
}

/////////////////////////////////EXPRESSION PARSING////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Parser::relation()
{

	expression();                            //read the first expression in AX

	scanner.pop_token();

	while( scanner.token == ">" || scanner.token == "=" || scanner.token == "<" 
	  || scanner.token == ">=" || scanner.token == "<=" || scanner.token == "!=") 
	{
		out << "PUSH AX" << endl;//next expression is coming, so save current expression
	
		if(scanner.token == ">")
		{
			expression();	    //read next expression in AX
			out << "POP BX" << endl;     //pop previous expression
			out << "CMP BX,AX" << endl;  //set FLAGS
			out << "SETA AL" << endl;    //if left > right then AX = 1
		}
		else
		if(scanner.token == "=")
		{
			expression();       //read next expression in AX
			out << "POP BX" << endl;     //pop previous expression	
			out << "CMP BX,AX" << endl;  //set FLAGS
			out << "SETE AL" << endl;    //if left = right then AX = 1 
		}
		else
		if(scanner.token == "!=")
		{
			expression();       //read next expression in AX
			out << "POP BX" << endl;     //pop previous expression	
			out << "CMP BX,AX" << endl;  //set FLAGS
			out << "SETNE AL" << endl;    //if left != right then AX = 1 
		}
		else
		if(scanner.token == "<")
		{
			expression();       //read next expression in AX
			out << "POP BX" << endl;     //pop previous expression	
			out << "CMP BX,AX" << endl;  //set FLAGS
			out << "SETB AL" << endl;    //if left < right then AX = 1
		}
		else
		if(scanner.token == ">=")
		{
			expression();	    //read next expression in AX
			out << "POP BX" << endl;     //pop previous expression
			out << "CMP BX,AX" << endl;  //set FLAGS
			out << "SETAE AL" << endl;    //if left > right then AX = 1
		}
		else
		if(scanner.token == "<=")
		{
			expression();	    //read next expression in AX
			out << "POP BX" << endl;     //pop previous expression
			out << "CMP BX,AX" << endl;  //set FLAGS
			out << "SETBE AL" << endl;    //if left > right then AX = 1
		}
	

		out << "CBW" << endl;            //convert byte to word
	

		scanner.pop_token();                //read next token
}

scanner.push_token();             //pass unrecognized token to caller 	
}

void Parser::b_expression()
{
	relation();                            //read the first expression in AX

	scanner.pop_token();                    //read next token

	while(scanner.token == "|" || scanner.token  == "&")//while tok = AND or tok = OR 
	{
		out << "PUSH AX" << endl;    //next expression is coming, so save current expression
	
	  
		if(scanner.token == "|")
		{
			relation();	         //read next relation in AX
			out << "POP BX" << endl;      //pop previous expression
			out << "OR AX,BX" << endl;    //OR the 2 expressions
		}
		else
		{
			relation();	        //read next relation in AX
			out << "POP BX" << endl;     //pop previous expression
			out << "AND AX,BX" << endl;  //AND the 2 expressions
		}
	

		scanner.pop_token();                //read next token
	}

	out << "TEST AX,AX" << endl;     //set FLAGS

	scanner.push_token();					//pass unrecognized token to caller

}



void Parser::expression()
{
	term();                             //read the first term in AX

	scanner.pop_token();		            //read next token

	while( scanner.token == "+" || scanner.token == "-")
	{
		out << "PUSH AX" << endl;    //next term is coming, so save current term
	
		if(scanner.token == "+")
		{
			term();	                     //read next term in AX
			out << "POP BX" << endl;     //pop previous term
			out << "ADD AX,BX" << endl;  //add the 2 terms 
		}
		else
		{
			term();                      //read next term in AX
			out << "POP BX" << endl;     //pop previous term
			out << "XCHG AX,BX" << endl; //exchange 2nd and 1st term	
			out << "SUB AX,BX" << endl;  //subtract 2nd from 1st term
		}

		scanner.pop_token();                //read next token
	}

	scanner.push_token();                   //pass token to caller 
}


void Parser::term()
{
	factor();                           //read first term in AX

	scanner.pop_token();                            //read next token


	while( scanner.token == "*" || scanner.token  == "/")     
	{
		out << "PUSH AX" << endl;    //next factor is coming, so save current factor
	
		if(scanner.token == "*")
		{	
			factor();           //read next factor in AX
			out << "POP BX" << endl;     //pop previous factor
			out << "MUL BX" << endl;     //multiply the 2 factors
		}	
		else
		{
			factor();           //read next factor in AX
			out << "POP BX" << endl;     //pop previous factor
			out << "XCHG AX,BX" << endl; //exchange 2nd and 1st factor
			out << "XOR DX,DX" << endl;  //DX = 0
			out << "DIV BX" << endl;     //divide 1st by 2nd term 
		}

		scanner.pop_token();                //check for '*' || '/'
	}

	scanner.push_token();                           //pass c to caller "expression"
}


void Parser::factor()
{
	scanner.pop_token();		                //read next token

	bool is_not_factor = false;     //factor is not preceded by a '!' sign
	
	if(scanner.token == "!")     //if factor preceded by not operator
	{
		is_not_factor = true;       //...so NOT AX *after* factor is extracted
		scanner.pop_token();                //read next token
	}

	if(scanner.token == "(")                        
	{
		b_expression();               //<factor> ::= (<b-expression>)
		scanner.pmatch(")");
	}
	else
		if(scanner.token == "TRUE" || scanner.token == "FALSE")          //c = b-literal
			if(scanner.token == "TRUE")                   //if TRUE
				out << "MOV AX,01h" << endl;        //set AX to 1
			else						   //if FALSE
				out << "MOV AX,00h" << endl;		   //set AX to 0
	else
		if (isalpha(scanner.token.at(0)))   //if first char in token is alpha
			identifier(scanner.token);                  //then token is identifier
	else
		if (isdigit(scanner.token.at(0)))   //if first char in token is alpha
			out << "MOV AX," << scanner.token << endl;   //then token is number
		else
			error_handler.expected("expression",scanner.token);

	if(is_not_factor)       //factor was preceded by '!' sign
	{
		out << "NOT AX" << endl;     //NOT factor
		out << "AND AX,01h" << endl; //make sure only first bit is set
	}
}

void Parser::identifier(string ident)
{
	copy_ident_reg(ident,MEM_TO_AX);	     //copy var/arr/func to AX/L
}

void Parser::copy_ident_reg(string ident,bool mem_to_AX)
{
	string type,sign,mem;

	scanner.pop_token();							//read next token
	
	if(scanner.token == "(")                        //it's a function()...
	{
		if(!mem_to_AX)                 //a function cannot be used as l-value 
			error_handler.error("invalid l-value " + ident + ".");
		else
			do_func(ident);            //extract function result in AX        
	}
	else                                            //it's a stack variable
	{
		 scanner.push_token();                       //push back non - "("

		 if(symtablemanager.in_loc_vartable(ident)) //check for declaration
		 {
			type = symtablemanager.loc_var(ident).get_type(); //get var type
			
			if(scanner.pop_token() == "[")                 //array[expression]
			{
				if(!mem_to_AX)
					out << "MOV CX,AX" << endl;           //save r-value in DX         
				
				expression(); //extract index into AX
				

				//multiply index by type size
				out << "MOV BX," << symtablemanager.loc_var(ident).get_type_size() 
					<< endl;
				out << "MUL BX" << endl; 
				out << "MOV SI,AX" << endl; //copy multiplied index to SI
				out << "NEG SI" << endl;    //negate SI for pos indexing

				if(!mem_to_AX)
					out << "MOV AX,CX" << endl;          //copy r-value to AX         
				
				
				if(symtablemanager.loc_var(ident).get_offset() > 0) //if param arr
				{
					out << "PUSH BP" << endl;             //save BP

					//extract address of param into BP
					out << "MOV BP,[BP+"<< symtablemanager.loc_var(ident).get_offset()
						<< "]" << endl;


					if(mem_to_AX)
						out << "MOV " << type_to_reg(type) << ",[BP+SI]" << endl;
					else
						out << "MOV [BP+SI]," << type_to_reg(type,NO_XOR) << endl;
				
					out << "POP BP" << endl;  //restore stack frame pointer
				}
				else                                            //if loc arr
				{
					if(mem_to_AX)
						out << "MOV " << type_to_reg(type) << ",[BP+SI-" 
						    << -symtablemanager.loc_var(ident).get_offset()
							<< "]" << endl;						
					else
						out << "MOV [BP+SI-"  
							<< -symtablemanager.loc_var(ident).get_offset()
							<< "]," << type_to_reg(type,NO_XOR) << endl;

				}

				scanner.pmatch("]");                   //check closing arr brace
			}
			else
			{
				scanner.push_token();                  //push non - "["

				if(symtablemanager.loc_var(ident).is_arr())
				{
					if(mem_to_AX)  //copy address of array to AX
					{
						if(symtablemanager.loc_var(ident).get_offset() < 0) //loc var
						{
							out << "MOV AX,BP" << endl;//copy curr stack frame pointer
							out << "SUB AX," 
								<< abs(symtablemanager.loc_var(ident).get_offset())
							    <<  endl; //add var offset
						}
						else     //param
							out << "MOV AX,[BP+" 
							    << symtablemanager.loc_var(ident).get_offset()
							    << "]";
					}
					else
						error_handler.error(ident + " cannot be used as an l-value.");
				}
				else   //var
				{
					//"+" = param; "-" = local var 
					sign = symtablemanager.loc_var(ident).get_offset() > 0 ? "+" : "-";

					//copy AX/L <-> mem
					
					if(mem_to_AX)
						out << "MOV " << type_to_reg(type) << ",[BP" << sign 
						    << abs(symtablemanager.loc_var(ident).get_offset())
							<< "]" << endl;
					else
						out << "MOV [BP" << sign  
							<< abs(symtablemanager.loc_var(ident).get_offset())
							<< "]," << type_to_reg(type,NO_XOR) << endl;
				}
			}
		 }
		 else
			 error_handler.error("undeclared identifier " + ident + ".");
	}
}

