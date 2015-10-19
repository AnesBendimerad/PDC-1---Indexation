#pragma once
#include <list>
#include "IDictionary.h"
#include "IHasher.h"
#include "Term.h"
#define DEFAULT_SIZE 10009
class HashTableDictionary : public IDictionary {
private:
	IHasher * hasher;
	list<Term>  ** hashTable;
	int size;
	unsigned long long termsNumber;
public:
	HashTableDictionary(int size,IHasher *hasher);
	HashTableDictionary();
	Term* addTerm(string token) ;
	void addTerm(Term* term);
	Term* getTerm(string token) ;
	IIterator * getIterator();
	unsigned long long& getTermsNumber();
	unsigned long long getMemorySize();
	~HashTableDictionary();
};