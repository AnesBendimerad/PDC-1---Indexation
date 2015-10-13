#pragma once
#include <string>
#include "ITokenizer.h"	
#include "Document.h"
using namespace std;
class Tokenizer : public ITokenizer {
private:
	Document* document;
	streampos currentPosition = ios_base::beg;
	string transformToken(string&);
	void preprocess();
public:
	Tokenizer(Document* document);
	string getNextToken();
	~Tokenizer();
};