#include "scanner.h"
using namespace std;

//=====================================================
// File scanner.cpp written by: Group Number: 12
//=====================================================

// -----  Tables -------------------------------------

string tokenName[30] = {"ERROR", "WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR"};  // for the display names of tokens

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

// ------------ Scanner -----------------------

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// Done by: Ryan Santos, Hugh O'Neill
void scanner(tokentype& a, string& w)
{
	fin >> w;  // Grab the next word from the file via fin
	cout << "Scanner called using word: " << w << endl;
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
		cout << endl << "Lexical error: " << w << " is not a valid token" << endl;
		a = ERROR;
	}

}//the end of scanner
