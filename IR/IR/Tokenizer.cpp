#include "stdafx.h"
#include "Tokenizer.h"
#include <iostream>
#include <sstream>

Tokenizer::Tokenizer(Document *document)
{
	this->document = document;
}

Tokenizer::~Tokenizer()
{
}


string Tokenizer::getNextToken()
{
	string text = document->getText();
	istringstream iss(text);
	string token;
	iss.seekg(currentPosition);
	while (iss >> token)
	{
		currentPosition = (unsigned int) iss.tellg();
		token = transformToken(token);
		if (token != "")
		{
			document->incrementWordsNumber();
			return token;
		}
	}
	return "";
}

// This function eliminates tags, punctuation and convert tokens to lower case
string Tokenizer::transformToken(string &token)
{
	unsigned i = 0;

	if (token.find("<") == 0 || token.find("</") == 0 || token.find("CHJ=") == 0 || token.find("CVJ=") == 0)
	{
		return "";
	}
	
	else
	{
		while (i < token.length())
		{
			token[i] = tolower(token[i]);

			if (ispunct(token[i]) && token[i] == '.')
			{
				token[i] = ' ';
			}
			else if (ispunct(token[i]) && token[i] != '-' && token[i] != '\'')
			{
				token.erase(i, 1);
			}
			else
			{
				i++;
			}
		}

	}
	return token;
}

