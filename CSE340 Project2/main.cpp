/*
* Copyright (C) Mohsen Zohrevandi, 2017
*
* Do not share this file with anyone
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"

using namespace std;

int main(int argc, char* argv[])
{
	int task;
	LexicalAnalyzer lexer;

	if (argc < 2)
	{
		cout << "Error: missing argument\n";
		return 1;
	}

	/*
	Note that by convention argv[0] is the name of your executable,
	and the first argument to your program is stored in argv[1]
	*/

	task = atoi(argv[1]);
        lexer.Parse();

	/*
	Hint: You can modify and use the lexer from previous project
	to read the input. Note that there are only 4 token types needed
	for reading the input in this project.

	WARNING: You will need to modify lexer.cc and lexer.h to only
	support the tokens needed for this project if you are going to
	use the lexer.
	*/

	switch (task) {
	case 1:
            lexer.PrintTaskOne();
		break;

	case 2:
            lexer.ParseGenerating();
            lexer.PrintReachable();
		break;

	case 3:
	    lexer.CreateFirstSets();
            lexer.PrintFirstSets();
		break;

	case 4:
		// TODO: perform task 4.
		break;

	case 5:
		// TODO: perform task 5.
		break;

	default:
		cout << "Error: unrecognized task number " << task << "\n";
		break;
	}
	return 0;
}
