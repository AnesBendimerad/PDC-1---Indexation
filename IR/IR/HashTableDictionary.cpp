#include "stdafx.h"
#include "HashTableDictionary.h"
#include "HashTableDictionaryTermIterator.h"
#include "Hasher.h"
#include <algorithm>

HashTableDictionary::HashTableDictionary(unsigned int size, IHasher *hasher)
{
	HashTableDictionary::termsNumber = 0;
	HashTableDictionary::size = size;
	hashTable = (list<Term>**) malloc(sizeof(list<Term>*)*size);
	for (unsigned int i = 0; i < size; i++)
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
	for (unsigned int i = 0; i < size; i++)
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
		while (it != cell->end() && token.compare(it->token)!=0)
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
			it=cell->insert(cell->end(), term);
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
		while (it != cell->end() && term->token.compare(it->token)!=0)
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
		while (it != cell->end() && token.compare(it->token) != 0 )
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

unsigned long long HashTableDictionary::getTokenId(string token)
{
	unsigned int index = hasher->hash(token) % size;
	unsigned int i = 0;
	list<Term>* cell = hashTable[index];
	if (cell != nullptr)
	{
		list<Term>::iterator it = cell->begin();
		while (it != cell->end() && token.compare(it->token) != 0)
		{
			i++;
			it++;
		}
		if (it != cell->end() && token.compare(it->token) == 0)
		{

			return i*size + index;
		}
		else 
		{
			throw runtime_error("No such token");
		}
	}
	return 0;
}

Term * HashTableDictionary::getTermById(unsigned long long id)
{
	unsigned int index = id % size;
	unsigned int i = id / size;
	list<Term>* cell = hashTable[index];
	if (cell != nullptr)
	{
		list<Term>::iterator it = cell->begin();
		while (it != cell->end() && i>0)
		{
			i--;
			it++;
		}
		if (it != cell->end() && i == 0)
		{

			return &(*it);
		}
	}
	return nullptr;
}

bool HashTableDictionary::compare(const void * firstTerm, const  void *secondTerm)
{
	Term * first = (Term*)firstTerm;
	Term * second = (Term*)secondTerm;

	return (first->totalFrequency > second->totalFrequency);
}

vector<Term*> HashTableDictionary::sortTermsByOccurances(){

	vector <Term*> allTerms;
	IIterator * iterator = getIterator();
	Term * term;

	//retrieve all terms
	while ((term = static_cast<Term*>(iterator->getNext())) != nullptr) {

		allTerms.push_back(term);
	}

	//sort all items
	sort(allTerms.begin(), allTerms.end(), compare);

	return allTerms;
}

void HashTableDictionary::writeCSVFile(){

	vector <Term*> allTerms = sortTermsByOccurances();

	ofstream allTermsCSV("allTermsInTheCorpus.csv");
	allTermsCSV << "Term" << "," << "frequency" << endl;

	for (unsigned int i = 0; i < allTerms.size(); i++){
		allTermsCSV << allTerms[i]->token << "," << allTerms[i]->totalFrequency << endl;
	}
}

HashTableDictionary::~HashTableDictionary()
{
	for (unsigned int i = 0; i < size; i++) 
	{
		delete hashTable[i];
	}
	free(hashTable);
}
