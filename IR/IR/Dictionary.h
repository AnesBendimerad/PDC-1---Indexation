#pragma once
#include <list>
#include "IDictionary.h"
#include "IHasher.h"
#include "Term.h"
class Dictionary : IDictionary {
private:
	IHasher * hasher;
	list<Term>  * hashTable;
	int size;
public:
	Dictionary(int size);
	void setHasher(IHasher * hasher);
	Term addTerm(string token) ;
	Term getTerm(string token) ;
	~Dictionary();
};