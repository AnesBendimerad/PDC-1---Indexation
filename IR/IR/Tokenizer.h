#pragma once
#include <string>
#include "ITokenizer.h"	
#include "Document.h"
using namespace std;
class Tokenizer : ITokenizer{
private:
	Document document;
public:
	Tokenizer(Document document);
	string getNextToken();
	~Tokenizer();
};