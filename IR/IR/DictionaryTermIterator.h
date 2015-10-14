#pragma once
#include <list>
#include "IIterator.h"
#include "Term.h"
class DictionaryTermIterator : public IIterator {
private:
	list<Term>  ** hashTable;
	int size;
	int currentElement = -1;
	list<Term>::iterator it;
	
	void processNextTermListIndex();
public:
	DictionaryTermIterator(int size, list<Term>  ** hashTable);
	void*  getNext();
	~DictionaryTermIterator();
};