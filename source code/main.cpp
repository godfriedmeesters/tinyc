#include "Parser.h"
#include "Error.h"

Error_handler error_handler;                //shared error handler object

void main(int argc, char *argv[])
{
	
	if(argc == 1)                           //no input file
	{
		cout << "TCC Compiler Version 0.2 Copyright (c) 2004 Godfried Meesters" << endl;
		cout <<  "Usage: TCC filename.tc" << endl << endl;
	}
	else
	{
		string s = argv[1];
		if(s.find(".tc") == -1)
		{
			error_handler.error("file type is not *.tc");
		}
		else
		{
		Parser parser(argv[1]);            
	 	parser.do_program();               //start compilation
		cout << "Compilation successfull." << endl;
		}
	}

}