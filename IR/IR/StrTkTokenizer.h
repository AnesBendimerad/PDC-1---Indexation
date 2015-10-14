#pragma once
#include <string>
#include "ITokenizer.h"	
#include "Document.h"
#include <deque>
#include<unordered_set>
	
using namespace std;

class StrTkTokenizer : public ITokenizer {
private:
	Document* document;
	deque<string> * tokens;
	int StrTkTokenizer::currentPosition;
	deque<string>* fillDequeWithTokens();
	string delimiters;
	unordered_set<string> * stopWords;
	
public:
	StrTkTokenizer(Document* document);
	string getNextToken();
	void setDelimiters(string delimiters);
	void setStopWords(std::unordered_set<std::string> * stopWords);
	~StrTkTokenizer();
};