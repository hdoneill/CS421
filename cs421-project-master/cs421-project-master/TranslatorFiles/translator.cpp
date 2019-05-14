#include "translator.h"
using namespace std;

//=================================================
// File translator.cpp written by Group Number: 12
//=================================================

// ----- Utility and Globals -----------------------------------

extern string tokenName[];
extern map<string, tokentype> reserved_words;
extern ifstream fin;

tokentype saved_token;
bool token_available;
string saved_lexeme;
string saved_e_word;

// dictionary that will hold the content of lexicon.txt
map<string, string> lexicon;
ofstream trans;

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

// Grammar: <s> ::= [CONNECTOR #getEword# #gen("CONNECTOR")#] <noun> #getEword# SUBJECT #gen("ACTOR")# <after subject>
// Done by: Hugh O'Neill
void s()
{
	cout << "Processing <s>" << endl;

	if (next_token() == CONNECTOR) {
		match(CONNECTOR);
		getEword();
		gen("CONNECTOR");
	}

	noun();

	match(SUBJECT);
	gen("ACTOR");

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

	getEword();
}

// Grammar: <after subject> ::= <verb> #getEword# #gen("ACTION")# <tense> #gen("TENSE")# PERIOD |
//              <noun> #getEword# <after noun>
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
	getEword();
	gen("ACTION");
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

	gen("TENSE");
}

// Grammar: <after noun> ::= <be> #gen("DESCRIPTION")# #gen("TENSE")# PERIOD |
//              DESTINATION #gen("TO")# <verb> #getEword# #gen("ACTION")# <tense> #gen("TENSE")# PERIOD |
//              OBJECT #gen("OBJECT")# <after object>
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
		gen("TO");
		verb();
		tense();
		match(PERIOD);
		break;
	case OBJECT:
		match(OBJECT);
		gen("OBJECT");
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

	gen("DESCRIPTION");
	gen("TENSE");
}

// Grammar: <after object> ::= <verb> #getEword# #gen("ACTION")# <tense> #gen("TENSE")# PERIOD |
//          <noun> #getEword# DESTINATION #gen("TO")# <verb> #getEword# #gen("ACTION")# <tense> #gen("TENSE")# PERIOD
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
		gen("TO");
		verb();
		tense();
		match(PERIOD);
		break;
	default:
		syntax_error2("after_object");
	}
}

//---------------------------------------

// using the current lexeme, look up the English word in the Lexicon if it is there -- save the result in saved_E_word
// otherwise, save the Japanese word as-is
// Done by: Ryan Santos
void getEword()
{
	map<string, string>::iterator e_word = lexicon.find(saved_lexeme);
	if (e_word != lexicon.end()) {  // lexicon[saved_lexeme] exists
		saved_e_word = e_word->second;
	}
	else {
		saved_e_word = saved_lexeme;
	}
}

// using the line type, sends a line of an IR to translated.txt (saved_E_word or saved_token is used)
// Done by: Hugh O'Neill
void gen(string line_type)
{
	string saved;

	if (line_type == "TENSE") {
		saved = tokenName[saved_token] + '\n';
	}
	else {
		saved = saved_e_word;
	}

	trans << line_type << ": " << saved << endl;
}

// -------------------------------------------

// The final test driver to start the translator
// Done by: Qian Zhu
int main()
{
	// open the lexicon.txt file and read it in
	ifstream lex("lexicon.txt", ios::in);
	string line;

	while (getline(lex, line)) {
		istringstream tokens(line);
		string jap, eng;

		tokens >> jap;
		tokens >> eng;

		lexicon[jap] = eng;
	}

	lex.close();

	trans.open("translated.txt", ios::out);

	string filename;

	cout << "Enter the input file name: ";
	cin >> filename;
	fin.open(filename.c_str());

	story();  // start parsing

	fin.close();
	trans.close();
}// end
