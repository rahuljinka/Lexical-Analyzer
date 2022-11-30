/*
 * main.cpp
 *
 * CS280 - Spring 2022
 * parser testing program
 */
#include <iostream>
#include <fstream>

#include "lex.h"
#include "parse.h"


using namespace std;



int main(int argc, char *argv[])
{
	int lineNumber = 1;

	istream *in = NULL;
	ifstream file;
		
	for( int i=1; i<argc; i++ )
    {
		string arg = argv[i];
		
		if( in != NULL ) 
        {
			cerr << "ONLY ONE FILE NAME ALLOWED" << endl;
			return 0;
		}
		else 
        {
			file.open(arg.c_str());
			if( file.is_open() == false ) 
            {
				cerr << "CANNOT OPEN " << arg << endl;
				return 0;
			}

			in = &file;
		}
	}
	if(argc == 1)
	{
		cerr << "Missing File Name." << endl;
		return 0;
	}
    //cout << "before entering parser" << endl;
    bool status = Prog(*in, lineNumber);
    //cout << "returned from parser" << endl;
    if( !status )
    {
        cout << "Unsuccessful Parsing" << endl << "Number of Syntax Errors " << ErrCount() << endl;
	}
	else
    {
		cout << "Successful Parsing" << endl;
	}
	return 0;
}
