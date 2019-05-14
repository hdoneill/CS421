#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "scanner.h"
using namespace std;

// INSTRUCTION:  Complete all ** parts.
// You may use any method to connect this file to scanner.cpp
// that you had written.
// You can copy scanner.cpp here by cp ../ScannerFiles/scanner.cpp .
// -----------------------------------------------------

//=================================================
// File parser.cpp written by Group Number: 12
//=================================================

// ----- Utility and Globals -----------------------------------

extern string tokenName[];
extern map<string, tokentype> reserved_words;

tokentype saved_token;
bool token_available;
string saved_lexeme;

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
// ** Be sure to put the name of the programmer above each function
// i.e. Done by:

// Done by: Ryan Santos
void syntax_error1(tokentype expected) {
	cout << endl << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << saved_lexeme << endl;
	exit(1);
}

// Done by: Ryan Santos
void syntax_error2(string function_name) {
	cout << endl << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << function_name << endl;
	exit(1);
}

// ** Need the updated match and next_token (with 2 global vars)
// ** Be sure to put the name of the programmer above each function
// i.e. Done by:

// Done by: Hugh O'Neill
tokentype next_token()
{
	if (!token_available) {
		scanner(saved_token, saved_lexeme);
		token_available = true;
	}

	return saved_token;
}

// Done by: Hugh O'Neill
bool match(tokentype expected)
{
	if (next_token() != expected) {
		syntax_error1(expected);
	}
	else {
		token_available = false;
		cout << "Matched " << tokenName[expected] << endl;
		return true;
	}
}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// i.e. Grammar:
// ** Be sure to put the name of the programmer above each function
// i.e. Done by:

// Grammar: <story> ::= <s> { <s> }
// Done by: Ryan Santos
void story()
{
	cout << "Processing <story>" << endl << endl;

	s();

	bool done = false;
	while (!done) {
		switch (next_token()) {
		case CONNECTOR:
		case WORD1:
		case PRONOUN:
			s();
			break;
		default: done = true;
		}
	}

	cout << endl << "Successfully parsed <story>." << endl;
}

// Grammar: <s> ::= [CONNECTOR] <noun> SUBJECT <after subject>
// Done by: Hugh O'Neill
void s()
{
	cout << "Processing <s>" << endl;

	if (next_token() == CONNECTOR) {
		match(CONNECTOR);
	}

	noun();

	match(SUBJECT);

	after_subject();
}

// Grammar: <noun> ::= WORD1 | PRONOUN
// Done by: Qian Zhu
void noun()
{
	cout << "Processing <noun>" << endl;

	switch (next_token()) {
	case WORD1:
		match(WORD1);
		break;
	case PRONOUN:
		match(PRONOUN);
		break;
	default:
		syntax_error2("noun");
	}
}

// Grammar: <after subject> ::= <verb> <tense> PEROD | <noun> <after noun>
// Done by: Ryan Santos
void after_subject()
{
	cout << "Processing <after_subject>" << endl;

	switch (next_token()) {
	case WORD2:
		verb();
		tense();
		match(PERIOD);
		break;
	case WORD1:
	case PRONOUN:
		noun();
		after_noun();
		break;
	default:
		syntax_error2("after_subject");
	}
}

// Grammar: <verb> ::= WORD2
// Done by: Hugh O'Neill
void verb()
{
	cout << "Processing <verb>" << endl;

	match(WORD2);
}

// Grammar: <tense> := VERBPAST | VERBPASTNEG | VERB | VERBNEG
// Done by: Qian Zhu
void tense()
{
	cout << "Processing <tense>" << endl;

	switch (next_token()) {
	case VERBPAST:
		match(VERBPAST);
		break;
	case VERBPASTNEG:
		match(VERBPASTNEG);
		break;
	case VERB:
		match(VERB);
		break;
	case VERBNEG:
		match(VERBNEG);
		break;
	default:
		syntax_error2("tense");
	}
}

// Grammar: <after noun> ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD | OBJECT <after object>
// Done by: Ryan Santos
void after_noun()
{
	cout << "Processing <after_noun>" << endl;

	switch (next_token()) {
	case IS:
	case WAS:
		be();
		match(PERIOD);
		break;
	case DESTINATION:
		match(DESTINATION);
		verb();
		tense();
		match(PERIOD);
		break;
	case OBJECT:
		match(OBJECT);
		after_object();
		break;
	default:
		syntax_error2("after_noun");
	}
}

// Grammar: <be> ::= IS | WAS
// Done by: Hugh O'Neill
void be()
{
	cout << "Processing <be>" << endl;

	switch (next_token()) {
	case IS:
		match(IS);
		break;
	case WAS:
		match(WAS);
		break;
	default:
		syntax_error2("be");
	}
}

// Grammar: <after object> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
// Done by: Qian Zhu
void after_object()
{
	cout << "Processing <after_object>" << endl;

	switch (next_token()) {
	case WORD2:
		verb();
		tense();
		match(PERIOD);
		break;
	case WORD1:
	case PRONOUN:
		noun();
		match(DESTINATION);
		verb();
		tense();
		match(PERIOD);
		break;
	default:
		syntax_error2("after_object");
	}
}

//---------------------------------------

extern ifstream fin;

// The new test driver to start the parser
// Done by: Qian Zhu
int main()
{
	string filename;

	cout << "Enter the input file name: ";
	cin >> filename;
	fin.open(filename.c_str());


	story();  //** calls the <story> to start parsing
	fin.close();  //** closes the input file
}// end
//** require no other input files!
//** syntax error EC requires producing errors.text of messages
