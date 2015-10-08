#include "stdafx.h"
#include "Dictionary.h"

Dictionary::Dictionary(int size, IHasher *hasher)
{
	Dictionary::size = size;
	hashTable = (list<Term>**) malloc(sizeof(list<Term>*)*size);
	for (int i = 0; i < size; i++)
	{
		hashTable[i]= nullptr;
	}
	Dictionary::hasher = hasher;
}

Term* Dictionary::addTerm(string token)
{
	int index = hasher->hash(token) % size;
	list<Term>* cell = hashTable[index];
	if (cell == nullptr)
	{
		Term term;
		term.token = token;
		cell = new list<Term>();
		cell->insert(cell->begin(), term);
		hashTable[index] = cell;
		return &(*(cell->begin()));
	}
	else 
	{
		list<Term>::iterator it = cell->begin();
		while (it != cell->end() && token.compare(it->token)<0)
		{
			it++;
		}
		if (it != cell->end() && token.compare(it->token) == 0)
		{
			it->totalFrequency++;
		}
		else
		{
			Term term;
			term.token = token;
			cell->insert(it, term);
		}
		return &(*it);
	}
}

Term* Dictionary::getTerm(string token)
{
	int index = hasher->hash(token) % size;
	list<Term>* cell = hashTable[index];
	if (cell != nullptr)
	{
		list<Term>::iterator it = cell->begin();
		while (token.compare(it->token) < 0 && it != cell->end())
		{
			it++;
		}
		if (token.compare(it->token) == 0)
		{
			return &(*it);
		}
	}
	return nullptr;
}

Dictionary::~Dictionary()
{
	for (int i = 0; i < size; i++) 
	{
		delete hashTable[i];
	}
	free(hashTable);
}
