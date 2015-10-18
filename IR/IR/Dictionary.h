#pragma once
#include <list>
#include "IDictionary.h"
#include "IHasher.h"
#include "Term.h"
class Dictionary : public IDictionary {
private:
	IHasher * hasher;
	list<Term>  ** hashTable;
	int size;
	unsigned long long termsNumber;
public:
	Dictionary(int size,IHasher *hasher);
	Term* addTerm(string token) ;
	void addTerm(Term* term);
	Term* getTerm(string token) ;
	IIterator * getIterator();
	unsigned long long& getTermsNumber();
	unsigned long long getMemorySize();
	~Dictionary();
};