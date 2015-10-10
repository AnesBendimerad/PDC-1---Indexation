#pragma once
#include <string>
#include "ITokenizer.h"	
#include "Document.h"
using namespace std;
class Tokenizer : public ITokenizer {
private:
	Document* document;
	unsigned int currentPosition = 0;
	string transformToken(string&);

public:
	Tokenizer(Document* document);
	string getNextToken();
	~Tokenizer();
};