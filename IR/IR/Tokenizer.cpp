#include "stdafx.h"
#include "Tokenizer.h"
#include <iostream>
#include <sstream>

Tokenizer::Tokenizer(Document *document)
{
	this->document = document;
	preprocess();
}

Tokenizer::~Tokenizer()
{
}


// This function returns token by token the text in the stringstream
string Tokenizer::getNextToken()
{
	string text = document->getText();
	istringstream iss(text);
	string token;

	iss.seekg(currentPosition);
	while (iss >> token)
	{
		currentPosition = iss.tellg();
		document->incrementWordsNumber();
		return token;
	}
	return "";
}

// This function eliminates HTML tags, punctuation and convert tokens to lower case
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

			if (ispunct(token[i]) && i == token.length()-1)
			{
				token.erase(i, 1);
			}
		   else if (ispunct(token[i]) && token[i] != '-' && token[i] != '\'' && token[i] != '.')
			{
				token.erase(i, 1);
			}
		   else if (ispunct(token[i]) && token[i] == '.')
		   {
			   token[i] = ' ';
		   }
			else
			{
				i++;
			}
		}

	}
	return token;
}

// This function do some preprocessing in the text before being used by getNextToken function
void Tokenizer:: preprocess()
{
	string text = document->getText();
	istringstream iss(text);
	string token;
	string transformedText;

	while (iss >> token)
	{
		token = transformToken(token);
		if (token != "")
		{
			transformedText += token + " ";
		}
	}
	document->setText(transformedText);
}