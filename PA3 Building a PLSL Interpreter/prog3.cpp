/* Programming Assignment 3
 * 
 * main.cpp
 *
 * CS280 - Spring 2022
 * parser and interpreter testing program
 */

#include <iostream>
#include <fstream>

#include "lex.h"
#include "val.h"
#include "parseInt.h"


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
    
    bool status = Prog(*in, lineNumber);
    
    if( !status ){
    	cout << "\nUnsuccessful Interpretation" << endl << "Number of Errors " << ErrCount()  << endl;
	}
	else{
        cout << endl;
		cout << "Successful Execution" << endl;
	}
    
}
