#include "stdafx.h"
#include "HashTableDictionary.h"
#include "HashTableDictionaryTermIterator.h"
#include "Hasher.h"

HashTableDictionary::HashTableDictionary(int size, IHasher *hasher)
{
	HashTableDictionary::termsNumber = 0;
	HashTableDictionary::size = size;
	hashTable = (list<Term>**) malloc(sizeof(list<Term>*)*size);
	for (int i = 0; i < size; i++)
	{
		hashTable[i] = nullptr;
	}
	HashTableDictionary::hasher = hasher;
}

HashTableDictionary::HashTableDictionary()
{
	HashTableDictionary::termsNumber = 0;
	HashTableDictionary::size = DEFAULT_SIZE;
	hashTable = (list<Term>**) malloc(sizeof(list<Term>*)*size);
	for (int i = 0; i < size; i++)
	{
		hashTable[i] = nullptr;
	}
	HashTableDictionary::hasher = new Hasher();
}

Term* HashTableDictionary::addTerm(string token)
{
	unsigned int index = hasher->hash(token) % size;
	list<Term>* cell = hashTable[index];
	if (cell == nullptr)
	{
		termsNumber++;
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
		while (it != cell->end() && token.compare(it->token)>0)
		{
			it++;
		}
		if (it != cell->end() && token.compare(it->token) == 0)
		{
			it->totalFrequency++;
		}
		else
		{
			termsNumber++;
			Term term;
			term.token = token;
			cell->insert(it, term);
			it--;
		}
		return &(*it);
	}
}

void HashTableDictionary::addTerm(Term * term)
 {
	unsigned int index = hasher->hash(term->token) % size;
	list<Term>* cell = hashTable[index];
	if (cell == nullptr)
		 {
		termsNumber++;
		cell = new list<Term>();
		cell->insert(cell->begin(), *term);
		hashTable[index] = cell;
		}
	else
		 {
		list<Term>::iterator it = cell->begin();
		while (it != cell->end() && term->token.compare(it->token)>0)
			 {
			it++;
			}
		if (it != cell->end() && term->token.compare(it->token) == 0)
			 {
			throw runtime_error("You can't add Term twice");
			}
		else
			 {
			termsNumber++;
			cell->insert(it, *term);
			}
		}
	}

Term* HashTableDictionary::getTerm(string token)
{
	unsigned int index = hasher->hash(token) % size;
	list<Term>* cell = hashTable[index];
	if (cell != nullptr)
	{
		list<Term>::iterator it = cell->begin();
		while (it != cell->end() && token.compare(it->token) > 0 )
		{
			it++;
		}
		if (it != cell->end() && token.compare(it->token) == 0)
		{
			return &(*it);
		}
	}
	return nullptr;
}

IIterator * HashTableDictionary::getIterator()
{
	return new HashTableDictionaryTermIterator(size,hashTable);
}

unsigned long long& HashTableDictionary::getTermsNumber()
{
	return termsNumber;
}

unsigned long long HashTableDictionary::getMemorySize()
{
	return sizeof(IHasher*)+size*sizeof(list<Term>*)+sizeof(list<Term>  **)+sizeof(int)+sizeof(unsigned long long)+termsNumber*(sizeof(Term)+sizeof(void*));
}

HashTableDictionary::~HashTableDictionary()
{
	for (int i = 0; i < size; i++) 
	{
		delete hashTable[i];
	}
	free(hashTable);
}
