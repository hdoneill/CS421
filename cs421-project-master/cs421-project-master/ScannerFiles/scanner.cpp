#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

//=====================================================
// File scanner.cpp written by: Group Number: 12
//=====================================================

// --------- DFAs ---------------------------------

// WORD DFA
// Done by: Qian Zhu
// RE: (vowel | vowel n | consonant vowel | consonant vowel n | consonant-pair vowel | consonant-pair vowel n)^+
bool word(string s)
{
	/*
	q0 = 0, qc = 1, qcon = 2, qcon1 = 3,
	qs = 4, qt = 5, qf = 6, qf1 = 7
	*/
    
	int state = 0;
	
	for (char c : s) {
		c = tolower(c);

		if ((state == 0 || state == 6 || state == 7) &&
			(c == 'd' || c == 'j' || c == 'w' || c == 'y' || c == 'z')) {
			state = 2;
		}
		else if ((state == 1 || state == 4) && (c == 'h')) {
			state = 2;
		}
		else if ((state == 5) && (c == 's')) {
			state = 2;
		}
		else if ((state == 3) && (c == 'y')) {
			state = 2;
		}
		else if ((state == 0 || state == 6 || state == 7) && (c == 'c')) {
			state = 1;
		}
		else if ((state == 7) && (c == 'n')) {
			state = 6;
		}
		else if ((state == 0 || state == 6) &&
			(c == 'b' || c == 'g' || c == 'h' || c == 'k' || c == 'm' || c == 'n' || c == 'p' || c == 'r')) {
			state = 3;
		}
		else if ((state == 7) &&
			(c == 'b' || c == 'g' || c == 'h' || c == 'k' || c == 'm' || c == 'p' || c == 'r')) {
			state = 3;
		}
		else if ((state == 0 || state == 6 || state == 7) && (c == 't')) {
			state = 5;
		}
		else if ((state == 0 || state == 6 || state == 7) && (c == 's')) {
			state = 4;
		}
		else if ((state == 0 || state == 2 || state == 3 || state == 4 || state == 5 || state == 6 || state == 7) &&
			(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')) {
			state = 7;
		}
		else {
			return false;
		}
	}

	return state == 6 || state == 7;
}

// PERIOD DFA
// Done by: Qian Zhu
bool period(string s)
{
	int state = 0;

	for (char c : s) {
		if (state == 0 && c == '.') {
			state = 1;
		}
		else {
			return false;
		}
	}

	return state == 1;
}

// -----  Tables -------------------------------------

enum tokentype { ERROR, WORD1, WORD2, PERIOD, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR };

string tokenName[30] = { "ERROR", "WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR" };  // for the display names of tokens

map<string, tokentype> reserved_words = {
	{"masu", VERB},
	{"masen", VERBNEG},
	{"mashita", VERBPAST},
	{"masendeshita", VERBPASTNEG},
	{"desu", IS},
	{"deshita", WAS},
	{"o", OBJECT},
	{"wa", SUBJECT},
	{"ni", DESTINATION},
	{"watashi", PRONOUN},
	{"anata", PRONOUN},
	{"kare", PRONOUN},
	{"kanojo", PRONOUN},
	{"sore", PRONOUN},
	{"mata", CONNECTOR},
	{"soshite", CONNECTOR},
	{"shikashi", CONNECTOR},
	{"dakara", CONNECTOR},
};

// ------------ Scanner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// Done by: Ryan Santos, Hugh O'Neill
void scanner(tokentype& a, string& w)
{

	fin >> w;  // Grab the next word from the file via fin
	map<string, tokentype>::iterator word_type;

	if (word(w)) {
		word_type = reserved_words.find(w);
		if (word_type != reserved_words.end()) {  // reserved_words[w] exists
			a = word_type->second;
		}
		else {
			char last_c = w.back();
			a = (last_c == 'I' || last_c == 'E') ? WORD2 : WORD1;
		}
	}
	else if (period(w)) {
		a = PERIOD;
	}
	else if (w == "eofm") {
		return;
	}
	else {
		cout << "Lexical error: " << w << " is not a valid token" << endl;
		a = ERROR;
	}

}//the end of scanner


// The temporary test driver to just call the scanner repeatedly  
// This will go away after this assignment
// DO NOT CHANGE THIS!!!!!! 
// Done by:  Rika
int main()
{
	tokentype thetype;
	string theword;
	string filename;

	cout << "Enter the input file name: ";
	cin >> filename;

	fin.open(filename.c_str());

	// the loop continues until eofm is returned.
	while (true) {
		scanner(thetype, theword);  // call the scanner
		if (theword == "eofm") break;  // stop now

		cout << "Type is:" << tokenName[thetype] << endl;
		cout << "Word is:" << theword << endl;
	}

	cout << "End of file is encountered." << endl;
	fin.close();

}// end
