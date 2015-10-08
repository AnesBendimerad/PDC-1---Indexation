#include "stdafx.h"
#include "Tokenizer.h"
#include <iostream>
#include <sstream>

Tokenizer::Tokenizer(Document document)
{
	this->document = document;
}

string Tokenizer::getNextToken()
{
	string text = document.getText();
	istringstream iss(text);
	string word;
	iss.seekg(currentPosition);
	while (iss >> word)
	{
		currentPosition = iss.tellg();
		return word;
	}
	return string();
}

Tokenizer::~Tokenizer()
{
}
