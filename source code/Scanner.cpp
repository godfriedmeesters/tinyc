// Scanner.cpp: implementation of the Scanner class.
//
//////////////////////////////////////////////////////////////////////

#include "Scanner.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



Scanner::Scanner(char* input_source)
{
	input = fopen(input_source,"r");        //open main input file for reading
	if(input == NULL)
		error_handler.error("couldn't open input file.");
}

Scanner::~Scanner()
{
	fclose(input);
}

void Scanner::push_char()
{
	ungetc(_char,input);          //push _char back onto the stream

	if(_char == '\n')
		error_handler.dec_line_counter();  //to previous line
}

void Scanner::pop_char()
{
	_char = getc(input);		//pop next _char from the stream	
	if(_char == '\n')
		error_handler.inc_line_counter(); //to next line
	_char = toupper(_char);      //convert _char to upper case
}




void Scanner::scan()           //scan next token
{
	do								 //skip white space and newlines 
	pop_char();						 
	while(_char == ' ' || _char == '\t' || _char == '\n');
		
	//type of token is determined by first letter of token...
	if(isalpha(_char))					
	{						
		token = get_name();	         //token is identifier
	}
	else
	if(isdigit(_char))
	{
		token = get_number();	     //token is number
	}
	else
	if(is_operator(_char))
	{
		token =  get_operator();	 //token is operator
	}
	else
	if(_char == EOF)
	{
		token = "EOF";                
	}
	else
	{				//it's neither an identfier/keyword, nor a number, nor an operator
		token = _char;         //so simply return _char
	}

	
	
}

string Scanner::get_name()      
{
	string name;                    
	
	do
	{
		name += _char;		           //add _char to 'name' string
		pop_char();					//read next _char
	
	}while(isalnum(_char));				//while _char is alphanumeric

	push_char();						//pass unrecognized _char to caller
	
	return name;					//return complete name 

}

string Scanner::get_number()
{
	string number;                  
	
	do
	{
		number += _char;         	 //add _char to 'number' string
		pop_char();					   //read next _char
	
	}while(isdigit(_char));				//while _char is numeric
	
	push_char();						//pass unrecognized _char to caller

	return number;					//return complete number
}

string Scanner::get_operator()
{
	string _operator;                
	
	do
	{
		_operator += _char;         	//add _char to '_operator' string
		pop_char();					//read next _char
	
	}while(is_operator(_char));

	push_char();						//pass unrecognized _char to caller

	return _operator;					//return complete operator
}


bool Scanner::is_operator(int _char)   //check if '_char' is (part of) operator
{
   return (_char == '+' || _char == '-' || _char == '*' || _char == '/' 
	   || _char == '=' || _char == '>' || _char == '<' || _char == '!' 
	   || _char == '&' || _char == ':' || _char == '|');
}

string Scanner::pop_token()        //extract next token
{
	if(!pushed_tokens.empty())         //if list of pushed tokens not empty 
	{
		token = pushed_tokens.front();  //copy the first pushed token
		pushed_tokens.pop_front();      //remove the first pushed token	
	}
	else
		scan();                //read next token from input stream

	return token;     
}

void Scanner::push_token(string ptoken)     //push back token
{	
	if(ptoken == "")
		pushed_tokens.push_front(this->token);//push curr token on "pushed_tokens" list
	else
	{
		pushed_tokens.push_back(ptoken);     //push ptoken on "pushed_tokens" list
	}
}

void Scanner::match(string m)
{
if(token != m)
	error_handler.expected(m,token);    //output Error_handler msg & exit program
}

void Scanner::pmatch(string m)
{
	pop_token();         //read next token
	match(m);			//check if m = token 
}

string Scanner::get_string()  //get "quoted" string
{
	string s;
	
	_char = getc(input);         
	
	while(_char != '\"' && _char != '\n')  
	{
		s = s + (char)_char;   //append char
		_char = getc(input);   
	}
	
	if(_char == '\n')            
		error_handler.error("a string cannot span more than one line."); 
							//string cannot extend over n lines
	return s;
}

int Scanner::check_number()          //check if token is number
{
	if(!isdigit(token.at(0)))
		error_handler.expected("number",token);
	else
		return atoi(token.data());   //return token converted to int

	return 0;                  //just to suppress compiler warning
}

