#include "stdafx.h"
#include "DictionaryTermIterator.h"

DictionaryTermIterator::DictionaryTermIterator(int size, list<Term>  ** hashTable)
{
	DictionaryTermIterator::size = size;
	DictionaryTermIterator::hashTable = hashTable;
}

void * DictionaryTermIterator::getNext()
{
	if (currentElement == -1) {
		processNextTermListIndex();
		if (currentElement < size) {
			it = hashTable[currentElement]->begin();
			return &(*it);
		}
	}
	else {
		it++;
		if (it != hashTable[currentElement]->end()) {
			return &(*it);
		}
		else {
			processNextTermListIndex();
			if (currentElement < size) {
				it = hashTable[currentElement]->begin();
				return &(*it);
			}
		}
	}
	return nullptr;
}
void DictionaryTermIterator::processNextTermListIndex()
{
	currentElement++;
	while (currentElement < size && hashTable[currentElement] == nullptr) {
		currentElement++;
	}
}
DictionaryTermIterator::~DictionaryTermIterator()
{
}
