#include "stdafx.h"
#include "StrTkTokenizer.h"
#include "stopWords.h"
#include "strtk.hpp"


StrTkTokenizer::StrTkTokenizer(Document *document)
{
	this->document = document;
	this->stopWords = nullptr;
	this->tokens=fillDequeWithTokens();
	StrTkTokenizer::currentPosition = 0;

}


string StrTkTokenizer::getNextToken()
{

	while (StrTkTokenizer::currentPosition < tokens->size() && tokens->at(StrTkTokenizer::currentPosition).empty())
			StrTkTokenizer::currentPosition = ++StrTkTokenizer::currentPosition;

		if (StrTkTokenizer::currentPosition < tokens->size())
		{
			document->incrementWordsNumber();
			return tokens->at(StrTkTokenizer::currentPosition = StrTkTokenizer::currentPosition++);
		}

		return "";
}

deque<std::string>* StrTkTokenizer::fillDequeWithTokens(){

	deque<std::string> * tokensList;
	std::unordered_set<std::string> * customStopWords;
	std::string text = document->getText();

	//to lower case
	strtk::convert_to_lowercase(text);
	// setting delimiters
	if (delimiters.empty())
		delimiters = "0123456789""-""\t\r\n ,,.;':\"£$€</> \\""()[]{}<> ";

	//filter stop words
	if (this->stopWords == nullptr){
		customStopWords = getStopWords();
	}
	else
	{
		customStopWords = stopWords;
	}

	strtk::split(" (", text, strtk::functional_inserter(

		[customStopWords](const strtk::range::string& range)
		{
			auto itr = customStopWords->find(range);
			if (customStopWords->end() != itr)
				{
					strtk::fill(range, ' ');
				}
		})
	);
	strtk::remove_consecutives_inplace(' ', text);

	//parse into deque
	tokensList = new deque<std::string>();
	strtk::parse(text, delimiters, *tokensList);

	return tokensList;
}

void StrTkTokenizer::setDelimiters(const string delimiters){
	this->delimiters = delimiters;
}

void StrTkTokenizer::setStopWords(std::unordered_set<std::string> * stopWords){
	this->stopWords = stopWords;
}

StrTkTokenizer::~StrTkTokenizer()
{
	delete tokens;
	tokens = 0;
	delete stopWords;
	stopWords = 0;
	delete document;
	document = 0;

}