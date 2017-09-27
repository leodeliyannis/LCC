/*
 * Leonardo Deliyannis Constantin
 * Recebe um código-fonte C (sem diretivas de
 * preprocessamento) e devolve seus lexemas e
 * sua tabela de símbolos.
*/

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <vector>
#include <set>
#include <iostream>
using namespace std;
#define BUF_LEN 512
#define ERR_BAD_ARGS (1)
#define ERR_BAD_FILENAME (2)

const string delimiters = 
	"!#$&',./:;<>?@[\\]^_`|~\t\n\r ";

set<string> keywords;
vector<string> lexemes;
vector<string> symbols;

void buildKeywordsTable(){
	vector<string> _reservedWords = {
		"asm", "auto", "break", "case", "char", 
		"const", "continue", "default", "do", 
		"double", "else", "enum", "extern", 
		"float", "for", "goto", "if", "int", 
		"long", "register", "return", "short", 
		"signed", "sizeof", "static", "struct", 
		"switch", "typedef", "union", "unsigned", 
		"void", "volatile", "while"
	};
	for(string _word : _reservedWords){
		keywords.insert(_word);
	}
}

vector<string> explode(const string &str, const string &delim){
	vector<string> ret;
	char *cstr = new (nothrow) char[str.size() + 1];
	if(cstr != nullptr){
		strcpy(cstr, str.c_str());

		for(char *p = strtok(cstr, delim.c_str()); p; 
			p = strtok(NULL, delim.c_str())){
			ret.push_back(p);
		}
		delete [] cstr;
	}
	return ret;
}

void process(const string &line){
	vector<string> tokens = explode(line, delimiters);
	for(auto it = tokens.begin(); it != tokens.end(); ++it){
		keywords.count(*it) ? 
			lexemes.push_back(*it):
			symbols.push_back(*it);
	}
}

void printVector(const vector<string> &v){
	for(auto it = v.begin(); it != v.end(); ++it){
		printf("%s\n", it->c_str());
	}
	printf("\n");
}

inline void prepare(){
	buildKeywordsTable();
}

int main(int argc, char **argv){
	FILE * pFile;
	char buf[BUF_LEN];
	string line;
	prepare();
	if(argc != 2){
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return ERR_BAD_ARGS;
	}
	pFile = fopen(argv[1], "r");
	if(pFile == NULL){
		fprintf(stderr, "FATAL ERROR: could not open file %s\n", argv[1]);
		return ERR_BAD_FILENAME;
	}
	buildKeywordsTable();
	line.clear();
	while(fgets(buf, BUF_LEN, pFile) != NULL){
		line.append(buf);
		if(!line.empty() && line.back() == '\n'){
			process(line);
			line.clear();
		}
	}
	fclose(pFile);
	printf("### LEXEMES: ###\n");
	printVector(lexemes);
	printf("### SYMBOLS: ###\n");
	printVector(symbols);
	return 0;
}

