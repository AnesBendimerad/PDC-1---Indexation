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

		if (StrTkTokenizer::currentPosition < ((int)(tokens->size())))
		{
			document->incrementWordsNumber();
			return tokens->at(StrTkTokenizer::currentPosition = StrTkTokenizer::currentPosition++);
		}
		delete tokens;
		tokens = 0;
		delete stopWords;
		stopWords = 0;
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

	// Second filter on StopWords
	//The first filter is over-all mandatory to perform split of the string !
	unsigned int i=0;
	while (i < tokensList->size()){
		auto itr = customStopWords->find(tokensList->at(i));
		if (customStopWords->end() != itr)
		{
			tokensList->erase(tokensList->begin() + i);
			i--;
		}
		i++; 
	}
	delete customStopWords;
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
	cout << "destructeur" << endl;
	delete tokens;
	tokens = 0;
	delete stopWords;
	stopWords = 0;
	delete document;
	document = 0;

}